add_library(for_tests_helpers STATIC "src/FillingStruct.cpp" "src/CleanAfterStruct.cpp")

target_compile_definitions(for_tests_helpers PRIVATE USER_MODE)
target_link_libraries(for_tests_helpers PRIVATE cslib serializable_structs)
target_include_directories(for_tests_helpers PUBLIC "include")
target_precompile_headers(for_tests_helpers PRIVATE "include/ft_helpers/ft_helpers.h")
