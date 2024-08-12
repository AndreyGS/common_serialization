cmake_minimum_required(VERSION 3.26)

project(cs_memory_management_interfaces)

add_library(cs_memory_management_interfaces_lib INTERFACE)
add_library(cs::memory_management_interfaces_lib ALIAS cs_memory_management_interfaces_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/memory_management_interfaces")
set(HEADERS
    "${HEADERS_DIR}/IStorageAllocator.h"
    "${HEADERS_DIR}/memory_management_interfaces.h"
)

target_sources(cs_memory_management_interfaces_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(cs_memory_management_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(cs_memory_management_interfaces_lib
    INTERFACE
        cs::commonlib
)
