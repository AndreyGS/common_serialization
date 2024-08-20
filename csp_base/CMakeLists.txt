cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_cs_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_cs_csp_base_included INTERFACE)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_csp_base_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
    CUSTOM_CONTAINERS_LIB_NAME 
    CUSTOM_CONTAINERS_LIB_VERSION
    CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH 
    CUSTOM_CONTAINERS_ALIAS 
)
    project(ags_cs_csp_base VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "csp_base_lib")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_CONTAINERS_LIB_NAME}"
        "${CUSTOM_CONTAINERS_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/csp_base")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/csp_base.h"
        "${LIB_HEADERS_DIR}/csp_base_config.h"
        "${LIB_HEADERS_DIR}/traits.h"
        "${LIB_HEADERS_DIR}/types.h"
        "${LIB_HEADERS_DIR}/Interface.h"
        "${LIB_HEADERS_DIR}/ISerializable.h"
        "${LIB_HEADERS_DIR}/context/Common.h"
        "${LIB_HEADERS_DIR}/context/CommonFlags.h"
        "${LIB_HEADERS_DIR}/context/Data.h"
        "${LIB_HEADERS_DIR}/context/DataFlags.h"
        "${LIB_HEADERS_DIR}/context/Message.h"
        "${LIB_HEADERS_DIR}/processing/rw.h"
        "${LIB_HEADERS_DIR}/processing/common/ContextProcessor.h"
        "${LIB_HEADERS_DIR}/processing/data/BodyProcessor.h"
        "${LIB_HEADERS_DIR}/processing/data/ContextProcessor.h"
        "${LIB_HEADERS_DIR}/processing/data/TemplateProcessor.h"
        "${LIB_HEADERS_DIR}/processing/data/VersionConverter.h"
        "${LIB_HEADERS_DIR}/processing/status/BodyProcessor.h"
        "${LIB_HEADERS_DIR}/processing/status/ContextProcessor.h"
        "${LIB_HEADERS_DIR}/processing/status/Helpers.h"
    )

    set(LIBS_TO_LINK "ags_cs::common_lib;0.1.0")
    if ("${CUSTOM_CONTAINERS_LIB_NAME}" STREQUAL "")
        string(APPEND LIBS_TO_LINK ";ags_cs::containers;0.1.0")
    else()
        string(APPEND LIBS_TO_LINK ";${CUSTOM_CONTAINERS_LIB_NAME};${CUSTOM_CONTAINERS_LIB_VERSION}")
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
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/AlignmentMayBeNotEqualTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/AllowUnmanagedPointersTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/AnotherBitnessTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/AnotherEndiannessTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/BasicModeTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/CheckRecursivePointersTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/InterfaceVersionsNotMatchTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/MultiModesTests.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/ProfillingTests.cpp" # placed here temporary (for convenience)
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/SimplyAssignableTagsOptimizationsAreTurnedOff.cpp"
           "${TESTS_SOURCES_DIR}/processing/data/full_cycles/SizeOfPrimitivesMayBeNotEqualTests.cpp"
        )

        if (NOT TARGET ags_cs_tests_restricted_structs_included)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/serializable_structs_libs/tests_restricted_structs")
        endif()

        if ("${CUSTOM_CONTAINERS_ALIAS}" STREQUAL "")
            ags_cs_create_tests_restricted_structs_lib("" "")
        else()
            ags_cs_create_tests_restricted_structs_lib(${QUALIFIED_LIB_NAME} "${CUSTOM_CONTAINERS_ALIAS}")
        endif()

        if (NOT TARGET ags_cs_tests_csp_interface_included)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/serializable_structs_libs/tests_csp_interface")
        endif()

        if ("${CUSTOM_CONTAINERS_ALIAS}" STREQUAL "")
            ags_cs_create_tests_csp_interface_lib("" "")
        else()
            ags_cs_create_tests_csp_interface_lib(${QUALIFIED_TEST_LIB_NAME} "${CUSTOM_CONTAINERS_ALIAS}")
        endif()

        set(TESTS_LIBS_TO_LINK "gtest_main;${LIB_NAME};${QUALIFIED_TEST_LIB_NAME}")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
