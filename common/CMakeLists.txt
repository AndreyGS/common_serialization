cmake_minimum_required(VERSION 3.26)

project(cs_common)

add_library(cs_common_lib INTERFACE)
add_library(cs::commonlib ALIAS cs_common_lib)

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

target_sources(cs_common_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(cs_common_lib
    INTERFACE 
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_compile_features(cs_common_lib INTERFACE cxx_std_20)
target_compile_definitions(cs_common_lib INTERFACE _HAS_CXX20)
