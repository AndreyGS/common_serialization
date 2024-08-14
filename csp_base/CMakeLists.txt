cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_allocation_managers VERSION 0.1.0)

option(AGS_CS_CSP_BASE "Build ags common serialization csp base project" ON)

set(AGS_CS_SAVED_CSP_BASE_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Saved ags_common_serialization_csp_base project folder")
set(AGS_CS_SAVED_CSP_BASE_PROJECT_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Saved ags_common_serialization_csp_base project version")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_csp_base_lib 
    EXPORT_AND_INSTALL_LIB
    AGS_CS_CUSTOM_CONTAINERS_LIB_NAME 
    AGS_CS_CUSTOM_CONTAINERS_LIB_VERSION
    AGS_CS_CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH 
    AGS_CS_CUSTOM_CONTAINERS_ALIAS 
)
    set(UNQUALIFIED_LIB_NAME "csp_base")
    ags_cs_update_lib_name_with_customized_names(
        "${UNQUALIFIED_LIB_NAME}"
        "${AGS_CS_CUSTOM_CONTAINERS_LIB_NAME}"
        "${AGS_CS_CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH}"
        "${AGS_CS_CUSTOM_CONTAINERS_ALIAS}"
    )

    set(HEADERS_DIR "${AGS_CS_CSP_BASE}/include/common_serialization/csp_base")
    set(HEADERS
        "${HEADERS_DIR}/Concepts.h"
        "${HEADERS_DIR}/csp_base.h"
        "${HEADERS_DIR}/csp_base_config.h"
        "${HEADERS_DIR}/ISerializable.h"
        "${HEADERS_DIR}/Macros.h"
        "${HEADERS_DIR}/traits.h"
        "${HEADERS_DIR}/context/Common.h"
        "${HEADERS_DIR}/context/CommonFlags.h"
        "${HEADERS_DIR}/context/Data.h"
        "${HEADERS_DIR}/context/DataFlags.h"
        "${HEADERS_DIR}/context/Message.h"
        "${HEADERS_DIR}/mandatory_structs/processing/Deserialize.h"
        "${HEADERS_DIR}/mandatory_structs/processing/Serialize.h"
        "${HEADERS_DIR}/processing/Rw.h"
        "${HEADERS_DIR}/processing/common/ContextProcessor.h"
        "${HEADERS_DIR}/processing/data/BodyProcessor.h"
        "${HEADERS_DIR}/processing/data/ContextProcessor.h"
        "${HEADERS_DIR}/processing/data/TemplateProcessor.h"
        "${HEADERS_DIR}/processing/data/VersionConverter.h"
        "${HEADERS_DIR}/processing/status/BodyProcessor.h"
        "${HEADERS_DIR}/processing/status/ContextProcessor.h"
        "${HEADERS_DIR}/processing/status/Helpers.h"
    )

    set(LIBS_TO_LINK "ags_common_serialization::common_lib;0.1.0")
    if ("${AGS_CS_CUSTOM_CONTAINERS_LIB_NAME}" STREQUAL "")
        string(APPEND LIBS_TO_LINK ";ags_common_serialization::containers;0.1.0")
    else()
        string(APPEND LIBS_TO_LINK ";${AGS_CS_CUSTOM_CONTAINERS_LIB_NAME};${AGS_CS_CUSTOM_CONTAINERS_LIB_VERSION}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}"
        ${AGS_CS_SAVED_CSP_BASE_PROJECT_VERSION}
        "${HEADERS}"
        "${LIBS_TO_LINK}"
        "ON"
        "${AGS_CS_SAVED_CSP_BASE_CMAKE_CURRENT_SOURCE_DIR}"
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    set(TESTS_SOURCES_DIR "${AGS_CS_SAVED_CSP_BASE_CMAKE_CURRENT_SOURCE_DIR}/unit_tests")
    set(TESTS_SOURCES
        "${TESTS_SOURCES_DIR}/UniquePtrTests.cpp"
        "${TESTS_SOURCES_DIR}/VectorTests.cpp"
        "${TESTS_SOURCES_DIR}/WalkerTests.cpp"
    )

    set(TESTS_ADDITIONAL_LIBS_TO_LINK "ags_common_serialization::tests_special_types_lib")

    ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_ADDITIONAL_LIBS_TO_LINK}" "${AGS_CS_SAVED_CSP_BASE_CMAKE_CURRENT_SOURCE_DIR}")

endfunction()
