project(common_serialization_allocation_manager_interfaces)

add_library(common_serialization_allocation_manager_interfaces_lib INTERFACE)
add_library(common_serialization::allocation_manager_interfaceslib ALIAS common_serialization_allocation_manager_interfaces_lib)

set(ALLOCATION_MANAGER_INTERFACES_INCLUDE_DIR "include/common_serialization/allocation_manager_interfaces")
set(ALLOCATION_MANAGER_INTERFACES_HEADERS
    "${ALLOCATION_MANAGER_INTERFACES_INCLUDE_DIR}/allocation_manager_interface.h"
    "${ALLOCATION_MANAGER_INTERFACES_INCLUDE_DIR}/IAllocationManager.h"
    "${ALLOCATION_MANAGER_INTERFACES_INCLUDE_DIR}/IAllocationStrategyUser.h"
)

target_sources(common_serialization_allocation_manager_interfaces_lib
    INTERFACE
        ${ALLOCATION_MANAGER_INTERFACES_HEADERS}
)

target_include_directories(common_serialization_allocation_manager_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_allocation_manager_interfaces_lib
    INTERFACE
        common_serialization::allocator_interfaceslib
)
