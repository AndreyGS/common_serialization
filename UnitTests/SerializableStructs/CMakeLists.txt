add_library(SerializableStructs STATIC 
    "interface_for_test/src/Generated/SerializeData.cpp"
    "interface_for_test/src/Generated/SerializeDataLegacy.cpp"
    "interface_for_test/src/Generated/DeserializeData.cpp"
    "interface_for_test/src/Generated/DeserializeDataLegacy.cpp"
    "interface_for_test/src/Generated/ConvertToOldStruct.cpp"
    "interface_for_test/src/Generated/ConvertFromOldStruct.cpp")

target_compile_definitions(SerializableStructs PRIVATE USER_MODE)
target_link_libraries(SerializableStructs PRIVATE cslib)
target_include_directories(SerializableStructs PUBLIC 
    "interface_for_test/include" 
    "not_part_of_interfaces/include"
)
target_precompile_headers(SerializableStructs PRIVATE 
    "interface_for_test/include/interface_for_test/Interface.h"
    "not_part_of_interfaces/include/not_part_of_interfaces/Structs.h"
)
