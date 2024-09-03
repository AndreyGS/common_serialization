cmake_minimum_required(VERSION 3.28)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/helper_functions.cmake")

function(ags_cs_create_common_lib 
    EXPORT_AND_INSTALL_LIB
    BUILD_UNIT_TESTS
)
    project(ags_cs_common VERSION 0.1.0)

    set(LIB_HEADERS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/common_serialization/common")
    set(LIB_HEADERS
        "${LIB_HEADERS_DIR}/common.h"
        "${LIB_HEADERS_DIR}/concepts.h"
        "${LIB_HEADERS_DIR}/helpers.h"
        "${LIB_HEADERS_DIR}/std_equivalents.h"
        "${LIB_HEADERS_DIR}/traits.h"
        "${LIB_HEADERS_DIR}/Status.h"
        "${LIB_HEADERS_DIR}/Uuid.h"
        "${LIB_HEADERS_DIR}/interfaces/IIoProcessor.h"
    )

    ags_cs_add_interface_lib(common_lib ${PROJECT_VERSION} "${LIB_HEADERS}" "" "" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" ${EXPORT_AND_INSTALL_LIB})

    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (AGS_CS_BUILD_UNIT_TESTS)
        set(TESTS_SOURCES_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/unit_tests")
        set(TESTS_SOURCES
            "${TESTS_SOURCES_DIR}/HelpersTests.cpp"
            "${TESTS_SOURCES_DIR}/StatusTests.cpp"
            "${TESTS_SOURCES_DIR}/UuidTests.cpp"
        )

        set(TESTS_LIBS_TO_LINK "gtest_main;${LIB_NAME};ags_cs::tests_special_types_lib")

        ags_cs_add_unit_tests_to_lib(${LIB_NAME} "${TESTS_SOURCES}" "${TESTS_LIBS_TO_LINK}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()

endfunction()
