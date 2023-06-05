cmake_minimum_required(VERSION 3.13)
project(cmake_kconfig VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(
  kconfig-test
  VERSION 1.0
  DESCRIPTION "kconfig test project"
  LANGUAGES C)

include(top)

if(CONFIG_TEST_OPTION)
  message("Config test_option enabled")
else(CONFIG_TEST_OPTION)
  message("Config test_option disabled")
endif()

add_subdirectory(example)
