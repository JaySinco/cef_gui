cmake_minimum_required(VERSION 3.2)

macro(configure_msvc_runtime)
  if(MSVC)
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()

project(NetTools)

configure_msvc_runtime()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/3rdparty/libcef/
    )
message(STATUS "Include dir -> " ${INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

set(LINK_DIR
    ${CMAKE_SOURCE_DIR}/3rdparty/libcef/lib/
    )
message(STATUS "Link dir -> " ${LINK_DIR})
link_directories(${LINK_DIR})

add_executable(cefsimple WIN32
  ${CMAKE_SOURCE_DIR}/src/cefsimple_win.cc
  ${CMAKE_SOURCE_DIR}/src/simple_app.cc
  ${CMAKE_SOURCE_DIR}/src/simple_handler.cc
  ${CMAKE_SOURCE_DIR}/src/simple_handler_win.cc
  )
target_link_libraries(cefsimple
  libcef
  libcef_dll_wrapper
  cef_sandbox
  )
add_custom_command(TARGET cefsimple
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/3rdparty/libcef/bin/
  $<TARGET_FILE_DIR:cefsimple>
  )