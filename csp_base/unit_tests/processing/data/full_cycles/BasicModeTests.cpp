/**
 * @file common_serializaiton/csp_base/unit_tests/processing/data/full_cycles/BasicModeTests.cpp
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

template<typename _T>
void mainTest()
{
    _T input;
    input.fill();

    BinWalkerT bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::NoError);

    _T output;
    EXPECT_EQ(output.deserialize(bin), Status::NoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);
}

TEST(BasicModeTests, EmptyTypeT)
{
    EmptyType input;
    BinWalkerT bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::NoError);

    EmptyType output;
    EXPECT_EQ(output.deserialize(bin), Status::NoError);
    EXPECT_EQ(bin.tell(), bin.size());
}

TEST(BasicModeTests, SimplyAssignableAlignedToOneT)
{
    mainTest<SimplyAssignableAlignedToOne<>>();
}

TEST(BasicModeTests, SimplyAssignableT)
{
    mainTest<SimplyAssignable<>>();
}

TEST(BasicModeTests, SimplyAssignableDescendantT)
{
    mainTest<SimplyAssignableDescendant<>>();
}

TEST(BasicModeTests, AlwaysSimplyAssignableT)
{
    mainTest<AlwaysSimplyAssignable<>>();
}

TEST(BasicModeTests, SimplyAssignableFixedSizeT)
{
    mainTest<SimplyAssignableFixedSize<>>();
}

TEST(BasicModeTests, DynamicPolymorphicT)
{
    mainTest<DynamicPolymorphic<>>();
}

TEST(BasicModeTests, DiamondT)
{
    mainTest<Diamond<>>();
}

} // namespace