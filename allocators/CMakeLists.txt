cmake_minimum_required(VERSION 3.26)

project(ags_common_serialization_allocators VERSION 0.1.0)

set(SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Saved cs::allocators_lib source folder")
set(SAVED_CS_ALLOCATORS_PROJECT_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Saved cs::allocators_lib version")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Creates allocators library optionally customized with memory management library and/or third-party memery management typedefs
# If library is not customized it named cs::allocators_lib, otherwise cs::allocators_lib_customized_with_${<custom_names_list>}
# Returns name in CREATED_LIB_NAME variable
function(create_allocators_lib CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH TYPEDEFS_ALIAS CUSTOM_MEMORY_MANAGEMENT_LIB_NAME)

    if ("${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}" STREQUAL "")
        set(LIB_NAME "allocators_lib")
        set(CREATED_LIB_NAME "cs::allocators_lib")
    else()
        string(REGEX REPLACE "[^a-zA-Z0-9]" "\_" CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME ${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME})
        set(LIB_NAME "allocators_lib_customized_with_${CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME}")
        set(CREATED_LIB_NAME "cs::allocators_lib_customized_with_${CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME}")
    endif()

    if (       "${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND NOT "${TYPEDEFS_ALIAS}" STREQUAL ""
        OR NOT "${CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND     "${TYPEDEFS_ALIAS}" STREQUAL ""
    )
        message(FATAL_ERROR "Custom header and respective alias must be both set or not at the same time")
    elseif (NOT "${TYPEDEFS_ALIAS}" STREQUAL "")
        set(POSITION -1)
        string(FIND ${LIB_NAME} "customized" POSITION)
        if (POSITION EQUAL -1)
            string(APPEND LIB_NAME "_and_typedefs_${TYPEDEFS_ALIAS}")
            string(APPEND CREATED_LIB_NAME "_and_typedefs_${TYPEDEFS_ALIAS}")
            message(WARNING ${LIB_NAME})
        else()
            string(APPEND LIB_NAME "_customized_with_typedefs_${TYPEDEFS_ALIAS}")
            string(APPEND CREATED_LIB_NAME "_customized_with_typedefs_${TYPEDEFS_ALIAS}")
        endif()
    endif()

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
        cs::allocator_interfaces_lib
    )

    if ("${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}" STREQUAL "")
        list(APPEND LINKED_LIBRARIES cs::memory_management_lib)
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

        if (NOT TARGET cs::tests_special_types_lib)
            add_subdirectory("${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/../unit_tests_helper_libs/tests_special_types")
        endif()

        target_compile_definitions(${TESTS_NAME} PRIVATE USER_MODE)
        target_link_libraries(${TESTS_NAME}
            PRIVATE
                ${LIB_NAME}
                cs::tests_special_types_lib
                gtest_main
        )

        include(GoogleTest)
        gtest_discover_tests(${TESTS_NAME})
    endif()

endfunction()

