project(common_serialization_allocation_managers)

add_library(common_serialization_allocation_managers_lib INTERFACE)
add_library(common_serialization::allocation_managerslib ALIAS common_serialization_allocation_managers_lib)

set(ALLOCATION_MANAGERS_INCLUDE_DIR "include/common_serialization/allocation_managers")
set(ALLOCATION_MANAGERS_HEADERS
    "${ALLOCATION_MANAGERS_INCLUDE_DIR}/allocation_managers.h"
    "${ALLOCATION_MANAGERS_INCLUDE_DIR}/allocation_managers_config.h"
    "${ALLOCATION_MANAGERS_INCLUDE_DIR}/GenericAllocationManager.h"
    "${ALLOCATION_MANAGERS_INCLUDE_DIR}/StrategicAllocationManager.h"
    "${ALLOCATION_MANAGERS_INCLUDE_DIR}/typedefs.h"
)

target_sources(common_serialization_allocation_managers_lib
    INTERFACE
        ${ALLOCATION_MANAGERS_HEADERS}
)

target_include_directories(common_serialization_allocation_managers_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(ALLOCATION_MANAGERS_LIBRARIES
    common_serialization::allocation_manager_interfaceslib
)

if (NOT CS_CUSTOM_ALLOCATORS)
    list(APPEND ALLOCATION_MANAGERS_LIBRARIES common_serialization::common_serialization_allocators_lib)
endif()

target_link_libraries(common_serialization_allocators_lib
    INTERFACE
        ${ALLOCATION_MANAGERS_LIBRARIES}
)