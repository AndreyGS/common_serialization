cmake_minimum_required(VERSION 3.28)

add_library(ags_common_serialization_tests_csp_with_std_interface_included INTERFACE)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar (but simplified) to ags_cs_create_allocators_lib()
function(ags_cs_create_tests_csp_with_std_interface_lib 
    AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_LIB_NAME  
    AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_ALIAS
)
    project(ags_common_serialization_tests_csp_with_std_interface)

    set(UNQUALIFIED_LIB_NAME "tests_csp_with_std_interface_lib")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_LIB_NAME}"
        "${AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/tests_csp_with_std_interface")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/structs.h"
        "${LIB_HEADERS_DIR}/tests_csp_with_std_interface.h"
        "${LIB_HEADERS_DIR}/csp_processing_data/generated/BodyProcessor.h"
    )

    set(LIB_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
    set(LIB_SOURCES
        "${LIB_SOURCES_DIR}/csp_processing_data/generated/BodyProcessor.cpp"
    )

    if ("${AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_LIB_NAME}" STREQUAL "")
        set(LIBS_TO_LINK "ags_common_serialization::tests_restricted_structs_lib")
    else()
        set(LIBS_TO_LINK "${AGS_CS_CUSTOM_TESTS_RESTRICTED_STRUCTS_LIB_NAME}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}" 
        ""
        "${LIB_HEADERS}"
        "${LIB_SOURCES}" 
        "${LIBS_TO_LINK}"
        "OFF"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" 
        "OFF"
    )

    set(QUALIFIED_TEST_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

endfunction()
