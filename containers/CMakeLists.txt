project(common_serialization_conatainers)

add_library(common_serialization_containers_lib INTERFACE)
add_library(common_serialization::containerslib ALIAS common_serialization_containers_lib)

set(CONTAINERS_INCLUDE_DIR "include/common_serialization/containers")
set(CONTAINERS_HEADERS
    "${CONTAINERS_INCLUDE_DIR}/CompressedPair.h"
    "${CONTAINERS_INCLUDE_DIR}/Concepts.h"
    "${CONTAINERS_INCLUDE_DIR}/containers.h"
    "${CONTAINERS_INCLUDE_DIR}/DefaultDeleter.h"
    "${CONTAINERS_INCLUDE_DIR}/GenericPointerKeeper.h"
    "${CONTAINERS_INCLUDE_DIR}/IteratorTagsDeclarations.h"
    "${CONTAINERS_INCLUDE_DIR}/typedefs.h"
    "${CONTAINERS_INCLUDE_DIR}/UniquePtr.h"
    "${CONTAINERS_INCLUDE_DIR}/Vector.h"
    "${CONTAINERS_INCLUDE_DIR}/Walker.h"
)

target_sources(common_serialization_containers_lib
    INTERFACE
        ${CONTAINERS_HEADERS}
)

target_include_directories(common_serialization_containers_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(CONTAINERS_LIBRARIES
    common_serialization::allocation_manager_interfaceslib
)

if (NOT CS_CUSTOM_ALLOCATION_MANAGERS)
    list(APPEND CONTAINERS_LIBRARIES common_serialization::allocation_managerslib)
endif()

target_link_libraries(common_serialization_containers_lib
    INTERFACE
        ${CONTAINERS_LIBRARIES}
)