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

project(cef_gui)

configure_msvc_runtime()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/external/libcef/
    )
message(STATUS "Include dir -> " ${INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

set(LINK_DIR
    ${CMAKE_SOURCE_DIR}/external/libcef/lib/
    )
message(STATUS "Link dir -> " ${LINK_DIR})
link_directories(${LINK_DIR})

add_executable(demo WIN32
  ${CMAKE_SOURCE_DIR}/core/common.cpp
  ${CMAKE_SOURCE_DIR}/core/basic_client.cpp
  ${CMAKE_SOURCE_DIR}/core/basic_app.cpp
  ${CMAKE_SOURCE_DIR}/core/msg_handler.cpp
  ${CMAKE_SOURCE_DIR}/core/demo.cpp
  )

target_link_libraries(demo
  libcef
  libcef_dll_wrapper
  cef_sandbox
  )

add_custom_command(TARGET demo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/external/libcef/bin/
  $<TARGET_FILE_DIR:demo>
  )