cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_allocation_manager_interfaces VERSION 0.1.0)

option(COMMON_SERIALIZATION_ALLOCATION_MANAGER_INTERFACES "Build allocation manager interfaces project" ON)

if (NOT TARGET ags_common_serialization::allocator_interfaces_lib)
    find_package(ags_cs_allocator_interfaces_lib 0.1.0 REQUIRED)
endif()

add_library(ags_cs_allocation_manager_interfaces_lib INTERFACE)
add_library(ags_common_serialization::allocation_manager_interfaces_lib ALIAS ags_cs_allocation_manager_interfaces_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/allocation_manager_interfaces")
set(HEADERS
    "${HEADERS_DIR}/allocation_manager_interface.h"
    "${HEADERS_DIR}/IAllocationManager.h"
    "${HEADERS_DIR}/IAllocationStrategyUser.h"
)

target_sources(ags_cs_allocation_manager_interfaces_lib
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HEADERS}
)

target_include_directories(ags_cs_allocation_manager_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

target_link_libraries(ags_cs_allocation_manager_interfaces_lib
    INTERFACE
        ags_common_serialization::allocator_interfaces_lib
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")
export_and_install_lib("${CMAKE_CURRENT_SOURCE_DIR}/../cmake" "ags_cs_allocation_manager_interfaces_lib" ${PROJECT_VERSION})