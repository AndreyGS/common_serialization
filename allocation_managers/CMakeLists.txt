cmake_minimum_required(VERSION 3.28)

project(ags_common_serialization_allocation_managers VERSION 0.1.0)

option(AGS_CS_ALLOCATION_MANAGERS "Build ags common serialization allocation managers project" ON)

set(AGS_CS_SAVED_ALLOCATION_MANAGERS_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Saved ags_common_serialization_allocation_managers project folder")
set(AGS_CS_SAVED_ALLOCATION_MANAGERS_PROJECT_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Saved ags_common_serialization_allocation_managers project version")

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

# Function description is similar to ags_cs_create_allocators_lib()
function(ags_cs_create_allocation_managers_lib 
    EXPORT_AND_INSTALL_LIB
    AGS_CS_CUSTOM_ALLOCATORS_LIB_NAME 
    AGS_CS_CUSTOM_ALLOCATORS_LIB_VERSION
    AGS_CS_CUSTOM_ALLOCATORS_TYPEDEFS_HEADER_PATH 
    AGS_CS_CUSTOM_ALLOCATORS_ALIAS 
)
    set(UNQUALIFIED_LIB_NAME "allocation_managers")
    ags_cs_update_lib_name_with_customized_names(
        "${UNQUALIFIED_LIB_NAME}"
        "${AGS_CS_CUSTOM_ALLOCATORS_LIB_NAME}"
        "${AGS_CS_CUSTOM_ALLOCATORS_TYPEDEFS_HEADER_PATH}"
        "${AGS_CS_CUSTOM_ALLOCATORS_ALIAS}"
    )

    set(HEADERS_DIR "${AGS_CS_SAVED_ALLOCATION_MANAGERS_CMAKE_CURRENT_SOURCE_DIR}/include/common_serialization/allocation_managers")
    set(HEADERS
        "${HEADERS_DIR}/allocation_managers.h"
        "${HEADERS_DIR}/allocation_managers_config.h"
        "${HEADERS_DIR}/GenericAllocationManager.h"
        "${HEADERS_DIR}/StrategicAllocationManager.h"
        "${HEADERS_DIR}/typedefs.h"
    )

    set(LIBS_TO_LINK "ags_common_serialization::allocation_manager_interfaces_lib;0.1.0")
    if ("${AGS_CS_CUSTOM_ALLOCATORS_LIB_NAME}" STREQUAL "")
        string(APPEND LIBS_TO_LINK ";ags_common_serialization::allocators_lib;0.1.0")
    else()
        string(APPEND LIBS_TO_LINK ";${AGS_CS_CUSTOM_ALLOCATORS_LIB_NAME};${AGS_CS_CUSTOM_ALLOCATORS_LIB_VERSION}")
    endif()

    ags_cs_add_interface_lib(
        "${UNQUALIFIED_LIB_NAME}" 
        ${AGS_CS_SAVED_ALLOCATION_MANAGERS_PROJECT_VERSION} 
        "${HEADERS}" 
        "${LIBS_TO_LINK}"
        "ON"
        "${AGS_CS_SAVED_ALLOCATION_MANAGERS_CMAKE_CURRENT_SOURCE_DIR}" 
        ${EXPORT_AND_INSTALL_LIB}
    )

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

endfunction()
