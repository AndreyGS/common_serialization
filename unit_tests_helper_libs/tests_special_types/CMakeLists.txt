cmake_minimum_required(VERSION 3.28)

project(ags_cs_tests_special_types)

add_library(ags_cs_tests_special_types_lib INTERFACE)
add_library(ags_cs::tests_special_types_lib ALIAS ags_cs_tests_special_types_lib)

set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/tests_special_types")
set(LIB_HEADERS
    "${LIB_HEADERS_DIR}/tests_special_types.h"
    "${LIB_HEADERS_DIR}/structs.h"
)

target_sources(ags_cs_tests_special_types_lib
    INTERFACE
        ${LIB_HEADERS}
)

target_include_directories(ags_cs_tests_special_types_lib
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${LIB_NAME}
    INTERFACE
        ags_cs::common_lib
)