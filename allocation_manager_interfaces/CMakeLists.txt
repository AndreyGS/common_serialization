cmake_minimum_required(VERSION 3.28)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

function(ags_cs_create_allocation_manager_interfaces_lib
    EXPORT_AND_INSTALL_LIB
)
    project(ags_cs_allocation_manager_interfaces VERSION 0.1.0)

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/allocation_manager_interfaces")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/allocation_manager_interface.h"
        "${LIB_HEADERS_DIR}/IAllocationManager.h"
        "${LIB_HEADERS_DIR}/IAllocationStrategyUser.h"
    )

    set(LIBS_TO_LINK "ags_cs::allocator_interfaces_lib;0.1.0")

    include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../cmake/helper_functions.cmake")

    ags_cs_add_interface_lib(allocation_manager_interfaces_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" ON "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" ${EXPORT_AND_INSTALL_LIB})

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)
endfunction()
