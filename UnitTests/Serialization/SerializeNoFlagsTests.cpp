/*
Vector<SerTInh<>, DefaultVectorAllocatorHelper<SerTInh<>>> vecTest;
vecTest.pushBack(SerTInh());
vecTest.pushBack(SerTInh());
vecTest[0].arr[1] = 1;
vecTest[0].arr[2] = 2;
vecTest[0].arr[3] = 3;
vecTest[0].arr[4] = 4;

vecTest[1].arr[4] = 6;

Walker<uint8_t, DefaultVectorAllocatorHelper<uint8_t>> vecBin;
vecTest.serialize(vecBin);
vecTest.clear();

vecBin.seek(0);

//Vector<SerTInh<>, GenericAllocatorHelper<SerTInh<>, ConstructorNoexceptAllocator<SerTInh<>>>> vecTest2;
vecTest.deserialize(vecBin);*/
