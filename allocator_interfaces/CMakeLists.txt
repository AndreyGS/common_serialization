cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_allocator_interfaces VERSION 0.1.0)

option(AGS_CS_BUILD_ALLOCATOR_INTERFACES "Build ags common serialization allocator interfaces project" ON)

set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/allocator_interfaces")
set(HEADERS
    "${HEADERS_DIR}/allocator_interface.h"
    "${HEADERS_DIR}/IAllocator.h"
    "${HEADERS_DIR}/IAllocatorTraits.h"
    "${HEADERS_DIR}/IStorageSetter.h"
)

set(LIBS_TO_LINK "ags_common_serialization::common_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("allocator_interfaces_lib" ${PROJECT_VERSION} "${HEADERS}" "${LIBS_TO_LINK}" "ON" "${CMAKE_CURRENT_SOURCE_DIR}" "ON")
