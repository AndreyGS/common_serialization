cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_memory_management_interfaces VERSION 0.1.0)

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT_INTERFACES "Build memory management interfaces project" ON)

set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/memory_management_interfaces")
set(HEADERS
    "${HEADERS_DIR}/IStorageAllocator.h"
    "${HEADERS_DIR}/memory_management_interfaces.h"
)
set(DEPENDENCIES "ags_common_serialization::common_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("memory_management_interfaces_lib")
