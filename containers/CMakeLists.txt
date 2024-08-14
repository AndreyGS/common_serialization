cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_allocation_managers VERSION 0.1.0)

option(AGS_CS_CONTAINERS "Build ags common serialization containers project" ON)

set(AGS_CS_SAVED_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Saved ags_common_serialization_containers project folder")
set(AGS_CS_SAVED_CONTAINERS_PROJECT_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Saved ags_common_serialization_containers project version")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_containers_lib 
    EXPORT_AND_INSTALL_LIB
    AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME 
    AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_VERSION
    AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH 
    AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_ALIAS 
)
    set(UNQUALIFIED_LIB_NAME "containers")
    ags_cs_update_lib_name_with_customized_names(
        "${UNQUALIFIED_LIB_NAME}"
        "${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME}"
        "${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH}"
        "${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_ALIAS}"
    )

    set(HEADERS_DIR "${AGS_CS_SAVED_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/containers")
    set(HEADERS
        "${HEADERS_DIR}/CompressedPair.h"
        "${HEADERS_DIR}/Concepts.h"
        "${HEADERS_DIR}/containers.h"
        "${HEADERS_DIR}/containers_config.h"
        "${HEADERS_DIR}/DefaultDeleter.h"
        "${HEADERS_DIR}/GenericPointerKeeper.h"
        "${HEADERS_DIR}/IteratorTagsDeclarations.h"
        "${HEADERS_DIR}/typedefs.h"
        "${HEADERS_DIR}/UniquePtr.h"
        "${HEADERS_DIR}/Vector.h"
        "${HEADERS_DIR}/Walker.h"
    )

    set(LIBS_TO_LINK "ags_common_serialization::allocation_manager_interfaces_lib;0.1.0")
    if ("${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME}" STREQUAL "")
        string(APPEND LIBS_TO_LINK ";ags_common_serialization::allocation_managers;0.1.0")
    else()
        string(APPEND LIBS_TO_LINK ";${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME};${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_LIB_VERSION}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}"
        ${AGS_CS_SAVED_CONTAINERS_PROJECT_VERSION}
        "${HEADERS}"
        "${LIBS_TO_LINK}"
        "ON"
        "${AGS_CS_SAVED_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR}"
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    set(TESTS_SOURCES_DIR "${AGS_CS_SAVED_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR}/unit_tests")
    set(TESTS_SOURCES
        "${TESTS_SOURCES_DIR}/UniquePtrTests.cpp"
        "${TESTS_SOURCES_DIR}/VectorTests.cpp"
        "${TESTS_SOURCES_DIR}/WalkerTests.cpp"
    )

    set(TESTS_ADDITIONAL_LIBS_TO_LINK "ags_common_serialization::tests_special_types_lib")

    ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_ADDITIONAL_LIBS_TO_LINK}" "${AGS_CS_SAVED_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR}")

endfunction()
