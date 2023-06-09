// C library headers
#include "crtp.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
using namespace std::chrono_literals;

// Linux headers
#include <errno.h>   // Error integer and strerror() function
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()

std::mutex io_mutex;

void receive_data(int serial_port) {
  uint8_t readBuffer[64]{};
  CRTPPacket packet{};

  int num_bytes{};

  for (;;) {
    {
      std::lock_guard<std::mutex> lock(io_mutex);
      num_bytes = read(serial_port, &readBuffer, sizeof(readBuffer));
    }

    if (num_bytes < 0) {
      std::lock_guard<std::mutex> lock(io_mutex);
      printf("Error reading: %s", strerror(errno));
      // return 1;
    }

    memcpy(&packet.data, &readBuffer[1], sizeof(packet.data));
    {
      std::lock_guard<std::mutex> lock(io_mutex);
      std::cout << "Data:" << unsigned(packet.data[1]) << '\n';
    }
    std::this_thread::sleep_for(1ms);
  }
}

int main() {
  // Open the serial port. Change device path as needed (currently set to an
  // standard FTDI USB-UART cable type device)
  int serial_port = open("/tmp/mytty", O_RDWR);
  CRTPPacket packet{};

  // create thread to receive data
  std::thread thread_obj(receive_data, serial_port);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if (tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in
                          // communication (most common)
  tty.c_cflag &= ~CSIZE;  // Clear all bits that set the data size
  tty.c_cflag |= CS8;     // 8 bits per byte (most common)
  tty.c_cflag &=
      ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |=
      CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;   // Disable echo
  tty.c_lflag &= ~ECHOE;  // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                   ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g.
                         // newline chars)
  tty.c_oflag &=
      ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT
  // PRESENT ON LINUX) tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars
  // (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon
                        // as any data is received.
  tty.c_cc[VMIN] = sizeof(packet.data);

  // Set in/out baud rate to be 115200
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    return 1;
  }

  // // Write to serial port
  // unsigned char msg[] = {'H', 'e', 'l', 'l', 'o', '\r'};
  // write(serial_port, msg, sizeof(msg));

  // // Normally you wouldn't do this memset() call, but since we will just
  // receive
  // // ASCII data for this example, we'll set everything to 0 so we can
  // // call printf() easily.
  // memset(&read_buf, '\0', sizeof(read_buf));

  // Allocate memory for send buffer, set size according to your needs
  const std::uint8_t sendBuffer[64] = {"selva\n"};

  for (;;) {

    {
      std::lock_guard<std::mutex> lock(io_mutex);

      // write received data back to serial port
      write(serial_port, sendBuffer, sizeof(sendBuffer));

      std::cout << "Sent Data:" << sendBuffer << '\n';
    }

    std::this_thread::sleep_for(1ms);
  }
  close(serial_port);
  return 0; // success
};