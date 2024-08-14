cmake_minimum_required(VERSION 3.28)

project(common_serialization
    VERSION 0.1.0
    DESCRIPTION "Definition and implementation of Common Serialization Protocol."
    HOMEPAGE_URL "https://github.com/AndreyGS/common_serialization"
)

if (NOT DEFINED FORCE_INSTALL_GTESTS)
    set(INSTALL_GTEST OFF)
endif()

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

option(AGS_CS_BUILD_COMMON "Build ags common serialization common project" ON)

if (AGS_CS_BUILD_COMMON)
   add_subdirectory(common)
endif()

# Memory Management Interfaces

option(AGS_CS_BUILD_MEMORY_MANAGEMENT_INTERFACES "Build ags common serialization memory management interfaces project" ON)

if (AGS_CS_BUILD_MEMORY_MANAGEMENT_INTERFACES)
   add_subdirectory(memory_management_interfaces)
endif()

# Memory Management

option(AGS_CS_BUILD_MEMORY_MANAGEMENT "Build ags common serialization memory management project" ON)

if (AGS_CS_BUILD_MEMORY_MANAGEMENT)
    add_subdirectory(memory_management)
endif()

# Allocator Interfaces

option(AGS_CS_BUILD_ALLOCATOR_INTERFACES "Build ags common serialization allocator interfaces project" ON)

if (AGS_CS_BUILD_ALLOCATOR_INTERFACES)
    add_subdirectory(allocator_interfaces)
endif()

# Allocators

option(AGS_CS_BUILD_ALLOCATORS "Build ags common serialization allocators project" ON)

if (AGS_CS_BUILD_ALLOCATORS)
    add_subdirectory(allocators)
    ags_cs_create_allocators_lib("ON" "" "" "" "")
    ags_cs_create_allocators_lib("ON" "ags_common_serialization::memory_management_lib" "0.1.0" "common_serialization/memory_management/typedefs2.h" "mm")
    message(WARNING ${QUALIFIED_LIB_NAME})
endif()

# Allocation Manager Interfaces

option(AGS_CS_ALLOCATION_MANAGER_INTERFACES "Build ags common serialization allocation manager interfaces project" ON)

if (AGS_CS_ALLOCATION_MANAGER_INTERFACES)
    add_subdirectory(allocation_manager_interfaces)
endif()

# Allocation Managers

option(AGS_CS_ALLOCATION_MANAGERS "Build ags common serialization allocation managers project" ON)

if (AGS_CS_ALLOCATION_MANAGERS)
    add_subdirectory(allocation_managers)
    ags_cs_create_allocation_managers_lib("ON" "" "" "" "")
endif()

# Containers

option(AGS_CS_CONTAINERS "Build ags common serialization containers project" ON)

if (AGS_CS_ALLOCATION_MANAGERS)
    add_subdirectory(containers)
    ags_cs_create_containers_lib("ON" "" "" "" "")
endif()
