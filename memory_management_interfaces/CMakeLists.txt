cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_memory_management_interfaces VERSION 0.1.0)

option(AGS_CS_BUILD_MEMORY_MANAGEMENT_INTERFACES "Build ags common serialization memory management interfaces project" ON)

set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/memory_management_interfaces")
set(LIB_HEADERS
    "${LIB_HEADERS_DIR}/IStorageAllocator.h"
    "${LIB_HEADERS_DIR}/memory_management_interfaces.h"
)
set(LIBS_TO_LINK "ags_common_serialization::common_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("memory_management_interfaces_lib" ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" "ON" "${CMAKE_CURRENT_SOURCE_DIR}" "ON")
