cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_tests_serializable_structs)

add_library(ags_cs_tests_serializable_structs_lib INTERFACE)
add_library(ags_common_serialization::tests_serializable_structs ALIAS ags_cs_tests_serializable_structs_lib)

set(SOURCE_INCLUDE_DIR "")

set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/tests_special_types")
set(HEADERS
    "${HEADERS_DIR}/tests_special_types.h"
    "${HEADERS_DIR}/special_types.h"
)

target_sources(ags_cs_tests_serializable_structs_lib
    INTERFACE
        ${HEADERS}
)

target_include_directories(ags_cs_tests_serializable_structs_lib
    INTERFACE
        $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
)

target_link_libraries(${LIB_NAME}
    INTERFACE
        ags_common_serialization::csp_base
)