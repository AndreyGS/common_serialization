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

using namespace special_types;

using size_type = typename Vector<uint8_t>::size_type;

template<typename TS, typename TD>
void mainTest(csp::context::DataFlags flags, uint32_t targetInterfaceVersion = TS::getInterfaceVersion())
{
    TS input;
    fillingStruct(input);

    Walker<uint8_t> bin;
    csp::context::SData<Vector<uint8_t>> ctxIn(bin.getVector());
    flags.allowUnmanagedPointers = true;
    flags.checkRecursivePointers = true;
    ctxIn.setFlags(flags);
    ctxIn.setInterfaceVersion(targetInterfaceVersion);

    std::unordered_map<const void*, uint64_t> sMap;
    ctxIn.setPointersMap(sMap);

    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);
    
    TD output;
    csp::context::DData<Walker<uint8_t>> ctxOut(bin);
    std::unordered_map<uint64_t, void*> dMap;
    ctxOut.setPointersMap(dMap);
    Vector<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(addedPointers);

    EXPECT_EQ(output.deserialize(ctxOut), Status::kNoError);

    TD reference;
    fillingStruct(reference);

    EXPECT_EQ(output, reference);
    EXPECT_EQ(bin.tell(), bin.size());

    cleanAfterStruct(input);
}

TEST(ISerializableAllModesTests, NoFlags0to3)
{
    mainTest<SForAllModesTests_Version0<>, DForAllModesTests<>>(csp::context::DataFlags{});
}

TEST(ISerializableAllModesTests, NoFlags2to3)
{
    mainTest<SForAllModesTests_Version2<>, DForAllModesTests<>>(csp::context::DataFlags{});
}

TEST(ISerializableAllModesTests, NoFlags3to3)
{
    mainTest<SForAllModesTests<>, DForAllModesTests<>>(csp::context::DataFlags{});
}
/*
TEST(ISerializableAllModesTests, NoFlags3to0)
{
    mainTest<SForAllModesTests<>, SForAllModesTests_Version0<>>(csp::context::DataFlags{}, SForAllModesTests_Version0<>::getInterfaceVersion());
}

TEST(ISerializableAllModesTests, NoFlags3to2)
{
    mainTest<SForAllModesTests<>, SForAllModesTests_Version2<>>(csp::context::DataFlags{}, SForAllModesTests_Version2<>::getInterfaceVersion());
}
*/

} // namespace anonymous
