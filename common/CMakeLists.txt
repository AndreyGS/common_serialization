cmake_minimum_required(VERSION 3.28)

project(ags_cs_common VERSION 0.1.0)

set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/common")
set(LIB_HEADERS
    "${LIB_HEADERS_DIR}/common.h"
    "${LIB_HEADERS_DIR}/concepts.h"
    "${LIB_HEADERS_DIR}/helpers.h"
    "${LIB_HEADERS_DIR}/std_equivalents.h"
    "${LIB_HEADERS_DIR}/traits.h"
    "${LIB_HEADERS_DIR}/Status.h"
    "${LIB_HEADERS_DIR}/Uuid.h"
    "${LIB_HEADERS_DIR}/interfaces/IIoProcessor.h"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib(common_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "" "" "${CMAKE_CURRENT_SOURCE_DIR}" ON)
