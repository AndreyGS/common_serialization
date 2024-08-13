cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_allocator_interfaces VERSION 0.1.0)

option(COMMON_SERIALIZATION_BUILD_ALLOCATOR_INTERFACES "Build allocator interfaces project" ON)

if (NOT TARGET ags_common_serialization::common_lib)
    find_package(ags_common_serialization_common_lib 0.1.0 REQUIRED)
endif()

add_library(ags_cs_allocator_interfaces_lib INTERFACE)
add_library(ags_common_serialization::allocator_interfaces_lib ALIAS ags_cs_allocator_interfaces_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/allocator_interfaces")
set(HEADERS
    "${HEADERS_DIR}/allocator_interface.h"
    "${HEADERS_DIR}/IAllocator.h"
    "${HEADERS_DIR}/IAllocatorTraits.h"
    "${HEADERS_DIR}/IStorageSetter.h"
)

target_sources(ags_cs_allocator_interfaces_lib
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HEADERS}
)

target_include_directories(ags_cs_allocator_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

target_link_libraries(ags_cs_allocator_interfaces_lib
    INTERFACE
        ags_common_serialization::common_lib
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")
export_and_install_lib("${CMAKE_CURRENT_SOURCE_DIR}/../cmake" "ags_cs_allocator_interfaces_lib" ${PROJECT_VERSION})
