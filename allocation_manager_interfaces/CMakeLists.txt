cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_allocation_manager_interfaces VERSION 0.1.0)

option(AGS_CS_BUILD_ALLOCATION_MANAGER_INTERFACES "Build ags common serialization allocation manager interfaces project" ON)

set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/allocation_manager_interfaces")
set(LIB_HEADERS
    "${LIB_HEADERS_DIR}/allocation_manager_interface.h"
    "${LIB_HEADERS_DIR}/IAllocationManager.h"
    "${LIB_HEADERS_DIR}/IAllocationStrategyUser.h"
)

set(LIBS_TO_LINK "ags_common_serialization::allocator_interfaces_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("allocation_manager_interfaces_lib" ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" "ON" "${CMAKE_CURRENT_SOURCE_DIR}" "ON")
