/**
 * @file SerializationNoFlagsTests.cpp
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gtest/gtest.h>
#include "FillingSpecialTypes.h"
#include "Serialization/ISerializable.h"
#include "Containers/Walker.h"

namespace
{

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
    filling::_SimpleAssignableAlignedToOneSerializable(saaToSIn);

    Walker<uint8_t> bin;
    EXPECT_EQ(saaToSIn.serialize(bin.getVector()), Status::kNoError);

    SimpleAssignableAlignedToOneSerializable saaToSOut;
    EXPECT_EQ(saaToSOut.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_TRUE(saaToSIn == saaToSOut);
}

TEST(SerializeNoFlagsTest, SimpleAssignableT)
{
    SimpleAssignableSerializable saSIn;
    filling::_SimpleAssignableSerializable(saSIn);

    Walker<uint8_t> bin;
    EXPECT_EQ(saSIn.serialize(bin.getVector()), Status::kNoError);

    SimpleAssignableSerializable saSOut;
    EXPECT_EQ(saSOut.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_TRUE(saSIn == saSOut);
}

TEST(SerializeNoFlagsTest, SimpleAssignableDescendantT)
{
    SimpleAssignableDescendantSerializable saSDIn;
    filling::_SimpleAssignableDescendantSerializable(saSDIn);

    Walker<uint8_t> bin;
    EXPECT_EQ(saSDIn.serialize(bin.getVector()), Status::kNoError);

    SimpleAssignableDescendantSerializable saSDOut;
    EXPECT_EQ(saSDOut.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_TRUE(saSDIn == saSDOut);
}

} // namespace anonymous


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
