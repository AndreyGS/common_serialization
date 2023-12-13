add_library(SerializableStructs STATIC 
    "InterfaceForTest/src/Generated/SerializeData.cpp"
    "InterfaceForTest/src/Generated/DeserializeData.cpp"
    "InterfaceForTest/src/Generated/SerializeDataLegacy.cpp"
    "InterfaceForTest/src/Generated/DeserializeDataLegacy.cpp"
    "InterfaceForTest/src/Generated/ConvertToOldStruct.cpp"
    "InterfaceForTest/src/Generated/ConvertFromOldStruct.cpp" 

    "DescendantInterface/src/Generated/SerializeData.cpp"
    "DescendantInterface/src/Generated/DeserializeData.cpp"

    "AnotherYetInterface/src/Generated/SerializeData.cpp"
    "AnotherYetInterface/src/Generated/DeserializeData.cpp"
    
    "NotPartOfInterfaces/src/Generated/SerializeData.cpp"
    "NotPartOfInterfaces/src/Generated/DeserializeData.cpp"
 )

target_compile_definitions(SerializableStructs PRIVATE USER_MODE)
target_link_libraries(SerializableStructs PRIVATE cslib)
target_include_directories(SerializableStructs PUBLIC 
    "InterfaceForTest/include"
    "DescendantInterface/include"
    "AnotherYetInterface/include"
    "NotPartOfInterfaces/include"
)

target_precompile_headers(SerializableStructs PRIVATE 
    "InterfaceForTest/include/interface_for_test/Interface.h"
    "DescendantInterface/include/descendant_interface/Interface.h"
    "AnotherYetInterface/include/another_yet_interface/Interface.h"
    "NotPartOfInterfaces/include/not_part_of_interfaces/NotInterface.h"
)
