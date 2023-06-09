#include "crtp.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std::chrono_literals;

std::mutex io_mutex;

void receive_data() {
  uint8_t readBuffer[64]{};

  for (;;) {

    {
      std::lock_guard<std::mutex> lock(io_mutex);
      std::cin >> readBuffer;
    }
    std::this_thread::sleep_for(1ms);
  }
}

int main() {

  // create thread to receive data
  std::thread thread_obj(receive_data);

  for (;;) {
    //     std::cout << "Please enter the name of the serial device" << '\n';
    // char i;
    // std::cin >> i;
    // std::cout << "Character received:" << i << "\r\n";

    uint8_t sendBuffer[64]{};

    CRTPPacket packet{};
    CRTPPacket packet_2{};

    int a = 6;

    packet.header = 6;
    packet.data[0] = 1;
    packet.data[1] = 2;
    packet.data[2] = 3;
    packet.size = 1;

    sendBuffer[0] = packet.header;

    if (packet.size <= CRTP_MAX_DATA_SIZE) {
      memcpy(&sendBuffer[1], packet.data, sizeof(packet.data));
    }

    // memcpy(&packet_2.data, &sendBuffer[1], sizeof(packet.data));

    // std::cout << "Data:" << unsigned(packet_2.data[0]) << '\n';

    {
      std::lock_guard<std::mutex> lock(io_mutex);
      std::cout << sendBuffer << std::flush;
    }
    std::this_thread::sleep_for(1ms);

    // receive data
    // std::cin >> readBuffer;
    // // std::cout << readBuffer << std::flush;

    // std::this_thread::sleep_for(1ms);
  }
}