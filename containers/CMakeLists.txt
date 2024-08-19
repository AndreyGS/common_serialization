cmake_minimum_required(VERSION 3.28)

# We need a mark, that we already added project ags_cs_allocators.
# Because this project is library factory and can be used from multiple places.
# Unfortunately cmake doesn't have direct way to do that (cached vars are caching until cache deleted and this is inconvenient).
add_library(ags_cs_containers_included INTERFACE)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_containers_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
    CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME 
    CUSTOM_ALLOCATOR_MANAGERS_LIB_VERSION
    CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH 
    CUSTOM_ALLOCATOR_MANAGERS_ALIAS 
)
    project(ags_cs_containers VERSION 0.1.0)

    set(UNQUALIFIED_LIB_NAME "containers")
    ags_cs_update_lib_name_with_customized_name(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME}"
        "${CUSTOM_ALLOCATOR_MANAGERS_ALIAS}"
    )

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/containers")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/concepts.h"
        "${LIB_HEADERS_DIR}/containers.h"
        "${LIB_HEADERS_DIR}/containers_config.h"
        "${LIB_HEADERS_DIR}/typedefs.h"
        "${LIB_HEADERS_DIR}/CompressedPair.h"
        "${LIB_HEADERS_DIR}/DefaultDeleter.h"
        "${LIB_HEADERS_DIR}/GenericPointerKeeper.h"
        "${LIB_HEADERS_DIR}/IteratorTagsDeclarations.h"
        "${LIB_HEADERS_DIR}/UniquePtr.h"
        "${LIB_HEADERS_DIR}/Vector.h"
        "${LIB_HEADERS_DIR}/Walker.h"
    )

    set(LIBS_TO_LINK "ags_cs::allocation_manager_interfaces_lib;0.1.0")
    if ("${CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME}" STREQUAL "")
        list(APPEND LIBS_TO_LINK "ags_cs::allocation_managers;0.1.0")
    else()
        list(APPEND LIBS_TO_LINK "${CUSTOM_ALLOCATOR_MANAGERS_LIB_NAME};${CUSTOM_ALLOCATOR_MANAGERS_LIB_VERSION}")
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
            "${TESTS_SOURCES_DIR}/UniquePtrTests.cpp"
            "${TESTS_SOURCES_DIR}/VectorTests.cpp"
            "${TESTS_SOURCES_DIR}/WalkerTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;${LIB_NAME};ags_cs::tests_special_types_lib")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
