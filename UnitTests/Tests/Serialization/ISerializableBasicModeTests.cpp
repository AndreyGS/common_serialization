/**
 * @file ISerializableBasicModeTests.cpp
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

namespace
{

using namespace special_types;

template<typename T>
void mainTest()
{
    T input;
    fillingStruct(input);

    BinWalker bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::kNoError);

    T output;
    EXPECT_EQ(output.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);
}

TEST(ISerializableBasicModeTests, EmptyTypeT)
{
    EmptyTypeSerializable input;
    BinWalker bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::kNoError);

    EmptyTypeSerializable output;
    EXPECT_EQ(output.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    cleanAfterStruct(input);
}

TEST(ISerializableBasicModeTests, SimpleAssignableAlignedToOneT)
{
    mainTest<SimpleAssignableAlignedToOneSerializable<>>();
}

TEST(ISerializableBasicModeTests, SimpleAssignableT)
{
    mainTest<SimpleAssignableSerializable<>>();
}

TEST(ISerializableBasicModeTests, SimpleAssignableDescendantT)
{
    mainTest<SimpleAssignableDescendantSerializable<>>();
}

TEST(ISerializableBasicModeTests, DynamicPolymorphicT)
{
    mainTest<DynamicPolymorphicSerializable<>>();
}

TEST(ISerializableBasicModeTests, DiamondT)
{
    mainTest<DiamondSerializable<>>();
}

} // namespace anonymous
