add_library(InterfaceForTestBase STATIC 
    "src/Generated/SerializeData.cpp"
    "src/Generated/SerializeDataLegacy.cpp"
    "src/Generated/DeserializeData.cpp"
    "src/Generated/DeserializeDataLegacy.cpp"
    "src/ConvertToOldStruct.cpp"
    "src/ConvertFromOldStruct.cpp")

target_compile_definitions(InterfaceForTestBase PRIVATE USER_MODE)
target_link_libraries(InterfaceForTestBase PRIVATE cslib)
target_precompile_headers(InterfaceForTestBase PRIVATE "Include/Interface.h")
