add_library(SerializableStructs STATIC 
    "InterfaceForTest/src/Generated/SerializeData.cpp"
    "InterfaceForTest/src/Generated/SerializeDataLegacy.cpp"
    "InterfaceForTest/src/Generated/DeserializeData.cpp"
    "InterfaceForTest/src/Generated/DeserializeDataLegacy.cpp"
    "InterfaceForTest/src/Generated/ConvertToOldStruct.cpp"
    "InterfaceForTest/src/Generated/ConvertFromOldStruct.cpp" 
    
    "NotPartOfInterfaces/src/Generated/SerializeData.cpp"
    "NotPartOfInterfaces/src/Generated/DeserializeData.cpp"
 "DescendantInterface/include/descendant_interface/Interface.h" "DescendantInterface/include/descendant_interface/Structs.h")

target_compile_definitions(SerializableStructs PRIVATE USER_MODE)
target_link_libraries(SerializableStructs PRIVATE cslib)
target_include_directories(SerializableStructs PUBLIC 
    "InterfaceForTest/include" 
    "NotPartOfInterfaces/include"
)

target_precompile_headers(SerializableStructs PRIVATE 
    "InterfaceForTest/include/interface_for_test/Interface.h"
    "NotPartOfInterfaces/include/not_part_of_interfaces/NotInterface.h"
)
