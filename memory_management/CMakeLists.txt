cmake_minimum_required(VERSION 3.26)

project(cs_memory_management)

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT "Build memory management project" ON)

add_library(cs_memory_management_lib INTERFACE)
add_library(cs::memory_management_lib ALIAS cs_memory_management_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/memory_management")
set(HEADERS
    "${HEADERS_DIR}/delete_replacements.h"
    "${HEADERS_DIR}/memory_management.h"
    "${HEADERS_DIR}/new_replacements.h"
    "${HEADERS_DIR}/typedefs.h"
    "${HEADERS_DIR}/platform_dependent/linux_kernel.h"
    "${HEADERS_DIR}/platform_dependent/switch.h"
    "${HEADERS_DIR}/platform_dependent/user_mode.h"
    "${HEADERS_DIR}/platform_dependent/windows_kernel.h"
)

target_sources(cs_memory_management_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(cs_memory_management_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(cs_memory_management_lib
    INTERFACE
        cs::commonlib
        cs::memory_management_interfaces_lib
)
