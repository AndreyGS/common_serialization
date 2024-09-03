cmake_minimum_required(VERSION 3.28)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

function(ags_cs_create_concurrency_lib
    EXPORT_AND_INSTALL_LIB
)
    project(ags_cs_concurrency VERSION 0.1.0)

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/concurrency")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/concurrency.h"
        "${LIB_HEADERS_DIR}/typedefs.h"
        "${LIB_HEADERS_DIR}/platform_dependent/switch.h"
        "${LIB_HEADERS_DIR}/platform_dependent/user_mode.h"
    )

    set(LIBS_TO_LINK "ags_cs::concurrency_interfaces_lib;0.1.0")

    ags_cs_add_interface_lib(concurrency_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" ON "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" ${EXPORT_AND_INSTALL_LIB})

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)
endfunction()
