project(common_serialization_allocator_interfaces)

add_library(common_serialization_allocator_interfaces_lib INTERFACE)
add_library(common_serialization::allocator_interfaceslib ALIAS common_serialization_allocator_interfaces_lib)

set(ALLOCATOR_INTERFACES_INCLUDE_DIR "include/common_serialization/allocator_interfaces")
set(ALLOCATOR_INTERFACES_HEADERS
    "${ALLOCATOR_INTERFACES_INCLUDE_DIR}/allocator_interface.h"
    "${ALLOCATOR_INTERFACES_INCLUDE_DIR}/IAllocator.h"
    "${ALLOCATOR_INTERFACES_INCLUDE_DIR}/IAllocatorTraits.h"
    "${ALLOCATOR_INTERFACES_INCLUDE_DIR}/IStorageSetter.h"
)

target_sources(common_serialization_allocator_interfaces_lib
    INTERFACE
        ${ALLOCATOR_INTERFACES_HEADERS}
)

target_include_directories(common_serialization_allocator_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_allocator_interfaces_lib
    INTERFACE
        common_serialization::commonlib
)
