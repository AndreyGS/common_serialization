cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_common_serialization_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_common_serialization_allocators_included INTERFACE)

option(AGS_CS_BUILD_ALLOCATORS "Build ags common serialization allocators project" ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Creates allocators library optionally customized with third-party memory management library and/or memory management typedefs.
# If library/package is not customized (all given arguments are empty strings) it will be named 'ags_cs_allocators_lib' ('ags_common_serialization::allocators_lib')
# otherwise 'ags_cs_allocators_lib_customized[_x_<custom_alias1>[_x_<custom_alias2>...]]' and respective qualified alias.
#
# Note: custom library and/or typedef's header must be already available in current build at the time of this function call
#
# Params:
#   EXPORT_AND_INSTALL_LIB: should library be exported and installed
#
#   BUILD_UNIT_TESTS: should unit-tests be built.
#       Must almost everytime be "ON", but in some specific circumstances with custom libs it may be necessary to not include all standard tests.
#
#   AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_NAME: name of custom memory management library that can be used in linking expressions
#       If this argument is empty string, then AGS_CS_CUSTOM_MEMORY_MANAGEMENT_ALIAS must be empty.
#
#   AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_VERSION: required version of custom memory management library.
#       This argument can be empty.
#
#   AGS_CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH: path to custom header typedefs. For example 'common_serialization/memory_management/typedefs2.h'.
#       Note: path must be without quotes or double quotes on the edges.
#
#   AGS_CS_CUSTOM_MEMORY_MANAGEMENT_ALIAS: alias that will be used in custom allocators library/package naming, it doesn't have to matter to the linker.
#       Note: it should contain only C-style naming characters.
#       Note: try make it shorter.
#
# Returns:
#   QUALIFIED_LIB_NAME name of created lib with namespace.
function(ags_cs_create_allocators_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
    AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_NAME 
    AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_VERSION
    AGS_CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH 
    AGS_CS_CUSTOM_MEMORY_MANAGEMENT_ALIAS
)
    project(ags_common_serialization_allocators VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "allocators_lib")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}"
        "${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/allocators")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/allocators.h"
        "${LIB_HEADERS_DIR}/allocators_config.h"
        "${LIB_HEADERS_DIR}/ConstructorNoexceptAllocator.h"
        "${LIB_HEADERS_DIR}/RawKeeperAllocator.h"
        "${LIB_HEADERS_DIR}/RawNoexceptAllocator.h"
        "${LIB_HEADERS_DIR}/typedefs.h"
    )

    set(LIBS_TO_LINK "ags_common_serialization::allocator_interfaces_lib;0.1.0")
    if ("${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}" STREQUAL "")
        string(APPEND LIBS_TO_LINK ";ags_common_serialization::memory_management_lib;0.1.0")
    else()
        string(APPEND LIBS_TO_LINK ";${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_NAME};${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_LIB_VERSION}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}" 
        ${PROJECT_VERSION} 
        "${LIB_HEADERS}" 
        "${LIBS_TO_LINK}"
        "ON"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" 
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (BUILD_UNIT_TESTS)
        set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
        set(TESTS_SOURCES
            "${TESTS_SOURCES_DIR}/RawNoexceptAllocatorTests.cpp"
            "${TESTS_SOURCES_DIR}/RawKeeperAllocatorTests.cpp"
            "${TESTS_SOURCES_DIR}/ConstructorNoexceptAllocatorTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;${LIB_NAME};ags_common_serialization::tests_special_types_lib")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
