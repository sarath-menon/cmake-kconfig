cmake_minimum_required(VERSION 3.13)
project(cmake_kconfig VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

# find_package(PkgConfig) pkg_check_modules(SERIAL libserial)

add_subdirectory(apps)
# add_subdirectory(external)
