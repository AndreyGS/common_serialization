/**
 * @file ISerializableSizeOfArithmeticTypesMayBeNotEqualTests.cpp
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

// In most of this tests we are simulating difference in arithmetic sizes by using
// distinct structs for serialization and deserialization
// (to accomplish that we set their name hashes to the same value)

namespace
{

using namespace special_types;

template<typename TS, typename TD>
void mainTest()
{
    TS input;
    fillingStruct(input);

    Walker<uint8_t> bin;
    csp::context::SData<Vector<uint8_t>> ctxIn(bin.getVector());
    csp::context::DataFlags flags;
    flags.sizeOfArithmeticTypesMayBeNotEqual = true;
    ctxIn.setFlags(flags);

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    csp::context::DData<Walker<uint8_t>> ctxOut(bin);
    TD output;

    EXPECT_EQ(output.deserialize(ctxOut), Status::kNoError);

    EXPECT_EQ(input.m_j, output.m_j);
    EXPECT_NE(input.m_k, output.m_k);
    EXPECT_EQ(static_cast<short>(input.m_k), output.m_k);

    // comparing with no flags size of serialized data
    TS input2;
    fillingStruct(input2);

    Walker<uint8_t> bin2;
    EXPECT_EQ(input2.serialize(bin2.getVector()), Status::kNoError);

    EXPECT_NE(bin2.size(), ctxIn.getBinaryData().size());

    cleanAfterStruct(input);
}

TEST(ISerializableSizeOfArithmeticTypesMayBeNotEqualTests, SimpleAssignableAlignedToOneSimilarTypeT)
{
    mainTest<SimpleAssignableAlignedToOneSimilarType1Serializable<>, SimpleAssignableAlignedToOneSimilarType2Serializable<>>();
}

TEST(ISerializableSizeOfArithmeticTypesMayBeNotEqualTests, SimpleAssignableSimilarTypeT)
{
    mainTest<SimpleAssignableSimilarType1Serializable<>, SimpleAssignableSimilarType2Serializable<>>();
}

TEST(ISerializableSizeOfArithmeticTypesMayBeNotEqualTests, SimilarTypeT)
{
    mainTest<SimilarType1Serializable<>, SimilarType2Serializable<>>();
}

TEST(ISerializableSizeOfArithmeticTypesMayBeNotEqualTests, SpecialTBasicT)
{
    SpecialProcessingTypeContainSerializable input;
    fillingStruct(input);

    Walker<uint8_t> bin;
    csp::context::SData<Vector<uint8_t>> ctxIn(bin.getVector());
    csp::context::DataFlags flags;
    flags.sizeOfArithmeticTypesMayBeNotEqual = true;
    flags.allowUnmanagedPointers = true;
    ctxIn.setFlags(flags);

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    csp::context::DData<Walker<uint8_t>> ctxOut(bin);

    Vector<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(addedPointers);

    SpecialProcessingTypeContainSerializable output;
    EXPECT_EQ(output.deserialize(ctxOut), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);

    cleanAfterStruct(input);
}

} // namespace anonymous
