/**
 * @file ISerializableAlignmentMayBeNotEqualTests.cpp
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

using size_type = typename Vector<uint8_t>::size_type;

template<typename T>
void mainTest()
{
    T input;
    fillingStruct(input);

    Walker<uint8_t> bin;
    csp::context::SData<Vector<uint8_t>> ctxIn(bin.getVector());
    csp::context::Flags flags;
    flags.alignmentMayBeNotEqual = true;
    ctxIn.setFlags(flags);

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    csp::context::DData<Walker<uint8_t>> ctxOut(bin);
    T output;

    EXPECT_EQ(output.deserialize(ctxOut), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);
}

TEST(SerializationAlignmentMayBeNotEqualTests, SimpleAssignableAlignedToOneT)
{
    mainTest<SimpleAssignableAlignedToOneSerializable<>>();
}

TEST(SerializationAlignmentMayBeNotEqualTests, SimpleAssignableT)
{
    mainTest<SimpleAssignableSerializable<>>();
}

TEST(SerializeNoFlagsTest, SpecialT)
{
    mainTest<SpecialProcessingTypeContainSerializable<>>();
}

TEST(SerializationAlignmentMayBeNotEqualTests, SimpleAssignableDataSizeT)
{
    SimpleAssignableSerializable input;
    fillingStruct(input);

    Walker<uint8_t> bin;
    csp::context::SData<Vector<uint8_t>> ctxIn(bin.getVector());
    csp::context::Flags flags;
    flags.alignmentMayBeNotEqual = true;
    ctxIn.setFlags(flags);

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    size_type sizeWithFlag = ctxIn.getBinaryData().size();

    ctxIn.clear();

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    size_type sizeWithoutFlag = ctxIn.getBinaryData().size();

    EXPECT_NE(sizeWithFlag, sizeWithoutFlag);
}

} // namespace anonymous
