add_library(module_lib INTERFACE)

target_link_libraries(module_lib INTERFACE module_2)

add_subdirectory_ifdef(CONFIG_MODULE_1 module_1)
add_subdirectory_ifdef(CONFIG_MODULE_2 module_2)
