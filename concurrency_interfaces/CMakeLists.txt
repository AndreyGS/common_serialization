project(common_serialization_concurrency_interfaces)

add_library(common_serialization_concurrency_interfaces_lib INTERFACE)
add_library(common_serialization::concurrency_interfaceslib ALIAS common_serialization_concurrency_interfaces_lib)

set(CONCURRENCY_INTERFACES_INCLUDE_DIR "include/common_serialization/concurrency_interfaces")
set(CONCURRENCY_INTERFACES_HEADERS
    "${CONCURRENCY_INTERFACES_INCLUDE_DIR}/concurrency_interfaces.h"
    "${CONCURRENCY_INTERFACES_INCLUDE_DIR}/GuardRW.h"
    "${CONCURRENCY_INTERFACES_INCLUDE_DIR}/ILockable.h"
)

target_sources(common_serialization_concurrency_interfaces_lib
    INTERFACE
        ${CONCURRENCY_INTERFACES_HEADERS}
)

target_include_directories(common_serialization_concurrency_interfaces_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
