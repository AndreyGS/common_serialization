add_library(SerializableStructs STATIC 
    "InterfaceForTest/src/processing/Generated/Serialize.cpp"
    "InterfaceForTest/src/processing/Generated/Deserialize.cpp"
    "InterfaceForTest/src/processing/Generated/SerializeLegacy.cpp"
    "InterfaceForTest/src/processing/Generated/DeserializeLegacy.cpp"
    "InterfaceForTest/src/processing/Generated/ConvertToOldStruct.cpp"
    "InterfaceForTest/src/processing/Generated/ConvertFromOldStruct.cpp" 

    "DescendantInterface/src/processing/Generated/Serialize.cpp"
    "DescendantInterface/src/processing/Generated/Deserialize.cpp"

    "AnotherYetInterface/src/processing/Generated/Serialize.cpp"
    "AnotherYetInterface/src/processing/Generated/Deserialize.cpp"
    
    "NotPartOfInterfaces/src/processing/Generated/Serialize.cpp"
    "NotPartOfInterfaces/src/processing/Generated/Deserialize.cpp"
    
    "WithStdIncludedInterface/src/processing/Generated/Deserialize.cpp"
    "WithStdIncludedInterface/src/processing/Generated/Serialize.cpp"
)

target_compile_definitions(SerializableStructs PRIVATE USER_MODE)
target_link_libraries(SerializableStructs PRIVATE cslib)
target_include_directories(SerializableStructs PUBLIC 
    "InterfaceForTest/include"
    "DescendantInterface/include"
    "AnotherYetInterface/include"
    "NotPartOfInterfaces/include"
    "StdStructsSerialiazation/include"
    "WithStdIncludedInterface/include"
)

target_precompile_headers(SerializableStructs PRIVATE "pch.h")