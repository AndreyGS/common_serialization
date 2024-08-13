cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_conatainers VERSION 0.1.0)

set(SAVED_CS_CONTAINERS_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Saved ags_common_serialization_conatainers project folder")
set(SAVED_CS_CONTAINERS_PROJECT_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Saved ags_common_serialization_conatainers project version")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Creates allocators library optionally customized with third-party memory management library and/or memory management typedefs.
# If library/package is not customized (all given arguments are empty strings) it will be named 'ags_cs_containers_lib' ('ags_common_serialization::containers_lib')
# otherwise 'ags_cs_containers_lib_customized[_x_<custom_alias1>[_x_<custom_alias2>]...]' and respective qualified alias.
#
# Params:
#   CUSTOM_MEMORY_MANAGEMENT_LIB_NAME: name of custom memory management library that can be used in linking expressions
#       If this argument is not empty string, then all other arguments must be not empty strings.
#       If this argument is empty string, then CUSTOM_MEMORY_MANAGEMENT_ALIAS must be empty.
#   CUSTOM_MEMORY_MANAGEMENT_ALIAS: alias that will be used in custom allocators library/package naming, it doesn't have to matter to the linker.
#       If this argument is not empty string, then all other arguments must be not empty strings.
#       If this argument is empty string, then CUSTOM_MEMORY_MANAGEMENT_LIB_NAME must be empty.
#       Note: it should contain only C-style naming characters.
#       Note: try make it shorter.
#   CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH: path to custom header. For example 'common_serialization/memory_management/typedefs2.h'.
#       If this argument is not empty string, then CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_ALIAS must be not empty string.
#       If this argument is empty string, then CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_ALIAS must be empty.
#       Note: path must be without quotes or double quotes on the edges.
#   CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_ALIAS: alias that will be used in custom allocators library/package naming, it doesn't have any other purpose.
#       If this argument is not empty string, then CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH must be not empty string.
#       If this argument is empty string, then CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH must be empty.
#       Note: try make it shorter.
#
# Returns:
#   CREATED_LIB_NAME name of created lib with namespace.
function(ags_cs_create_containers_lib 
    CUSTOM_MEMORY_MANAGEMENT_LIB_NAME 
    CUSTOM_MEMORY_MANAGEMENT_ALIAS 
    CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH 
    CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_ALIAS
)
    
    set(UNQUALIFIED_LIB_NAME "allocators_lib")
    update_lib_name_with_customized_names(
        "${UNQUALIFIED_LIB_NAME}"
        "${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}"
        "${CUSTOM_MEMORY_MANAGEMENT_ALIAS}"
        "${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}"
        "${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_ALIAS}"
    )

    set(CREATED_LIB_NAME "ags_common_serialization::${UNQUALIFIED_LIB_NAME}")
    set(LIB_NAME "ags_cs_allocators_lib")

    if (TARGET ${LIB_NAME})
        message(WARNING "Target '${LIB_NAME}' already exists.")
        return()
    endif()

    add_library(${LIB_NAME} INTERFACE)
    add_library(${CREATED_LIB_NAME} ALIAS ${LIB_NAME})

    set(CREATED_LIB_NAME ${CREATED_LIB_NAME} PARENT_SCOPE)

    if (NOT "${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}" STREQUAL "")
        target_compile_definitions(${LIB_NAME}
            INTERFACE
                CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER=<${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}>
        )
    endif()

    set(SOURCE_INCLUDE_DIR "${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/include")

    set(HEADERS_DIR "${SOURCE_INCLUDE_DIR}/common_serialization/allocators")
    set(HEADERS
        "${HEADERS_DIR}/allocators.h"
        "${HEADERS_DIR}/ConstructorNoexceptAllocator.h"
        "${HEADERS_DIR}/RawKeeperAllocator.h"
        "${HEADERS_DIR}/RawNoexceptAllocator.h"
        "${HEADERS_DIR}/typedefs.h"
    )

    target_sources(${LIB_NAME}
        INTERFACE
            FILE_SET HEADERS
            BASE_DIRS "${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES
                ${HEADERS}
    )

    target_include_directories(${LIB_NAME}
        INTERFACE 
            $<BUILD_INTERFACE:${SOURCE_INCLUDE_DIR}>
    )

    set(LINKED_LIBRARIES
        ags_common_serialization::allocator_interfaces_lib
    )

    if ("${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}" STREQUAL "")
        list(APPEND LINKED_LIBRARIES ags_common_serialization::memory_management_lib)
    else()
        list(APPEND LINKED_LIBRARIES ${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME})
    endif()

    target_link_libraries(${LIB_NAME}
        INTERFACE
            ${LINKED_LIBRARIES}
    )

    export_and_install_lib("${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${LIB_NAME} ${SAVED_CS_ALLOCATORS_PROJECT_VERSION})

    if (TARGET gtest)
        enable_testing()

        set(TESTS_NAME "${LIB_NAME}_tests")

        add_executable(${TESTS_NAME})

        set(UNIT_TESTS_SOURCES_DIR "${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/unit_tests")
        set(UNIT_TESTS_SOURCES
            "${UNIT_TESTS_SOURCES_DIR}/RawNoexceptAllocatorTests.cpp"
            "${UNIT_TESTS_SOURCES_DIR}/RawKeeperAllocatorTests.cpp"
            "${UNIT_TESTS_SOURCES_DIR}/ConstructorNoexceptAllocatorTests.cpp"
        )

        target_sources(${TESTS_NAME}
            PRIVATE
                ${UNIT_TESTS_SOURCES}
        )

        if (NOT TARGET ags_common_serialization::tests_special_types_lib)
            add_subdirectory("${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/../unit_tests_helper_libs/tests_special_types")
        endif()

        target_compile_definitions(${TESTS_NAME} PRIVATE USER_MODE)
        target_link_libraries(${TESTS_NAME}
            PRIVATE
                ${LIB_NAME}
                ags_common_serialization::tests_special_types_lib
                gtest_main
        )

        include(GoogleTest)
        gtest_discover_tests(${TESTS_NAME})
    endif()

endfunction()

add_library(common_serialization_containers_lib INTERFACE)
add_library(common_serialization::containerslib ALIAS common_serialization_containers_lib)

set(CONTAINERS_INCLUDE_DIR "include/common_serialization/containers")
set(CONTAINERS_HEADERS
    "${CONTAINERS_INCLUDE_DIR}/CompressedPair.h"
    "${CONTAINERS_INCLUDE_DIR}/Concepts.h"
    "${CONTAINERS_INCLUDE_DIR}/containers.h"
    "${CONTAINERS_INCLUDE_DIR}/DefaultDeleter.h"
    "${CONTAINERS_INCLUDE_DIR}/GenericPointerKeeper.h"
    "${CONTAINERS_INCLUDE_DIR}/IteratorTagsDeclarations.h"
    "${CONTAINERS_INCLUDE_DIR}/typedefs.h"
    "${CONTAINERS_INCLUDE_DIR}/UniquePtr.h"
    "${CONTAINERS_INCLUDE_DIR}/Vector.h"
    "${CONTAINERS_INCLUDE_DIR}/Walker.h"
)

target_sources(common_serialization_containers_lib
    INTERFACE
        ${CONTAINERS_HEADERS}
)

target_include_directories(common_serialization_containers_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(CONTAINERS_LIBRARIES
    common_serialization::allocation_manager_interfaceslib
)

if (NOT CS_CUSTOM_ALLOCATION_MANAGERS)
    list(APPEND CONTAINERS_LIBRARIES common_serialization::allocation_managerslib)
endif()

target_link_libraries(common_serialization_containers_lib
    INTERFACE
        ${CONTAINERS_LIBRARIES}
)