cmake_minimum_required(VERSION 3.26)

project(cs_allocator_interfaces)

add_library(cs_allocator_interfaces_lib INTERFACE)
add_library(cs::allocator_interfaces_lib ALIAS cs_allocator_interfaces_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/allocator_interfaces")
set(HEADERS
    "${HEADERS_DIR}/allocator_interface.h"
    "${HEADERS_DIR}/IAllocator.h"
    "${HEADERS_DIR}/IAllocatorTraits.h"
    "${HEADERS_DIR}/IStorageSetter.h"
)

target_sources(cs_allocator_interfaces_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(cs_allocator_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(cs_allocator_interfaces_lib
    INTERFACE
        cs::commonlib
)
