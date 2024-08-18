/**
 * @file common_serializaiton/csp_base/unit_tests/processing/data/full_cycles/MultiModesTests.cpp
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

namespace
{

using namespace common_serialization;
using namespace tests_csp_interface;

using size_type = typename BinVectorT::size_type;

template<typename TS, typename TD>
void mainTest(csp::context::DataFlags dataFlags, uint32_t targetVersion)
{
    TS input;
    input.fill();

    BinWalkerT bin;
    csp::context::SData ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kAllowUnmanagedPointers | csp::context::DataFlags::kCheckRecursivePointers));
    ctxIn.setInterfaceVersion(targetVersion);

    csp::context::SPointersMap sMap;
    ctxIn.setPointersMap(&sMap);

    // In this function we need special processing to emulate behavior of real interface.
    // Otherwise serialization and deserialization of old interface can use newer structs inside main struct.
    // Currently this issue applies only in serialization to and from interface Version2.
    // And for bypassing this we manualy set interfaceVersionsNotMatch flag in respective contexts.
    EXPECT_EQ(csp::processing::common::ContextProcessor::serialize(ctxIn), Status::NoError);
    EXPECT_EQ(csp::processing::data::ContextProcessor::serialize<TS>(ctxIn), Status::NoError);

    if (TS::getLatestInterfaceVersion() == 2)
        ctxIn.setInterfaceVersionsNotMatch(true);

    EXPECT_EQ(csp::processing::data::BodyProcessor::serialize(input, ctxIn), Status::NoError);
    
    TD output;
    csp::context::DData ctxOut(bin);
    csp::context::DPointersMap  dMap;
    ctxOut.setPointersMap(&dMap);
    VectorT<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(&addedPointers);

    EXPECT_EQ(csp::processing::common::ContextProcessor::deserialize(ctxOut), Status::NoError);

    csp::Id id;
    uint32_t minimumInterfaceVersion = 0;

    EXPECT_EQ(csp::processing::data::ContextProcessor::deserializeNoChecks(ctxOut, id), Status::NoError);
    EXPECT_EQ(csp::processing::data::ContextProcessor::deserializePostprocessId<TD>(id), Status::NoError);
    EXPECT_EQ(csp::processing::data::ContextProcessor::deserializePostprocessRest<TD>(ctxOut, minimumInterfaceVersion), Status::NoError);

    if (TD::getLatestInterfaceVersion() == 2)
        ctxOut.setInterfaceVersionsNotMatch(true);

    EXPECT_EQ(csp::processing::data::BodyProcessor::deserialize(ctxOut, output), Status::NoError);

    TD reference;
    reference.fill();

    EXPECT_EQ(output, reference);
    EXPECT_EQ(bin.tell(), bin.size());

    input.clean();
}

template<typename TS, typename TD>
void allFlags(uint32_t targetVersion)
{
    csp::context::DataFlags dataFlags;
    mainTest<TS, TD>(dataFlags, targetVersion);
    dataFlags.addFlags(csp::context::DataFlags::kAlignmentMayBeNotEqual);
    mainTest<TS, TD>(dataFlags, targetVersion);
    dataFlags.addFlags(csp::context::DataFlags::kSizeOfIntegersMayBeNotEqual);
    mainTest<TS, TD>(dataFlags, targetVersion);
    dataFlags.removeFlags(csp::context::DataFlags::kAlignmentMayBeNotEqual);
    mainTest<TS, TD>(dataFlags, targetVersion);
}

// 0 to 3 if for interface versions serialization = 0, deserialization = 3
TEST(MultiModesTests, AllFlags0to3)
{
    allFlags<SForAllModesTests_Version0<>, DForAllModesTests<>>(SForAllModesTests_Version0<>::getLatestInterfaceVersion());
}

TEST(MultiModesTests, AllFlags2to3)
{
    allFlags<SForAllModesTests_Version2<>, DForAllModesTests<>>(SForAllModesTests_Version2<>::getLatestInterfaceVersion());
}

TEST(MultiModesTests, AllFlags3to3)
{
    allFlags<DForAllModesTests<>, DForAllModesTests<>>(DForAllModesTests<>::getLatestInterfaceVersion());
}

TEST(MultiModesTests, AllFlags3to0)
{
    allFlags<DForAllModesTests<>, SForAllModesTests_Version0<>>(SForAllModesTests_Version0<>::getLatestInterfaceVersion());
}

TEST(MultiModesTests, AllFlags3to2)
{
    allFlags<DForAllModesTests<>, SForAllModesTests_Version2<>>(SForAllModesTests_Version2<>::getLatestInterfaceVersion());
}

} // namespace
