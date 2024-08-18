cmake_minimum_required(VERSION 3.28)

project(common_serialization
    VERSION 0.1.0
    DESCRIPTION "Definition and implementation of Common Serialization Protocol."
    HOMEPAGE_URL "https://github.com/AndreyGS/common_serialization"
)

if (NOT DEFINED FORCE_INSTALL_GTESTS)
    set(INSTALL_GTEST OFF)
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
    if (NOT TARGET ags_cs_allocators_included)
        add_subdirectory(allocators)
    endif()

    ags_cs_create_allocators_lib(ON ON "" "" "" "")
    ags_cs_create_allocators_lib(ON ON ags_cs::memory_management_lib 0.1.0 common_serialization/memory_management/typedefs2.h mm)
    message(WARNING ${QUALIFIED_LIB_NAME})
endif()

# Allocation Manager Interfaces

option(AGS_CS_BUILD_ALLOCATION_MANAGER_INTERFACES "Build ags common serialization allocation manager interfaces project" ON)

if (AGS_CS_BUILD_ALLOCATION_MANAGER_INTERFACES)
    add_subdirectory(allocation_manager_interfaces)
endif()

# Allocation Managers

option(AGS_CS_BUILD_ALLOCATION_MANAGERS "Build ags common serialization allocation managers project" ON)

if (AGS_CS_BUILD_ALLOCATION_MANAGERS)
    if (NOT TARGET ags_cs_allocation_managers_included)
        add_subdirectory(allocation_managers)
    endif()

    ags_cs_create_allocation_managers_lib(ON ON "" "" "" "")
endif()

# Containers

option(AGS_CS_BUILD_CONTAINERS "Build ags common serialization containers project" ON)

if (AGS_CS_BUILD_CONTAINERS)
    if (NOT TARGET ags_cs_containers_included)
        add_subdirectory(containers)
    endif()

    ags_cs_create_containers_lib(ON ON "" "" "" "")
endif()

# CSP base

option(AGS_CS_BUILD_CSP_BASE "Build ags common serialization csp base project" ON)

if (AGS_CS_BUILD_CSP_BASE)
    if (NOT TARGET ags_cs_csp_base_included)
        add_subdirectory(csp_base)
    endif()

    ags_cs_create_csp_base_lib(ON ON "" "" "" "")
endif()

# Concurrency

option(AGS_CS_BUILD_CONCURRENCY_INTERFACES "Build ags common serialization concurrency interfaces" ON)

if (AGS_CS_BUILD_CONCURRENCY_INTERFACES)
    add_subdirectory(concurrency_interfaces)
endif()

# Concurrency Interfaces

option(AGS_CS_BUILD_CONCURRENCY "Build ags common serialization concurrency" ON)

if (AGS_CS_BUILD_CONCURRENCY)
    add_subdirectory(concurrency)
endif()

# CSP Messaging

option(AGS_CS_BUILD_CSP_MESSAGING "Build ags common serialization csp messaging project" ON)

if (AGS_CS_BUILD_CSP_MESSAGING)
    if (NOT TARGET ags_cs_csp_messaging_included)
        add_subdirectory(csp_messaging)
    endif()

    ags_cs_create_csp_messaging_lib(ON ON "" "" "" "" "" "" "")
endif()
