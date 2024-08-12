cmake_minimum_required(VERSION 3.26)

project(cs_tests_special_types)

add_library(cs_tests_special_types_lib INTERFACE)
add_library(cs::tests_special_types_lib ALIAS cs_tests_special_types_lib)

set(SOURCE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/cs_tests_special_types")
set(HEADERS
    "${HEADERS_DIR}/cs_tests_special_types.h"
    "${HEADERS_DIR}/special_types.h"
)

target_sources(cs_tests_special_types_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(cs_tests_special_types_lib
    INTERFACE
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)
