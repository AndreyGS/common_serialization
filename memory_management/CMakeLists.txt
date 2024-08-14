cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_memory_management VERSION 0.1.0)

option(AGS_CS_BUILD_MEMORY_MANAGEMENT "Build ags common serialization memory management project" ON)

set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/memory_management")
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

set(LIBS_TO_LINK "ags_common_serialization::memory_management_interfaces_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("memory_management_lib" ${PROJECT_VERSION} "${HEADERS}" "${LIBS_TO_LINK}" "ON" "${CMAKE_CURRENT_SOURCE_DIR}" "ON")
