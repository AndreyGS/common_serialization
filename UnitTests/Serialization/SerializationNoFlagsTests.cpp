#include <gtest/gtest.h>
#include "Serialization/ISerializable.h"
#include "Containers/Walker.h"
#include "special_types.h"

using namespace special_types;
using namespace common_serialization;

TEST(SerializeNoFlagsTest, EmptyTypeT)
{
    EmptyTypeSerializable etSerIn;
    Walker<uint8_t> bin;
    EXPECT_EQ(etSerIn.serialize(bin.getVector()), Status::kNoError);

    EmptyTypeSerializable etSerOut;
    EXPECT_EQ(etSerIn.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());
}

TEST(SerializeNoFlagsTest, SimpleAssignableAlignedToOneT)
{
    SimpleAssignableAlignedToOneSerializable saaToSIn;
    saaToSIn.getX() = 1;
    saaToSIn.getY() = 2;

    Walker<uint8_t> bin;
    EXPECT_EQ(saaToSIn.serialize(bin.getVector()), Status::kNoError);

    SimpleAssignableAlignedToOneSerializable saaToSOut;
    EXPECT_EQ(saaToSOut.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(saaToSIn.getX(), saaToSOut.getX());
    EXPECT_EQ(saaToSIn.getY(), saaToSOut.getY());
}

TEST(SerializeNoFlagsTest, SimpleAssignableT)
{
    SimpleAssignableSerializable saSIn;
    saSIn.getI() = 1;
    saSIn.getJ() = 2;
    saSIn.getSaaToS().getX() = 3;
    saSIn.getSaaToS().getY() = 4;
    saSIn.getSaaToNS().a = 5;
    saSIn.getSaaToNS().s = 6;
    saSIn.getSaNS().q = 7;
    saSIn.getSaNS().w = 8;

    memcpy(saSIn.getArrI32(), "123456789012", saSIn.getSizeOfArrI32());
    memcpy(saSIn.getArrSaaToS(), "00001000034000056", saSIn.getSizeOfArrSaaToS());
    memcpy(saSIn.getArrSaaToNS(), "123456789", saSIn.getSizeOfArrSaaToNS());

    // we shouldn't memcpy array of SimpleAssignableNotSerializable cause its not aligned by one
    saSIn.getArrSaNS()[0].q = 123;
    saSIn.getArrSaNS()[0].w = 456;
    saSIn.getArrSaNS()[1].q = 567;
    saSIn.getArrSaNS()[1].w = 890;
    saSIn.getArrSaNS()[2].q = 789;
    saSIn.getArrSaNS()[2].w = 012;

    Walker<uint8_t> bin;
    EXPECT_EQ(saSIn.serialize(bin.getVector()), Status::kNoError);

    SimpleAssignableSerializable saSOut;
    EXPECT_EQ(saSOut.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(saSIn.getI(), saSOut.getI());
    EXPECT_EQ(saSIn.getJ(), saSOut.getJ());
    EXPECT_EQ(saSIn.getSaaToS().getX(), saSOut.getSaaToS().getX());
    EXPECT_EQ(saSIn.getSaaToS().getY(), saSOut.getSaaToS().getY());
    EXPECT_EQ(saSIn.getSaaToNS().a, saSOut.getSaaToNS().a);
    EXPECT_EQ(saSIn.getSaaToNS().s, saSOut.getSaaToNS().s);
    EXPECT_EQ(saSIn.getSaNS().q, saSOut.getSaNS().q);
    EXPECT_EQ(saSIn.getSaNS().w, saSOut.getSaNS().w);

    EXPECT_EQ(memcmp(saSIn.getArrI32(), saSOut.getArrI32(), saSIn.getSizeOfArrI32()), 0);
    EXPECT_EQ(memcmp(saSIn.getArrSaaToS(), saSOut.getArrSaaToS(), saSIn.getSizeOfArrSaaToS()), 0);
    EXPECT_EQ(memcmp(saSIn.getArrSaaToNS(), saSOut.getArrSaaToNS(), saSIn.getSizeOfArrSaaToNS()), 0);
    
    for (size_t i = 0; i < 3; ++i)
    {
        EXPECT_EQ(saSIn.getArrSaNS()[i].q, saSOut.getArrSaNS()[i].q);
        EXPECT_EQ(saSIn.getArrSaNS()[i].w, saSOut.getArrSaNS()[i].w);
    }

}

/*
Vector<SerTInh<>, DefaultAllocatorHelper<SerTInh<>>> vecTest;
vecTest.pushBack(SerTInh());
vecTest.pushBack(SerTInh());
vecTest[0].arr[1] = 1;
vecTest[0].arr[2] = 2;
vecTest[0].arr[3] = 3;
vecTest[0].arr[4] = 4;

vecTest[1].arr[4] = 6;

Walker<uint8_t, DefaultAllocatorHelper<uint8_t>> vecBin;
vecTest.serialize(vecBin);
vecTest.clear();

vecBin.seek(0);

//Vector<SerTInh<>, GenericAllocatorHelper<SerTInh<>, ConstructorNoexceptAllocator<SerTInh<>>>> vecTest2;
vecTest.deserialize(vecBin);*/
