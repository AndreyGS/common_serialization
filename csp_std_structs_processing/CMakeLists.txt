cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_cs_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_cs_csp_std_structs_processing_included INTERFACE)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_csp_std_structs_processing_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
    CUSTOM_CSP_BASE_LIB_NAME 
    CUSTOM_CSP_BASE_LIB_VERSION
    CUSTOM_CSP_BASE_ALIAS 
)
    project(ags_cs_csp_std_structs_processing VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "csp_std_structs_processing_lib")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_CSP_BASE_LIB_NAME}"
        "${CUSTOM_CSP_BASE_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/csp_std_structs_processing")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/csp_processing_data/TemplateProcessorStd.h"
    )

    if ("${CUSTOM_CSP_BASE_LIB_NAME}" STREQUAL "")
        set(LIBS_TO_LINK "ags_cs::csp_base_lib;0.1.0")
    else()
        set(LIBS_TO_LINK ";${CUSTOM_CSP_BASE_LIB_NAME};${CUSTOM_CSP_BASE_ALIAS}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}"
        ${PROJECT_VERSION}
        "${LIB_HEADERS}"
        "${LIBS_TO_LINK}"
        ON
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}"
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (BUILD_UNIT_TESTS)
        set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
        set(TESTS_SOURCES
           "${TESTS_SOURCES_DIR}/MultiTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;gmock_main;${LIB_NAME}")

        if (NOT TARGET ags_cs_tests_csp_with_std_interface_included)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/serializable_structs_libs/tests_csp_with_std_interface")
        endif()

        if ("${CUSTOM_CSP_BASE_ALIAS}" STREQUAL "")
            ags_cs_create_tests_csp_with_std_interface_lib("" "")
        else()
            ags_cs_create_tests_csp_with_std_interface_lib(${LIB_NAME} "${CUSTOM_CSP_BASE_ALIAS}")
        endif()

        list(APPEND TESTS_LIBS_TO_LINK "${QUALIFIED_TEST_LIB_NAME}")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
