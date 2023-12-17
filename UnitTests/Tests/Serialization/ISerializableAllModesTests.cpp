/**
 * @file ISerializableAllModesTests.cpp
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

using namespace common_serialization;
using namespace interface_for_test;
using namespace ft_helpers;

using size_type = typename BinVector::size_type;

template<typename TS, typename TD>
void mainTest(csp::context::DataFlags flags, uint32_t targetVersion)
{
    TS input;
    fillingStruct(input);

    BinWalker bin;
    csp::context::SData<> ctxIn(bin.getVector());
    flags.allowUnmanagedPointers = true;
    flags.checkRecursivePointers = true;
    ctxIn.setFlags(flags);
    ctxIn.setInterfaceVersion(targetVersion);

    std::unordered_map<const void*, uint64_t> sMap;
    ctxIn.setPointersMap(sMap);

    // In this function we need special processing to emulate behavior of real interface.
    // Otherwise serialization and deserialization of old interface can use newer structs inside main struct.
    // Currently this issue applies only in serialization to and from interface Version2.
    // And for bypassing this we manualy set interfaceVersionsNotMatch flag in respective contexts.
    EXPECT_EQ(csp::processing::serializeHeaderContext(ctxIn), Status::kNoError);
    EXPECT_EQ(csp::processing::serializeDataContext<TS>(ctxIn), Status::kNoError);

    if (TS::getInterfaceVersion() == 2)
        ctxIn.setInterfaceVersionsNotMatch(true);

    EXPECT_EQ(csp::processing::DataProcessor::serializeData(input, ctxIn), Status::kNoError);
    
    TD output;
    csp::context::DData<> ctxOut(bin);
    std::unordered_map<uint64_t, void*> dMap;
    ctxOut.setPointersMap(dMap);
    Vector<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(addedPointers);

    EXPECT_EQ(csp::processing::deserializeHeaderContext(ctxOut), Status::kNoError);

    Uuid id;
    uint32_t minimumInterfaceVersion = 0;

    EXPECT_EQ(csp::processing::deserializeDataContext(ctxOut, id), Status::kNoError);
    EXPECT_EQ(csp::processing::deserializeDataContextPostprocess<TD>(ctxOut, id, minimumInterfaceVersion), Status::kNoError);

    if (TD::getInterfaceVersion() == 2)
        ctxOut.setInterfaceVersionsNotMatch(true);

    EXPECT_EQ(csp::processing::DataProcessor::deserializeData(ctxOut, output), Status::kNoError);

    TD reference;
    fillingStruct(reference);

    EXPECT_EQ(output, reference);
    EXPECT_EQ(bin.tell(), bin.size());

    cleanAfterStruct(input);
}

template<typename TS, typename TD>
void allFlags(uint32_t targetVersion)
{
    csp::context::DataFlags flags;
    mainTest<TS, TD>(flags, targetVersion);
    flags.alignmentMayBeNotEqual = true;
    mainTest<TS, TD>(flags, targetVersion);
    flags.sizeOfArithmeticTypesMayBeNotEqual = true;
    mainTest<TS, TD>(flags, targetVersion);
    flags.alignmentMayBeNotEqual = false;
    mainTest<TS, TD>(flags, targetVersion);
}

// 0 to 3 if for interface versions serialization = 0, deserialization = 3
TEST(ISerializableAllModesTests, AllFlags0to3)
{
    allFlags<SForAllModesTests_Version0<>, DForAllModesTests<>>(SForAllModesTests_Version0<>::getInterfaceVersion());
}

TEST(ISerializableAllModesTests, AllFlags2to3)
{
    allFlags<SForAllModesTests_Version2<>, DForAllModesTests<>>(SForAllModesTests_Version2<>::getInterfaceVersion());
}

TEST(ISerializableAllModesTests, AllFlags3to3)
{
    allFlags<DForAllModesTests<>, DForAllModesTests<>>(DForAllModesTests<>::getInterfaceVersion());
}

TEST(ISerializableAllModesTests, AllFlags3to0)
{
    allFlags<DForAllModesTests<>, SForAllModesTests_Version0<>>(SForAllModesTests_Version0<>::getInterfaceVersion());
}

TEST(ISerializableAllModesTests, AllFlags3to2)
{
    allFlags<DForAllModesTests<>, SForAllModesTests_Version2<>>(SForAllModesTests_Version2<>::getInterfaceVersion());
}

} // namespace anonymous
