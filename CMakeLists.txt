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

if (COMMON_SERIALIZATION_BUILD_ALLOCATOR_INTERFACES)
    add_subdirectory(allocator_interfaces)
endif()

# Allocation Manager Interfaces

add_subdirectory(allocation_manager_interfaces)

# Allocation Managers

option(CS_CUSTOM_ALLOCATION_MANAGERS "Use custom Allocation Managers" OFF)
option(CS_CUSTOM_ALLOCATION_MANAGERS_TYPEDEFS "Use custom Allocation Managers typedefs" OFF)

if (CS_CUSTOM_ALLOCATION_MANAGERS AND NOT DEFINED CS_CUSTOM_ALLOCATION_MANAGERS_TYPEDEFS)
    add_compile_definitions(CS_CUSTOM_ALLOCATION_MANAGERS_TYPEDEFS)
endif()

if (NOT CS_CUSTOM_ALLOCATION_MANAGERS)
    add_subdirectory(allocation_managers)
endif()

# Containers

option(CS_CUSTOM_CONTAINERS "Use custom Containers" OFF)
option(CS_CUSTOM_CONTAINERS_TYPEDEFS "Use custom Containers typedefs" OFF)

if (CS_CUSTOM_CONTAINERS AND NOT DEFINED CS_CUSTOM_CONTAINERS_TYPEDEFS)
    add_compile_definitions(CS_CUSTOM_CONTAINERS_TYPEDEFS)
endif()

if (NOT CS_CUSTOM_CONTAINERS)
    add_subdirectory(containers)
endif()

# Concurrency Interfaces

add_subdirectory(concurrency_interfaces)

# Concurrency

option(CS_CUSTOM_CONCURRENCY "Use custom Concurrency types" OFF)
option(CS_CUSTOM_CONCURENCY_TYPEDEFS "Use custom Concurrency typedefs" OFF)

if (CS_CUSTOM_CONCURRENCY AND NOT DEFINED CS_CUSTOM_CONCURENCY_TYPEDEFS)
    add_compile_definitions(CS_CUSTOM_CONCURENCY_TYPEDEFS)
endif()

if (NOT CS_CUSTOM_CONCURRENCY)
    add_subdirectory(concurrency)
endif()

# Csp Base

option(CS_CSP_BASE "Common Serialization Protocol base definition" ON)

if (CS_CSP_BASE)
    add_subdirectory(csp_base)
endif()

# Csp restricted structs processing

option(CS_CSP_RESTRICTED_STRUCTS_PROCESSING "Processing of restricted types with Common Serialization Protocol" ON)

if (CS_CSP_RESTRICTED_STRUCTS_PROCESSING)
    add_subdirectory(csp_restricted_structs_processing)
endif()

# Csp Messaging

option(CS_CSP_MESSAGING "Common Serialization Protocol Messaging definition" ON)

if (CS_CSP_MESSAGING)
    add_subdirectory(csp_messaging)
endif()
