project(common_serialization_csp_base)

add_library(common_serialization_csp_base_lib INTERFACE)
add_library(common_serialization::csp_baselib ALIAS common_serialization_csp_base_lib)

set(CSP_BASE_INCLUDE_DIR "include/common_serialization/csp_base")
set(CSP_BASE_HEADERS
    "${CSP_BASE_INCLUDE_DIR}/Concepts.h"
    "${CSP_BASE_INCLUDE_DIR}/csp_base.h"
    "${CSP_BASE_INCLUDE_DIR}/DefaultDeleter.h"
    "${CSP_BASE_INCLUDE_DIR}/GenericPointerKeeper.h"
    "${CSP_BASE_INCLUDE_DIR}/IteratorTagsDeclarations.h"
    "${CSP_BASE_INCLUDE_DIR}/typedefs.h"
    "${CSP_BASE_INCLUDE_DIR}/UniquePtr.h"
    "${CSP_BASE_INCLUDE_DIR}/Vector.h"
    "${CSP_BASE_INCLUDE_DIR}/Walker.h"
    "${CSP_BASE_INCLUDE_DIR}/CompressedPair.h"
    "${CSP_BASE_INCLUDE_DIR}/Concepts.h"
    "${CSP_BASE_INCLUDE_DIR}/containers.h"
    "${CSP_BASE_INCLUDE_DIR}/DefaultDeleter.h"
    "${CSP_BASE_INCLUDE_DIR}/GenericPointerKeeper.h"
    "${CSP_BASE_INCLUDE_DIR}/IteratorTagsDeclarations.h"
    "${CSP_BASE_INCLUDE_DIR}/typedefs.h"
    "${CSP_BASE_INCLUDE_DIR}/UniquePtr.h"
    "${CSP_BASE_INCLUDE_DIR}/Vector.h"
    "${CSP_BASE_INCLUDE_DIR}/Walker.h"
    "${CSP_BASE_INCLUDE_DIR}/CompressedPair.h"
    "${CSP_BASE_INCLUDE_DIR}/Concepts.h"
    "${CSP_BASE_INCLUDE_DIR}/containers.h"
    "${CSP_BASE_INCLUDE_DIR}/DefaultDeleter.h"
    "${CSP_BASE_INCLUDE_DIR}/GenericPointerKeeper.h"
    "${CSP_BASE_INCLUDE_DIR}/IteratorTagsDeclarations.h"
    "${CSP_BASE_INCLUDE_DIR}/typedefs.h"
    "${CSP_BASE_INCLUDE_DIR}/UniquePtr.h"
    "${CSP_BASE_INCLUDE_DIR}/Vector.h"
    "${CSP_BASE_INCLUDE_DIR}/Walker.h"
)

target_sources(common_serialization_csp_base_lib
    INTERFACE
        ${CSP_BASE_INCLUDE_DIR}
)

target_include_directories(common_serialization_csp_base_lib
    INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(common_serialization_csp_base_lib
    INTERFACE
        common_serialization::commonlib
)
