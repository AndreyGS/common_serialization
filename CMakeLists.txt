cmake_minimum_required(VERSION 3.26)

project(common_serialization
    DESCRIPTION "Definition and implementation of Common Serialization Protocol."
    HOMEPAGE_URL "https://github.com/AndreyGS/common_serialization"
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/9fce5480448488e17a50bcbf88d2f3bdb637ad6c.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Common

option(COMMON_SERIALIZATION_BUILD_COMMON "Build common project" ON)

if (COMMON_SERIALIZATION_BUILD_COMMON)
   add_subdirectory(common)
endif()

# Memory Management Interfaces

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT_INTERFACES "Build memory management interfaces project" ON)

if (COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT_INTERFACES)
   add_subdirectory(memory_management_interfaces)
endif()

# Memory Management

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT "Build memory management project" ON)

if (COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT)
    add_subdirectory(memory_management)
endif()

# Allocator Interfaces

option(COMMON_SERIALIZATION_BUILD_ALLOCATOR_INTERFACES "Build allocator interfaces project" ON)

if (COMMON_SERIALIZATION_BUILD_ALLOCATOR_INTERFACES)
    add_subdirectory(allocator_interfaces)
endif()

# Allocators

option(COMMON_SERIALIZATION_BUILD_ALLOCATORS "Build allocators project" ON)

if (COMMON_SERIALIZATION_BUILD_ALLOCATORS)
    add_subdirectory(allocators)
    set(ALLOCATORS_LIB_NAME "")
    create_cs_allocators_lib(ALLOCATORS_LIB_NAME "")
endif()
