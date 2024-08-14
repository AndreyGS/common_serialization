cmake_minimum_required(VERSION 3.28)

function(get_template_lib_config_file LIB_NAME)
    set(GENERIC_CONFIG_TEMPLATE_FILE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generic_libConfig.cmake.in")
    set(TEMP_CONFIG_TEMPLATE "${CMAKE_BINARY_DIR}/temp/cmake/${LIB_NAME}Config.cmake.in")

    file(READ "${GENERIC_CONFIG_TEMPLATE_FILE}" CONFIG_CONTENT)
    string(REPLACE "\${LIB_NAME}" "${LIB_NAME}" CONFIG_CONTENT "${CONFIG_CONTENT}")
    file(WRITE "${TEMP_CONFIG_TEMPLATE}" "${CONFIG_CONTENT}")

    set(TEMP_CONFIG_TEMPLATE ${TEMP_CONFIG_TEMPLATE} PARENT_SCOPE)
endfunction()

function(ags_cs_export_and_install_lib LIB_NAME LIB_VERSION)
    export(
        TARGETS ${LIB_NAME}
        NAMESPACE common_serialization::
        FILE ${CMAKE_BINARY_DIR}/cmake/common_serialization/${LIB_NAME}Targets.cmake
    )

    install(
        TARGETS ${LIB_NAME}
        EXPORT ${LIB_NAME}Targets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT ${LIB_NAME}Targets
        FILE ${LIB_NAME}Targets.cmake
        NAMESPACE common_serialization::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/${LIB_NAME}ConfigVersion.cmake
        VERSION ${LIB_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    get_template_lib_config_file(${LIB_NAME})

    configure_package_config_file(
        ${TEMP_CONFIG_TEMPLATE}
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/${LIB_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )

    file(REMOVE ${TEMP_CONFIG_TEMPLATE})

    install(FILES
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/${LIB_NAME}Config.cmake
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/${LIB_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )
endfunction()

function(ags_cs_update_lib_name_with_customized_names UNQUALIFIED_LIB_NAME CUSTOM_DEPENDENCY_LIB_NAME CUSTOM_TYPEDEFS_HEADER_PATH CUSTOM_DEPENDENCY_ALIAS)
    set(NOTE_CUSTOMIZED_LIB_NAME "Note: if there is more than one custom dependency, name of building library maybe already customized with another dependencies.")

    if (    NOT (    "${CUSTOM_DEPENDENCY_LIB_NAME}" STREQUAL "" AND     "${CUSTOM_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND     "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL "")
        AND NOT (NOT "${CUSTOM_DEPENDENCY_LIB_NAME}" STREQUAL "" AND NOT "${CUSTOM_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND NOT "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL "")
    )
        message(FATAL_ERROR "Custom library name, custom typedef's header path, custom alias must all together be an empty strings or not empty strings.
            Note: current values: building library: '${UNQUALIFIED_LIB_NAME}'; 
                custom dependency library: '${CUSTOM_DEPENDENCY_LIB_NAME}'; 
                custom typedef's header: '${CUSTOM_TYPEDEFS_HEADER_PATH}'
                custom dependency library alias: '${CUSTOM_DEPENDENCY_ALIAS}'.
            Note: if there is more than one custom dependency, name of building library maybe already customized with another dependencies."
        )
    endif()

    set(DELIMITER "_x_")

    if (NOT "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL "")
        string(APPEND UNQUALIFIED_LIB_NAME "${DELIMITER}${CUSTOM_DEPENDENCY_ALIAS}")
    endif()

    set(UNQUALIFIED_LIB_NAME ${UNQUALIFIED_LIB_NAME} PARENT_SCOPE)
endfunction()

macro(ags_cs_add_custom_typedef_header_paths)
    if (NOT "${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}" STREQUAL "")
        target_compile_definitions(${LIB_NAME}
            INTERFACE
                AGS_CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH=<${AGS_CS_CUSTOM_MEMORY_MANAGEMENT_TYPEDEFS_HEADER_PATH}>
        )
    endif()
    if (NOT "${AGS_CS_CUSTOM_ALLOCATORS_TYPEDEFS_HEADER_PATH}" STREQUAL "")
        target_compile_definitions(${LIB_NAME}
            INTERFACE
                AGS_CS_CUSTOM_ALLOCATORS_TYPEDEFS_HEADER_PATH=<${AGS_CS_CUSTOM_ALLOCATORS_TYPEDEFS_HEADER_PATH}>
        )
    endif()
    if (NOT "${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH}" STREQUAL "")
        target_compile_definitions(${LIB_NAME}
            INTERFACE
                AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH=<${AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH}>
        )
    endif()
    if (NOT "${AGS_CS_CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH}" STREQUAL "")
        target_compile_definitions(${LIB_NAME}
            INTERFACE
                AGS_CS_CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH=<${AGS_CS_CUSTOM_CONTAINERS_TYPEDEFS_HEADER_PATH}>
        )
    endif()
endmacro()

function(ags_cs_add_find_packages LIBS_TO_LINK)
    list(LENGTH LIBS_TO_LINK NUM_ELEMENTS)
    set(LIBS_TO_LINK_NAMES)

    set(I 0)
    math(EXPR NUM_PAIRS "${NUM_ELEMENTS} / 2")
    while (I LESS ${NUM_PAIRS})
        math(EXPR NAME_INDEX "2 * ${I}")
        math(EXPR VERSION_INDEX "2 * ${I} + 1")

        list(GET LIBS_TO_LINK ${NAME_INDEX} LINK_LIB_NAME)
        list(GET LIBS_TO_LINK ${VERSION_INDEX} LINK_LIB_VERSION)

        if (NOT TARGET "${LINK_LIB_NAME}")
            string(REPLACE "ags_common_serialization::" "ags_cs_" LINK_LIB_NAME "${LINK_LIB_NAME}")
            # need to test when LINK_LIB_VERSION is an empty_string
            find_package(${LINK_LIB_NAME} ${LINK_LIB_VERSION} REQUIRED)
        endif()

        list(APPEND LIBS_TO_LINK_NAMES "${LINK_LIB_NAME}")
        math(EXPR I "${I} + 1")
    endwhile()

    set(LIBS_TO_LINK_NAMES "${LIBS_TO_LINK_NAMES}" PARENT_SCOPE)
endfunction()

function(ags_cs_add_interface_lib UNQUALIFIED_LIB_NAME LIB_VERSION HEADERS LIBS_TO_LINK FIND_PACKAGES LIB_SOURCE_DIR EXPORT_AND_INSTALL_LIB)
    
    set(LIB_NAME "ags_cs_${UNQUALIFIED_LIB_NAME}")
    set(QUALIFIED_LIB_NAME "ags_common_serialization::${UNQUALIFIED_LIB_NAME}")

    set(LIB_NAME ${LIB_NAME} PARENT_SCOPE)
    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (TARGET ${LIB_NAME})
        message(WARNING "Target '${LIB_NAME}' already exists.")
        return()
    endif()

    if (FIND_PACKAGES)
        ags_cs_add_find_packages("${LIBS_TO_LINK}")
    endif()

    add_library(${LIB_NAME} INTERFACE)
    add_library(${QUALIFIED_LIB_NAME} ALIAS ${LIB_NAME})

    target_sources(${LIB_NAME}
        INTERFACE
            FILE_SET HEADERS
            BASE_DIRS "${LIB_SOURCE_DIR}/include"
            FILES
                ${HEADERS}
    )

    target_include_directories(${LIB_NAME}
        INTERFACE 
            $<BUILD_INTERFACE:${LIB_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    )

    target_compile_features(${LIB_NAME} INTERFACE cxx_std_20)
    target_compile_definitions(${LIB_NAME} INTERFACE _HAS_CXX20)

    ags_cs_add_custom_typedef_header_paths()

    if (NOT "${LIBS_TO_LINK_NAMES}" STREQUAL "")
        target_link_libraries(${LIB_NAME}
            INTERFACE
                ${LIBS_TO_LINK_NAMES}
        )
    endif()

    if (EXPORT_AND_INSTALL_LIB)
        ags_cs_export_and_install_lib(${LIB_NAME} ${LIB_VERSION})
    endif()

    set(LIB_NAME ${LIB_NAME} PARENT_SCOPE)
    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

endfunction()

function(ags_cs_add_unit_tests_to_lib LIB_NAME TESTS_SOURCES TESTS_ADDITIONAL_LIBS_TO_LINK LIB_SOURCE_DIR)
    if (TARGET gtest)
        set(TESTS_NAME "${LIB_NAME}_tests")

        if (TARGET ${TESTS_NAME})
            message(WARNING "Target '${TESTS_NAME}' already exists.")
            return()
        endif()

        enable_testing()

        add_executable(${TESTS_NAME})

        target_sources(${TESTS_NAME}
            PRIVATE
                ${TESTS_SOURCES}
        )

        if (NOT TARGET ags_common_serialization::tests_special_types_lib)
            add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../unit_tests_helper_libs/tests_special_types")
        endif()

        target_compile_definitions(${TESTS_NAME} PRIVATE USER_MODE)
        target_link_libraries(${TESTS_NAME}
            PRIVATE
                gtest_main    
                ${LIB_NAME}
                ${TESTS_ADDITIONAL_LIBS_TO_LINK}  
        )

        set_target_properties(${TESTS_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit-tests")

        include(GoogleTest)
        gtest_discover_tests(${TESTS_NAME})

        add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})
    endif()
endfunction()

function(ags_cs_add_static_lib UNQUALIFIED_LIB_NAME LIB_VERSION HEADERS LIB_SOURCES LIBS_TO_LINK FIND_LIBS LIB_SOURCE_DIR EXPORT_AND_INSTALL_LIB)

    set(LIB_NAME "ags_cs_${UNQUALIFIED_LIB_NAME}")
    set(QUALIFIED_LIB_NAME "ags_common_serialization::${UNQUALIFIED_LIB_NAME}")

    set(LIB_NAME ${LIB_NAME} PARENT_SCOPE)
    set(QUALIFIED_LIB_NAME ${QUALIFIED_LIB_NAME} PARENT_SCOPE)

    if (TARGET ${LIB_NAME})
        message(WARNING "Target '${LIB_NAME}' already exists.")
        return()
    endif()

    if (FIND_PACKAGES)
        ags_cs_add_find_packages("${LIBS_TO_LINK}")
    endif()

    add_library(${LIB_NAME} STATIC)
    add_library(${QUALIFIED_LIB_NAME} ALIAS ${LIB_NAME})

    target_sources(${LIB_NAME}
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS "${LIB_SOURCE_DIR}/include"
            FILES
                ${HEADERS}
        PRIVATE
            ${LIB_SOURCES}
    )

    target_include_directories(${LIB_NAME}
        PUBLIC 
            $<BUILD_INTERFACE:${LIB_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    )

    target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)
    target_compile_definitions(${LIB_NAME} PUBLIC _HAS_CXX20)

    ags_cs_add_custom_typedef_header_paths()

    if (NOT "${LIBS_TO_LINK_NAMES}" STREQUAL "")
        target_link_libraries(${LIB_NAME}
            PUBLIC
                ${LIBS_TO_LINK_NAMES}
        )
    endif()

    if (EXPORT_AND_INSTALL_LIB)
        ags_cs_export_and_install_lib(${LIB_NAME} ${LIB_VERSION})
    endif()

endfunction()
