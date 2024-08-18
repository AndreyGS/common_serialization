cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_cs_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_cs_csp_messaging_included INTERFACE)

option(AGS_CS_BUILD_CSP_MESSAGING "Build ags common serialization csp messaging project" ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_csp_messaging_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
    CUSTOM_CSP_BASE_LIB_NAME 
    CUSTOM_CSP_BASE_LIB_VERSION
    CUSTOM_CSP_BASE_ALIAS 
    CUSTOM_CONCURRENCY_LIB_NAME 
    CUSTOM_CONCURRENCY_LIB_VERSION
    CUSTOM_CONCURRENCY_TYPEDEFS_HEADER_PATH 
    CUSTOM_CONCURRENCY_ALIAS 
)
    project(ags_cs_csp_messaging VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "csp_messaging_lib")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_CSP_BASE_LIB_NAME}"
        "${CUSTOM_CSP_BASE_ALIAS}"
    )
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_CONCURRENCY_LIB_NAME}"
        "${CUSTOM_CONCURRENCY_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/csp_messaging")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/csp_messaging.h"
        "${LIB_HEADERS_DIR}/csp_messaging_config.h"
        "${LIB_HEADERS_DIR}/Client.h"
        "${LIB_HEADERS_DIR}/GenericServerDataHandlerRegistrar.h"
        "${LIB_HEADERS_DIR}/IClientDataHandlerTraits.h"
        "${LIB_HEADERS_DIR}/IServerDataHandler.h"
        "${LIB_HEADERS_DIR}/IServerDataHandlerBase.h"
        "${LIB_HEADERS_DIR}/IServerDataHandlerRegistrar.h"
        "${LIB_HEADERS_DIR}/IServerDataHandlerTraits.h"
        "${LIB_HEADERS_DIR}/Server.h"
        "${LIB_HEADERS_DIR}/service_structs/service_structs.h"
        "${LIB_HEADERS_DIR}/service_structs/structs.h"
        "${LIB_HEADERS_DIR}/service_structs/Interface.h"
        "${LIB_HEADERS_DIR}/service_structs/csp_processing_data/BodyProcessor.h"
    )

    set(LIB_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src")
    set(LIB_SOURCES
        "${LIB_SOURCES_DIR}/service_structs/csp_processing_data/BodyProcessor.cpp"
    )

    if ("${CUSTOM_CSP_BASE_LIB_NAME}" STREQUAL "")
        set(LIBS_TO_LINK "ags_cs::csp_base_lib;0.1.0")
    else()
        set(LIBS_TO_LINK ";${CUSTOM_CSP_BASE_LIB_NAME};${CUSTOM_CSP_BASE_ALIAS}")
    endif()

    if ("${CUSTOM_CONCURRENCY_LIB_NAME}" STREQUAL "")
        list(APPEND LIBS_TO_LINK "ags_cs::concurrency_lib;0.1.0")
    else()
        list(APPEND LIBS_TO_LINK "${CUSTOM_CONCURRENCY_LIB_NAME};${CUSTOM_CSP_BASE_LIB_VERSION}")
    endif()

    ags_cs_add_static_lib(
        "${UNQUALIFIED_LIB_NAME}"
        ${PROJECT_VERSION}
        "${LIB_HEADERS}"
        "${LIB_SOURCES}"
        "${LIBS_TO_LINK}"
        ON
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}"
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (BUILD_UNIT_TESTS)
        set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
        set(TESTS_SOURCES
           "${TESTS_SOURCES_DIR}/ClientTests.cpp"
           "${TESTS_SOURCES_DIR}/Helpers.h"
           "${TESTS_SOURCES_DIR}/ServerTests.cpp"
           "${TESTS_SOURCES_DIR}/ToRefactorTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;gmock_main;${QUALIFIED_LIB_NAME}")

        # Calling ags_cs_create_tests_csp_interface_lib only for getting QUALIFIED_TEST_LIB_NAME of tests_csp_interface_lib
        if ("${CUSTOM_CONTAINERS_ALIAS}" STREQUAL "")
            ags_cs_create_tests_csp_interface_lib("" "")
        else()
            ags_cs_create_tests_csp_interface_lib(${QUALIFIED_TEST_LIB_NAME} "${CUSTOM_CONTAINERS_ALIAS}")
        endif()

        list(APPEND TESTS_LIBS_TO_LINK ${QUALIFIED_TEST_LIB_NAME})

        if (NOT TARGET ags_cs_tests_csp_descendant_interface_included)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/serializable_structs_libs/tests_csp_descendant_interface")
        endif()

        if ("${CUSTOM_CONTAINERS_ALIAS}" STREQUAL "")
            ags_cs_create_tests_csp_descendant_interface_lib("" "")
        else()
            ags_cs_create_tests_csp_descendant_interface_lib(${QUALIFIED_TEST_LIB_NAME} "${CUSTOM_CONTAINERS_ALIAS}")
        endif()

        list(APPEND TESTS_LIBS_TO_LINK "${QUALIFIED_TEST_LIB_NAME}")

        if (NOT TARGET ags_cs_tests_csp_another_interface_included)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/serializable_structs_libs/tests_csp_another_interface")
        endif()

        if ("${CUSTOM_CONTAINERS_ALIAS}" STREQUAL "")
            ags_cs_create_tests_csp_another_interface_lib("" "")
        else()
            ags_cs_create_tests_csp_another_interface_lib(${CUSTOM_CSP_BASE_LIB_NAME} "${CUSTOM_CSP_BASE_ALIAS}")
        endif()

        list(APPEND TESTS_LIBS_TO_LINK "${QUALIFIED_TEST_LIB_NAME}")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
