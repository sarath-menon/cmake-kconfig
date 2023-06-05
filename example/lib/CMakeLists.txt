add_library(module_lib INTERFACE)

add_module_ifdef(CONFIG_MODULE_1 module_lib module_1)
add_module_ifdef(CONFIG_MODULE_2 module_lib module_2)

# target_link_libraries(module_lib INTERFACE module_1)

# add_subdirectory_ifdef(CONFIG_MODULE_1 module_1)
# add_subdirectory_ifdef(CONFIG_MODULE_2 module_2)
