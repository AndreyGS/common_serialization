/**
 * @file common_serializaiton/common/unit_tests/HelpersTests.cpp
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

#include <common_serialization/common/helpers.h>

using namespace common_serialization;
using namespace common_serialization::helpers;

namespace
{

TEST(HelpersTests, IsBitness32)
{
#ifndef X32_PLATFORM
    EXPECT_FALSE(isBitness32());
#else
    EXPECT_TRUE(isBitness32());
#endif
}

TEST(HelpersTests, IsBitness64)
{
    EXPECT_TRUE(isBitness32() != isBitness64());
}

TEST(HelpersTests, IsBigEndianPlatform)
{
    EXPECT_EQ(isBigEndianPlatform(), std::endian::native == std::endian::big);
}

TEST(HelpersTests, IsLittleEndianPlatform)
{
    EXPECT_EQ(isLittleEndianPlatform(), std::endian::native == std::endian::little);
}

TEST(HelpersTests, AreRegionsOverlap)
{
    int arr[4]{ 0 };
    int* p1 = arr;
    int* p2 = &arr[2];

    EXPECT_TRUE(areRegionsOverlap(p1, p2, 3));
    EXPECT_FALSE(areRegionsOverlap(p1, p2, 2));
    EXPECT_TRUE(areRegionsOverlap(p2, p1, 3));
    EXPECT_FALSE(areRegionsOverlap(p2, p1, 2));
}

TEST(HelpersTests, Countof)
{
    int arr[4]{ 0 };
    EXPECT_EQ(countof(arr), 4);
}

TEST(HelpersTests, ReverseEndianessUInt64)
{

    uint64_t i{ 0x0123456789abcdef };
    EXPECT_EQ(reverseEndianessUInt64(i), 0xefcdab8967452301);
}

TEST(HelpersTests, ReverseEndianessUInt32)
{

    uint32_t i{ 0x01234567 };
    EXPECT_EQ(reverseEndianessUInt32(i), 0x67452301);
}

TEST(HelpersTests, ReverseEndianessUInt16)
{

    uint16_t i{ 0x0123 };
    EXPECT_EQ(reverseEndianessUInt16(i), 0x2301);
}

TEST(HelpersTests, ReverseEndianess)
{
    uint64_t i1{ 0x0123456789abcdef };
    uint32_t i2{ 0x01234567 };
    uint16_t i3{ 0x0123 };

    EXPECT_EQ(reverseEndianess(i1), 0xefcdab8967452301);
    EXPECT_EQ(reverseEndianess(i2), 0x67452301);
    EXPECT_EQ(reverseEndianess(i3), 0x2301);
}

TEST(HelpersTests, CastToSmallerType)
{
    uint64_t i1{ 0xffffffffffffffff };
    uint32_t i2{ 0 };
    int32_t _i2{ 0 };

    EXPECT_EQ(castToSmallerType(i1, i2), Status::ErrorValueOverflow);
    EXPECT_EQ(castToSmallerType(i1, _i2), Status::ErrorValueOverflow);
}

} // namespace