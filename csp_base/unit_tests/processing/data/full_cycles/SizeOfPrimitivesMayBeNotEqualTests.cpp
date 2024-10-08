/**
 * @file common_serializaiton/csp_base/unit_tests/processing/data/full_cycles/SizeOfPrimitivesMayBeNotEqualTests.cpp
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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
#include <common_serialization/tests_csp_interface/tests_csp_interface.h>
#include <common_serialization/tests_restricted_structs/tests_restricted_structs.h>

// In most of this tests we are simulating difference in arithmetic sizes by using
// distinct structs for serialization and deserialization
// (to accomplish that we set their name ids to the same value)

namespace
{

using namespace common_serialization;
using namespace tests_csp_interface;

template<typename TS, typename TD>
void mainTest()
{
    TS input;
    input.fill();

    BinWalkerT bin;
    csp::context::SData ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kSizeOfIntegersMayBeNotEqual));

    EXPECT_EQ(input.serialize(ctxIn), Status::NoError);

    csp::context::DData ctxOut(bin);
    TD output;

    EXPECT_EQ(output.deserialize(ctxOut), Status::NoError);

    TD reference;
    reference.fill();

    EXPECT_EQ(output, reference);

    // comparing with no dataFlags size of serialized data
    TS input2;
    input2.fill();

    BinWalkerT bin2;
    EXPECT_EQ(input2.serialize(bin2.getVector()), Status::NoError);

    EXPECT_NE(bin2.size(), ctxIn.getBinaryData().size());
}

TEST(SizeOfPrimitivesMayBeNotEqualTests, SimplyAssignableAlignedToOneSimilarTypeT)
{
    mainTest<SimplyAssignableAlignedToOneSimilarType1<>, SimplyAssignableAlignedToOneSimilarType2<>>();
}

TEST(SizeOfPrimitivesMayBeNotEqualTests, SimplyAssignableSimilarTypeT)
{
    mainTest<SimplyAssignableSimilarType1<>, SimplyAssignableSimilarType2<>>();
}

TEST(SizeOfPrimitivesMayBeNotEqualTests, SimilarTypeT)
{
    mainTest<SimilarType1<>, SimilarType2<>>();
}

TEST(SizeOfPrimitivesMayBeNotEqualTests, SpecialTBasicT)
{
    SpecialProcessingType input;
    input.fill();

    BinWalkerT bin;
    csp::context::SData ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kSizeOfIntegersMayBeNotEqual | csp::context::DataFlags::kAllowUnmanagedPointers));

    EXPECT_EQ(input.serialize(ctxIn), Status::NoError);

    csp::context::DData ctxOut(bin);

    VectorT<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(&addedPointers);

    SpecialProcessingType output;
    EXPECT_EQ(output.deserialize(ctxOut), Status::NoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);

    cleanAfterStruct(input);

    input.clean();
}

} // namespace
