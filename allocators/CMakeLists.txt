cmake_minimum_required(VERSION 3.26)

project(common_serialization_allocators)

add_library(common_serialization_allocators_lib INTERFACE)
add_library(common_serialization::allocatorslib ALIAS common_serialization_allocators_lib)

set(ALLOCATORS_INCLUDE_DIR "include/common_serialization/allocators")
set(ALLOCATORS_HEADERS
    "${ALLOCATORS_INCLUDE_DIR}/allocators.h"
    "${ALLOCATORS_INCLUDE_DIR}/ConstructorNoexceptAllocator.h"
    "${ALLOCATORS_INCLUDE_DIR}/RawKeeperAllocator.h"
    "${ALLOCATORS_INCLUDE_DIR}/RawNoexceptAllocator.h"
    "${ALLOCATORS_INCLUDE_DIR}/typedefs.h"
)

target_sources(common_serialization_allocators_lib
    INTERFACE
        ${ALLOCATORS_HEADERS}
)

target_include_directories(common_serialization_allocators_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(ALLOCATORS_LIBRARIES
    common_serialization::allocator_interfaceslib
)

if (DEFINED COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT AND COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT)
    list(APPEND ALLOCATORS_LIBRARIES common_serialization::memory_managementlib)
elseif (DEFINED COMMON_SERIALIZATION_CUSTOM_MEMORY_MANAGEMENT_LIB AND COMMON_SERIALIZATION_CUSTOM_MEMORY_MANAGEMENT_LIB)
    list(APPEND ALLOCATORS_LIBRARIES ${COMMON_SERIALIZATION_CUSTOM_MEMORY_MANAGEMENT_LIB})
else()
    message(FATAL_ERROR "You must build common_serialization::memory_managementlib
        or build custom memory management library and define COMMON_SERIALIZATION_CUSTOM_MEMORY_MANAGEMENT_LIB 
        with name of custom library before building common_serialization::allocatorslib.")
endif()

target_link_libraries(common_serialization_allocators_lib
    INTERFACE
        ${ALLOCATORS_LIBRARIES}
)

if (TARGET gtest OR TARGET gmock)
    enable_testing()

    add_executable(common_serialization_allocators_tests PRIVATE)
    set_target_properties(common_serialization_allocators_tests PROPERTIES EXPORT_NAME allocators_tests)

    set(UNIT_TESTS_SOURCES_DIR "${PROJECT_SOURCE_DIR}/unit_tests")
    set(ALLOCATOR_TESTS_SOURCES
        "${UNIT_TESTS_SOURCES_DIR}/RawKeeperAllocatorTests.cpp"
        "${UNIT_TESTS_SOURCES_DIR}/RawNoexceptAllocatorTests.cpp"
        "${UNIT_TESTS_SOURCES_DIR}/ConstructorNoexceptAllocatorTests.cpp"
    )

    target_sources(common_serialization_allocators_tests
        PRIVATE
            ${ALLOCATOR_TESTS_SOURCES}
    )

    target_compile_definitions(common_serialization_allocators_tests PRIVATE USER_MODE)
    target_link_libraries(common_serialization_allocators_tests 
        PRIVATE
            common_serialization::allocatorslib
            common_serialization::unit_tests_helpers
            gtest_main
    )
endif()
