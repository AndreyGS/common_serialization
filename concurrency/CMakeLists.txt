cmake_minimum_required(VERSION 3.28)

project(ags_cs_concurrency VERSION 0.1.0)

set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/concurrency")
set(LIB_HEADERS
    "${LIB_HEADERS_DIR}/concurrency.h"
    "${LIB_HEADERS_DIR}/typedefs.h"
    "${LIB_HEADERS_DIR}/platform_dependent/switch.h"
    "${LIB_HEADERS_DIR}/platform_dependent/user_mode.h"
)

set(LIBS_TO_LINK "ags_cs::concurrency_interfaces_lib;0.1.0")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib(concurrency_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" ON "${CMAKE_CURRENT_SOURCE_DIR}" ON)

set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)
