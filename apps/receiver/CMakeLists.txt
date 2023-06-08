add_executable(receiver src/main.cpp)
target_include_directories(receiver PUBLIC include ${SERIAL_INCLUDE_DIRS})
