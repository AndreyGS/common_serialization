cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_common VERSION 0.1.0)

option(AGS_CS_BUILD_COMMON "Build ags common serialization common project" ON)

set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/common")
set(HEADERS
    "${HEADERS_DIR}/Status.h"
    "${HEADERS_DIR}/Concepts.h"
    "${HEADERS_DIR}/Helpers.h"
    "${HEADERS_DIR}/std_equivalents.h"
    "${HEADERS_DIR}/Uuid.h"
    "${HEADERS_DIR}/interfaces/IIoProcessor.h"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

ags_cs_add_interface_lib("common_lib" ${PROJECT_VERSION} "${HEADERS}" "" "" "${CMAKE_CURRENT_SOURCE_DIR}" "ON")
