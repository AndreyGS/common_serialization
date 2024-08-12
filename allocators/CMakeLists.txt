cmake_minimum_required(VERSION 3.26)

project(cs_allocators)

set(SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Saved cs::allocators_lib source folder")

function(create_cs_allocators_lib CREATED_LIB_NAME CUSTOM_MEMORY_MANAGEMENT_LIB_NAME)

    if ("${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME}" STREQUAL "")
        set(LIB_NAME "cs_allocators_lib")
        set(${CREATED_LIB_NAME} "cs::allocators_lib" PARENT_SCOPE)
    else()
        string(REGEX REPLACE "[^a-zA-Z0-9]" "\_" CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME ${CUSTOM_MEMORY_MANAGEMENT_LIB_NAME})
        set(LIB_NAME "cs_allocators_lib_customized_with_${CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME}")
        set(${CREATED_LIB_NAME} "cs::allocators_lib_customized_with_${CUSTOM_MEMORY_MANAGEMENT_CORRECTED_LIB_NAME}" PARENT_SCOPE)
    endif()

    if (TARGET ${LIB_NAME})
        message(WARNING "Target '${LIB_NAME}' already exists.")
        return()
    endif()

    add_library(${LIB_NAME} INTERFACE)
    add_library(${CREATED_LIB_NAME} ALIAS ${LIB_NAME})

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

    set(EXPORT_TARGETS_LIST ${LIB_NAME})

    if (TARGET gtest)
        enable_testing()

        set(TESTS_NAME "${LIB_NAME}_tests")

        add_executable(${TESTS_NAME})

        set(UNIT_TESTS_SOURCES_DIR "${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/unit_tests")
        set(UNIT_TESTS_SOURCES
            "${UNIT_TESTS_SOURCES_DIR}/RawNoexceptAllocatorTests.cpp"
            "${UNIT_TESTS_SOURCES_DIR}/ConstructorNoexceptAllocatorTests.cpp"
        )

        target_sources(${TESTS_NAME}
            PRIVATE
                ${UNIT_TESTS_SOURCES}
        )

        if (NOT TARGET cs::tests_special_types_lib)
            add_subdirectory(${SAVED_CS_ALLOCATORS_CMAKE_CURRENT_SOURCE_DIR}/../unit_tests_helper_libs/cs_tests_special_types)
        endif()

        target_compile_definitions(${TESTS_NAME} PRIVATE USER_MODE)
        target_link_libraries(${TESTS_NAME}
            PRIVATE
                ${LIB_NAME}
                cs::tests_special_types_lib
                gtest_main
        )

        list(APPEND EXPORT_TARGETS_LIST ${TESTS_NAME})

        include(GoogleTest)
        gtest_discover_tests(${TESTS_NAME})
    endif()

    #export(TARGETS ${EXPORT_TARGETS_LIST} FILE "${CMAKE_BINARY_DIR}/${LIB_NAME}_targets.cmake")

endfunction()

