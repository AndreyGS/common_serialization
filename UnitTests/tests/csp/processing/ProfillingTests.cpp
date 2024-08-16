/**
 * @file UnitTests/tests/csp/processing/ProfillingTests.cpp
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

// Should be moved in separate project later

#include <chrono>

namespace
{

using namespace common_serialization;
using namespace tests_csp_interface;
using namespace ft_helpers;

template<typename TS, typename TD>
void mainTest(csp::context::DataFlags dataFlags)
{
    TS input;
    input.fill();

    BinWalkerT bin;
    csp::context::SData ctxIn(bin.getVector());
    ctxIn.setDataFlags(dataFlags);

    csp::context::DData ctxOut(bin);
    TD output;

    for (size_t i = 0; i < 1000000; ++i)
    {
        ctxIn.clear();
        ctxOut.clear();
        input.serialize(ctxIn);
        output.deserialize(ctxOut);
    }

    cleanAfterStruct(input);
}

TEST(ProfillingTests, SimplyAssignableAlignedToOneT)
{
    mainTest<SimplyAssignableAlignedToOne<>, SimplyAssignableAlignedToOne<>>({});
}

TEST(ProfillingTests, SimplyAssignableT)
{
    mainTest<SimplyAssignable<>, SimplyAssignable<>>({});
}

TEST(ProfillingTests, SimplyAssignableDescendantT)
{
    mainTest<SimplyAssignableDescendant<>, SimplyAssignableDescendant<>>({});
}

TEST(ProfillingTests, AlwaysSimplyAssignableT)
{
    mainTest<AlwaysSimplyAssignable<>, AlwaysSimplyAssignable<>>({});
}

TEST(ProfillingTests, SimplyAssignableFixedSizeT)
{
    mainTest<SimplyAssignableFixedSize<>, SimplyAssignableFixedSize<>>({});
}

TEST(ProfillingTests, DynamicPolymorphicT)
{
    mainTest<DynamicPolymorphic<>, DynamicPolymorphic<>>({});
}

TEST(ProfillingTests, DiamondT)
{
    mainTest<Diamond<>, Diamond<>>({});
}

TEST(ProfillingTests, ContainBigStructs1T)
{
    ContainBigStructs1<> input;

    for (size_t i = 0; i < 100000; ++i)
    {
        SimplyAssignableAlignedToOne<> item1;
        fillingStruct(item1);
        input.m_big1.m_vector.pushBack(std::move(item1));
        SimplyAssignable<> item2;
        fillingStruct(item2);
        input.m_big2.m_vector.pushBack(std::move(item2));
        SimplyAssignableDescendant<> item3;
        fillingStruct(item3);
        input.m_big3.m_vector.pushBack(std::move(item3));
        AlwaysSimplyAssignable<> item4;
        fillingStruct(item4);
        input.m_big4.m_vector.pushBack(std::move(item4));
        SimplyAssignableFixedSize<> item5;
        fillingStruct(item5);
        input.m_big5.m_vector.pushBack(std::move(item5));
        DynamicPolymorphic<> item6;
        fillingStruct(item6);
        input.m_big6.m_vector.pushBack(std::move(item6));
        Diamond<> item7;
        fillingStruct(item7);
        input.m_big7.m_vector.pushBack(std::move(item7));
    }

    BinWalkerT bin;
    const auto serStart = std::chrono::steady_clock::now();
    input.serialize(bin.getVector());
    const auto serEnd = std::chrono::steady_clock::now();

    std::cout << "Serialization Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(serEnd - serStart) << "\n";

    ContainBigStructs1<> output;
    const auto deserStart = std::chrono::steady_clock::now();
    output.deserialize(bin);
    const auto deserEnd = std::chrono::steady_clock::now();

    std::cout << "Deserialization Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(deserEnd - deserStart) << "\n";

    EXPECT_EQ(input, output);

    for (size_t i = 0; i < 100000; ++i)
    {
        cleanAfterStruct(input.m_big1.m_vector[i]);
        cleanAfterStruct(input.m_big2.m_vector[i]);
        cleanAfterStruct(input.m_big3.m_vector[i]);
        cleanAfterStruct(input.m_big4.m_vector[i]);
        cleanAfterStruct(input.m_big5.m_vector[i]);
        cleanAfterStruct(input.m_big6.m_vector[i]);
        cleanAfterStruct(input.m_big7.m_vector[i]);
    }
}

TEST(ProfillingTests, ContainBigStructs2T)
{
    ContainBigStructs2ForAllModes<> input;

    for (size_t i = 0; i < 100000; ++i)
    {
        DForAllModesTests<> item;
        fillingStruct(item);
        input.m_big.m_vector.pushBack(std::move(item));
    }

    BinWalkerT bin;
    csp::context::SData ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kAllowUnmanagedPointers 
                                            | csp::context::DataFlags::kCheckRecursivePointers 
                                            | csp::context::DataFlags::kAlignmentMayBeNotEqual
                                            | csp::context::DataFlags::kSizeOfIntegersMayBeNotEqual));
    ctxIn.setInterfaceVersion(0);
    csp::context::SPointersMap sMap;
    ctxIn.setPointersMap(&sMap);

    const auto serStart = std::chrono::steady_clock::now();
    input.serialize(ctxIn);
    const auto serEnd = std::chrono::steady_clock::now();

    std::cout << "Serialization Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(serEnd - serStart) << "\n";

    csp::context::DData ctxOut(bin);
    csp::context::DPointersMap dMap;
    ctxOut.setPointersMap(&dMap);
    Vector<GenericPointerKeeper> addedPointers;
    ctxOut.setAddedPointers(&addedPointers);

    ContainBigStructs2ForAllModes<> output;
    const auto deserStart = std::chrono::steady_clock::now();
    output.deserialize(ctxOut);
    const auto deserEnd = std::chrono::steady_clock::now();

    std::cout << "Deserialization Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(deserEnd - deserStart) << "\n";

    for (size_t i = 0; i < 100000; ++i)
        cleanAfterStruct(input.m_big.m_vector[i]);
}

} // namespace
