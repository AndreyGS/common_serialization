add_library(serializable_structs STATIC 
    "interface_for_test/src/processing/Generated/Serialize.cpp"
    "interface_for_test/src/processing/Generated/Deserialize.cpp"
    "interface_for_test/src/processing/Generated/SerializeLegacy.cpp"
    "interface_for_test/src/processing/Generated/DeserializeLegacy.cpp"
    "interface_for_test/src/processing/Generated/ConvertToOldStruct.cpp"
    "interface_for_test/src/processing/Generated/ConvertFromOldStruct.cpp" 

    "descendant_interface/src/processing/Generated/Serialize.cpp"
    "descendant_interface/src/processing/Generated/Deserialize.cpp"

    "another_yet_interface/src/processing/Generated/Serialize.cpp"
    "another_yet_interface/src/processing/Generated/Deserialize.cpp"
    
    "restricted_structs/src/processing/Generated/Serialize.cpp"
    "restricted_structs/src/processing/Generated/Deserialize.cpp"
    
    "with_std_included_interface/src/processing/Generated/Deserialize.cpp"
    "with_std_included_interface/src/processing/Generated/Serialize.cpp"
)

target_compile_definitions(serializable_structs PRIVATE USER_MODE)
target_link_libraries(serializable_structs PRIVATE cslib)
target_include_directories(serializable_structs PUBLIC 
    "interface_for_test/include"
    "descendant_interface/include"
    "another_yet_interface/include"
    "restricted_structs/include"
    "StdStructsSerialiazation/include"
    "with_std_included_interface/include"
)

target_precompile_headers(serializable_structs PRIVATE "pch.h")