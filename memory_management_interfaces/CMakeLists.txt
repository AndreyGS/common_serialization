cmake_minimum_required(VERSION 3.26)

project(common_serialization_memory_management_interfaces)

add_library(common_serialization_memory_management_interfaces_lib INTERFACE)
add_library(common_serialization::memory_management_interfaceslib ALIAS common_serialization_memory_management_interfaces_lib)

set(MEMORY_MANAGEMENT_INTERFACES_INCLUDE_DIR "include/common_serialization/memory_management_interfaces")
set(MEMORY_MANAGEMENT_INTERFACES_HEADERS
    "${MEMORY_MANAGEMENT_INTERFACES_INCLUDE_DIR}/IStorageAllocator.h"
    "${MEMORY_MANAGEMENT_INTERFACES_INCLUDE_DIR}/memory_management_interfaces.h"
)

target_sources(common_serialization_memory_management_interfaces_lib
    INTERFACE
        ${MEMORY_MANAGEMENT_INTERFACES_HEADERS}
)

target_include_directories(common_serialization_memory_management_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_memory_management_interfaces_lib
    INTERFACE
        common_serialization::commonlib
)
