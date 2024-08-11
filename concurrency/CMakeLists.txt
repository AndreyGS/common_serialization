project(common_serialization_concurrency)

add_library(common_serialization_concurrency_lib INTERFACE)
add_library(common_serialization::concurrencylib ALIAS common_serialization_concurrency_lib)

set(CONCURRENCY_INCLUDE_DIR "include/common_serialization/concurrency")
set(CONCURRENCY_HEADERS
    "${CONCURRENCY_INCLUDE_DIR}/Concepts.h"
    "${CONCURRENCY_INCLUDE_DIR}/typedefs.h"
    "${CONCURRENCY_INCLUDE_DIR}/platform_dependent/switch.h"
    "${CONCURRENCY_INCLUDE_DIR}/platform_dependent/user_mode.h"
)

target_sources(common_serialization_concurrency_lib
    INTERFACE
        ${CONCURRENCY_HEADERS}
)

target_include_directories(common_serialization_concurrency_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_concurrency_lib
    INTERFACE
        common_serialization::commonlib
        common_serialization::concurrency_interfaceslib
)
