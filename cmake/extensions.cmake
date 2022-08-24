

# 2.2 Misc
#
# import_kconfig(<prefix> <kconfig_fragment> [<keys>])
#
# Parse a KConfig fragment (typically with extension .config) and
# introduce all the symbols that are prefixed with 'prefix' into the
# CMake namespace. List all created variable names in the 'keys'
# output variable if present.
function(import_kconfig prefix kconfig_fragment)
  # Parse the lines prefixed with 'prefix' in ${kconfig_fragment}
  file(
    STRINGS
    ${kconfig_fragment}
    DOT_CONFIG_LIST
    REGEX "^${prefix}"
    ENCODING "UTF-8"
  )

  foreach (CONFIG ${DOT_CONFIG_LIST})
    # CONFIG could look like: CONFIG_NET_BUF=y

    # Match the first part, the variable name
    string(REGEX MATCH "[^=]+" CONF_VARIABLE_NAME ${CONFIG})

    # Match the second part, variable value
    string(REGEX MATCH "=(.+$)" CONF_VARIABLE_VALUE ${CONFIG})
    # The variable name match we just did included the '=' symbol. To just get the
    # part on the RHS we use match group 1
    set(CONF_VARIABLE_VALUE ${CMAKE_MATCH_1})

    if("${CONF_VARIABLE_VALUE}" MATCHES "^\"(.*)\"$") # Is surrounded by quotes
      set(CONF_VARIABLE_VALUE ${CMAKE_MATCH_1})
    endif()

    set("${CONF_VARIABLE_NAME}" "${CONF_VARIABLE_VALUE}" PARENT_SCOPE)
    list(APPEND keys "${CONF_VARIABLE_NAME}")
  endforeach()

  foreach(outvar ${ARGN})
    set(${outvar} "${keys}" PARENT_SCOPE)
  endforeach()
endfunction()
