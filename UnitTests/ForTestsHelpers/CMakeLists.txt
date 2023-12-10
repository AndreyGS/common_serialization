add_library(ForTestsHelpers STATIC "src/FillingStruct.cpp" "src/CleanAfterStruct.cpp")

target_compile_definitions(ForTestsHelpers PRIVATE USER_MODE)
target_link_libraries(ForTestsHelpers PRIVATE cslib SerializableStructs)
target_include_directories(ForTestsHelpers PUBLIC "include")
target_precompile_headers(ForTestsHelpers PRIVATE "include/ft_helpers/ft_helpers.h")
