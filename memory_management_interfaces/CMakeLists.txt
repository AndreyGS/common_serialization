cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_memory_management_interfaces VERSION 0.1.0)

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT_INTERFACES "Build memory management interfaces project" ON)

if (NOT TARGET cs::common_lib)
    find_package(ags_common_serialization_common_lib 0.1.0 REQUIRED)
endif()

add_library(memory_management_interfaces_lib INTERFACE)
add_library(cs::memory_management_interfaces_lib ALIAS memory_management_interfaces_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/memory_management_interfaces")
set(HEADERS
    "${HEADERS_DIR}/IStorageAllocator.h"
    "${HEADERS_DIR}/memory_management_interfaces.h"
)

target_sources(memory_management_interfaces_lib
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HEADERS}
)

target_include_directories(memory_management_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(memory_management_interfaces_lib
    INTERFACE
        cs::common_lib
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")
export_and_install_lib("${CMAKE_CURRENT_SOURCE_DIR}/../cmake" "memory_management_interfaces_lib" ${PROJECT_VERSION})