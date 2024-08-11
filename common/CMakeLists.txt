cmake_minimum_required(VERSION 3.26)

project(common_serialization_common)

add_library(common_serialization_common_lib INTERFACE)
add_library(common_serialization::commonlib ALIAS common_serialization_common_lib)

set(COMMON_INCLUDE_DIR "include/common_serialization/common")
set(COMMON_HEADERS
    "${COMMON_INCLUDE_DIR}/Status.h"
    "${COMMON_INCLUDE_DIR}/Concepts.h"
    "${COMMON_INCLUDE_DIR}/Helpers.h"
    "${COMMON_INCLUDE_DIR}/std_equivalents.h"
    "${COMMON_INCLUDE_DIR}/Uuid.h"
    "${COMMON_INCLUDE_DIR}/interfaces/IIoProcessor.h"
)

target_sources(common_serialization_common_lib
    INTERFACE
        ${COMMON_HEADERS}
)

target_include_directories(common_serialization_common_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(common_serialization_common_lib INTERFACE cxx_std_20)
target_compile_definitions(common_serialization_common_lib INTERFACE _HAS_CXX20)
