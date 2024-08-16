cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_cs_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_cs_csp_restricted_structs_processing_included INTERFACE)

option(AGS_CS_BUILD_CSP_RESTRICTED_STRUCTS_PROCESSING "Build ags common serialization csp base project" ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_csp_restricted_structs_processing_lib 
    EXPORT_AND_INSTALL_LIB
    CUSTOM_CSP_BASE_LIB_NAME 
    CUSTOM_CSP_BASE_LIB_VERSION
    CUSTOM_CSP_BASE_TYPEDEFS_HEADER_PATH 
    CUSTOM_CSP_BASE_ALIAS 
)
    project(ags_cs_csp_restricted_structs_processing VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "csp_base")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_CONTAINERS_LIB_NAME}"
        "${CUSTOM_CONTAINERS_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CSP_BASE}/include/common_serialization/csp_base")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/Concepts.h"
        "${LIB_HEADERS_DIR}/csp_base.h"
        "${LIB_HEADERS_DIR}/csp_base_config.h"
        "${LIB_HEADERS_DIR}/ISerializable.h"
        "${LIB_HEADERS_DIR}/Macros.h"
        "${LIB_HEADERS_DIR}/traits.h"
        "${LIB_HEADERS_DIR}/context/Common.h"
        "${LIB_HEADERS_DIR}/context/CommonFlags.h"
        "${LIB_HEADERS_DIR}/context/Data.h"
        "${LIB_HEADERS_DIR}/context/DataFlags.h"
        "${LIB_HEADERS_DIR}/context/Message.h"
        "${LIB_HEADERS_DIR}/mandatory_structs/processing/Deserialize.h"
        "${LIB_HEADERS_DIR}/mandatory_structs/processing/Serialize.h"
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
        "ON"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}"
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
    set(TESTS_SOURCES
        "${TESTS_SOURCES_DIR}/UniquePtrTests.cpp"
        "${TESTS_SOURCES_DIR}/VectorTests.cpp"
        "${TESTS_SOURCES_DIR}/WalkerTests.cpp"
    )

    set(TESTS_ADDITIONAL_LIBS_TO_LINK "ags_cs::tests_special_types_lib")

    ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_ADDITIONAL_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")

endfunction()
