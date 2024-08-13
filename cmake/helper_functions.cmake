cmake_minimum_required(VERSION 3.26)

function(get_template_lib_config_file COMMON_CMAKE_DIR LIB_NAME)
    set(GENERIC_CONFIG_TEMPLATE_FILE "${COMMON_CMAKE_DIR}/generic_libConfig.cmake.in")
    set(TEMP_CONFIG_TEMPLATE "${CMAKE_BINARY_DIR}/temp/cmake/${LIB_NAME}Config.cmake.in")

    file(READ "${GENERIC_CONFIG_TEMPLATE_FILE}" CONFIG_CONTENT)
    string(REPLACE "\${LIB_NAME}" "${LIB_NAME}" CONFIG_CONTENT "${CONFIG_CONTENT}")
    file(WRITE "${TEMP_CONFIG_TEMPLATE}" "${CONFIG_CONTENT}")

    set(TEMP_CONFIG_TEMPLATE ${TEMP_CONFIG_TEMPLATE} PARENT_SCOPE)
endfunction()

function(export_and_install_lib COMMON_CMAKE_DIR LIB_NAME LIB_VERSION)
    export(
        TARGETS ${LIB_NAME}
        NAMESPACE common_serialization::
        FILE ${CMAKE_BINARY_DIR}/cmake/common_serialization/ags_common_serialization_${LIB_NAME}Targets.cmake
    )

    install(
        TARGETS ${LIB_NAME}
        EXPORT ${LIB_NAME}Targets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT ${LIB_NAME}Targets
        FILE ags_common_serialization_${LIB_NAME}Targets.cmake
        NAMESPACE common_serialization::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/ags_common_serialization_${LIB_NAME}ConfigVersion.cmake
        VERSION ${LIB_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    get_template_lib_config_file("${COMMON_CMAKE_DIR}" "${LIB_NAME}")

    configure_package_config_file(
        ${TEMP_CONFIG_TEMPLATE}
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/ags_common_serialization_${LIB_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )

    file(REMOVE ${TEMP_CONFIG_TEMPLATE})

    install(FILES
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/ags_common_serialization_${LIB_NAME}Config.cmake
        ${CMAKE_BINARY_DIR}/cmake/common_serialization/ags_common_serialization_${LIB_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/common_serialization
    )
endfunction()

function(update_lib_name_with_customized_names LIB_NAME CUSTOM_DEPENDENCY_LIB_NAME CUSTOM_DEPENDENCY_ALIAS CUSTOM_TYPEDEFS_HEADER_PATH CUSTOM_TYPEDEFS_ALIAS)
    set(NOTE_CUSTOMIZED_LIB_NAME "Note: if there is more than one custom dependency name of building library maybe already customized with another dependencies.")

    if (       "${CUSTOM_DEPENDENCY_LIB_NAME}" STREQUAL "" AND NOT "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL ""
        OR NOT "${CUSTOM_DEPENDENCY_LIB_NAME}" STREQUAL "" AND     "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL ""
    )
        message(FATAL_ERROR "Custom dependency library and respective alias must be both set or not at the same time.\n
            Note: current values: building library: '${LIB_NAME}'; custom dependency library: '${CUSTOM_DEPENDENCY_LIB_NAME}'; custom dependency library alias: '${CUSTOM_DEPENDENCY_ALIAS}'.\n
            ${NOTE_CUSTOMIZED_LIB_NAME}")
    endif()

    if (       "${CUSTOM_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND NOT "${CUSTOM_TYPEDEFS_ALIAS}" STREQUAL ""
        OR NOT "${CUSTOM_TYPEDEFS_HEADER_PATH}" STREQUAL "" AND     "${CUSTOM_TYPEDEFS_ALIAS}" STREQUAL ""
    )
        message(FATAL_ERROR "Custom typedef's header and respective alias must be both set or not at the same time.\n
            Note: current values: building library: '${LIB_NAME}'; custom typedef's header: '${CUSTOM_TYPEDEFS_HEADER_PATH}'; custom typedef's alias: '${CUSTOM_TYPEDEFS_ALIAS}'\n
            ${NOTE_CUSTOMIZED_LIB_NAME}")
    endif()

    if (NOT "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL "" AND "${CUSTOM_TYPEDEFS_ALIAS}" STREQUAL "")
        message(FATAL_ERROR "No custom typedef's header is defined for custom dependency library. If custom dependency library is set, then custom typedef's header also must be present.\n
            Note: current values: building library: '${LIB_NAME}'; custom dependency library: '${CUSTOM_DEPENDENCY_LIB_NAME}'; custom dependency library alias: '${CUSTOM_DEPENDENCY_ALIAS}'.\n
            ${NOTE_CUSTOMIZED_LIB_NAME}")
    endif()

    set(CUSTOMIZED "customized")
    set(FORBIDDEN_CHARS_REGEX "[^a-zA-Z0-9]")
    set(FORBIDDEN_CHARS_SUBSTITUTION "\_")

    if (NOT "${CUSTOM_DEPENDENCY_ALIAS}" STREQUAL "")
        string(REGEX REPLACE "${FORBIDDEN_CHARS_REGEX}" "${FORBIDDEN_CHARS_SUBSTITUTION}" CUSTOM_DEPENDENCY_ALIAS ${CUSTOM_DEPENDENCY_ALIAS})

        set(I -1)
        string(FIND ${LIB_NAME} ${CUSTOMIZED} I)
        if (I EQUAL -1)
            string(APPEND LIB_NAME "_and_${CUSTOM_DEPENDENCY_ALIAS}_lib")
        else()
            string(APPEND LIB_NAME "_customized_with_${TYPEDEFS_ALIAS}_lib")
        endif()
    endif()

    if (NOT "${CUSTOM_TYPEDEFS_ALIAS}" STREQUAL "")
        string(REGEX REPLACE "${FORBIDDEN_CHARS_REGEX}" "${FORBIDDEN_CHARS_SUBSTITUTION}" CUSTOM_TYPEDEFS_ALIAS ${CUSTOM_TYPEDEFS_ALIAS})

        set(I -1)
        string(FIND ${LIB_NAME} ${CUSTOMIZED} I)
        if (I EQUAL -1)
            string(APPEND LIB_NAME "_and_${CUSTOM_TYPEDEFS_ALIAS}_typedefs")
        else()
            string(APPEND LIB_NAME "_customized_with_${CUSTOM_TYPEDEFS_ALIAS}_typedefs")
        endif()
    endif()

    set(LIB_NAME ${LIB_NAME} PARENT_SCOPE)
endfunction()