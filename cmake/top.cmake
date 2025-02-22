set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(KCONFIG_ROOT ${CMAKE_SOURCE_DIR}/Kconfig)
set(RL_CONFIG_DIR ${CMAKE_SOURCE_DIR}/configs)
set(AUTOCONF_DIR ${CMAKE_CURRENT_BINARY_DIR}/kconfig/include/generated)
set(AUTOCONF_FILE ${CMAKE_CURRENT_BINARY_DIR}/include/generated/autoconf.h)

# Re-configure (Re-execute all CMakeLists.txt code) when autoconf.h changes
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS ${AUTOCONF_FILE})

include(cmake/extensions.cmake)
include(cmake/python.cmake)
include(cmake/kconfig.cmake)
include(cmake/helper.cmake)
