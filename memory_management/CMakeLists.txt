project(common_serialization_memory_management)

option(COMMON_SERIALIZATION_BUILD_MEMORY_MANAGEMENT "Build memory management project" ON)

add_library(common_serialization_memory_management_lib INTERFACE)
add_library(common_serialization::memory_managementlib ALIAS common_serialization_memory_management_lib)

set(MEMORY_MANAGEMENT_INCLUDE_DIR "include/common_serialization/memory_management")
set(MEMORY_MANAGEMENT_HEADERS
    "${MEMORY_MANAGEMENT_INCLUDE_DIR}/IStorageAllocator.h"
    "${MEMORY_MANAGEMENT_INCLUDE_DIR}/memory_management_interfaces.h"
)

target_sources(common_serialization_memory_management_lib
    INTERFACE
        ${MEMORY_MANAGEMENT_HEADERS}
)

target_include_directories(common_serialization_memory_management_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_memory_management_lib
    INTERFACE
        common_serialization::commonlib
        common_serialization::memory_management_interfaceslib
)
