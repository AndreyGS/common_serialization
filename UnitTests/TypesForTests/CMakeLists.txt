add_library(types4tests STATIC "src/SpecialTypesFillingStruct.cpp" "src/SpecialTypesCleanAfterStruct.cpp" "include/SimpleDataClient.h" "include/SimpleDataServers.h")

target_compile_definitions(types4tests PRIVATE USER_MODE)
target_link_libraries(types4tests PRIVATE cslib SerializableStructs)
target_include_directories(types4tests PRIVATE "include")
target_precompile_headers(types4tests PRIVATE "include/TypesForTests.h")
