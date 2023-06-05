add_executable(app src/main.cpp)

target_include_directories(app PUBLIC include ${AUTOCONF_DIR})

target_link_libraries(app PRIVATE module_lib)
