cmake_minimum_required(VERSION 3.28)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

function(ags_cs_create_memory_management_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
)
    project(ags_cs_memory_management VERSION 0.1.0)

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/memory_management")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/delete_replacements.h"
        "${LIB_HEADERS_DIR}/memory_management.h"
        "${LIB_HEADERS_DIR}/new_replacements.h"
        "${LIB_HEADERS_DIR}/typedefs.h"
        "${LIB_HEADERS_DIR}/platform_dependent/linux_kernel.h"
        "${LIB_HEADERS_DIR}/platform_dependent/switch.h"
        "${LIB_HEADERS_DIR}/platform_dependent/user_mode.h"
        "${LIB_HEADERS_DIR}/platform_dependent/windows_kernel.h"
    )

    set(LIBS_TO_LINK "ags_cs::memory_management_interfaces_lib;0.1.0")

    ags_cs_add_interface_lib(memory_management_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "${LIBS_TO_LINK}" ON "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" ON)

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (AGS_CS_BUILD_UNIT_TESTS)
        set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
        set(TESTS_SOURCES
            "${TESTS_SOURCES_DIR}/NewReplacementsTests.cpp"
            "${TESTS_SOURCES_DIR}/platform_dependent/HeapAllocatorTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;${LIB_NAME}")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
