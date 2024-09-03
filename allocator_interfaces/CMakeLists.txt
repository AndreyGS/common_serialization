cmake_minimum_required(VERSION 3.28)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

function(ags_cs_create_allocator_interfaces_lib
    EXPORT_AND_INSTALL_LIB
)
    project(ags_cs_allocator_interfaces VERSION 0.1.0)

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/allocator_interfaces")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/allocator_interface.h"
        "${LIB_HEADERS_DIR}/IAllocator.h"
        "${LIB_HEADERS_DIR}/IAllocatorTraits.h"
        "${LIB_HEADERS_DIR}/IStorageSetter.h"
    )

    set(LIBS_TO_LINK "ags_cs::common_lib;0.1.0")

    ags_cs_add_interface_lib(allocator_interfaces_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" ON "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" ${EXPORT_AND_INSTALL_LIB})

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)
endfunction()
