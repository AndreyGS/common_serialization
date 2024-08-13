cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_memory_management VERSION 0.1.0)

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT "Build memory management project" ON)

if (NOT TARGET cs::memory_management_interfaces_lib)
    find_package(ags_common_serialization_memory_management_interfaces_lib 0.1.0 REQUIRED)
endif()

add_library(memory_management_lib INTERFACE)
add_library(cs::memory_management_lib ALIAS memory_management_lib)

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

target_sources(memory_management_lib
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HEADERS}
)

target_include_directories(memory_management_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(memory_management_lib
    INTERFACE
        cs::memory_management_interfaces_lib
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")
export_and_install_lib("${CMAKE_CURRENT_SOURCE_DIR}/../cmake" "memory_management_lib" ${PROJECT_VERSION})
