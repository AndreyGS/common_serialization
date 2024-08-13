cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_common VERSION 0.1.0)

option(COMMON_SERIALIZATION_BUILD_COMMON "Build common project" ON)

add_library(common_lib INTERFACE)
add_library(cs::common_lib ALIAS common_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/common")
set(HEADERS
    "${HEADERS_DIR}/Status.h"
    "${HEADERS_DIR}/Concepts.h"
    "${HEADERS_DIR}/Helpers.h"
    "${HEADERS_DIR}/std_equivalents.h"
    "${HEADERS_DIR}/Uuid.h"
    "${HEADERS_DIR}/interfaces/IIoProcessor.h"
)

target_sources(common_lib
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HEADERS}
)

target_include_directories(common_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

target_compile_features(common_lib INTERFACE cxx_std_20)
target_compile_definitions(common_lib INTERFACE _HAS_CXX20)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")
export_and_install_lib("${CMAKE_CURRENT_SOURCE_DIR}/../cmake" "common_lib" ${PROJECT_VERSION})
