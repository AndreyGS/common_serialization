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
    uint64_t ref64{ 0xffffffffffffffff };
    int64_t _ref64 = static_cast<int64_t>(0x8000000000000000);

    uint64_t i64{ 0 };
    int64_t _i64{ 0 };

    EXPECT_EQ(castToSmallerType(ref64, i64), Status::NoError);
    EXPECT_EQ(ref64, i64);
    EXPECT_EQ(castToSmallerType(_ref64, _i64), Status::NoError);
    EXPECT_EQ(_ref64, _i64);

    uint32_t ref32{ 0xffffffff };
    int32_t _ref32 = static_cast<int32_t>(0x80000000);

    uint32_t i32{ 0 };
    int32_t _i32{ 0 };

    EXPECT_EQ(castToSmallerType(ref64, i32), Status::ErrorValueOverflow);
    EXPECT_EQ(castToSmallerType(_ref64, _i32), Status::ErrorValueOverflow);

    EXPECT_EQ(castToSmallerType(ref32, i32), Status::NoError);
    EXPECT_EQ(ref32, i32);
    EXPECT_EQ(castToSmallerType(_ref32, _i32), Status::NoError);
    EXPECT_EQ(_ref32, _i32);

    uint16_t ref16{ 0xffff };
    int16_t _ref16 = static_cast<int16_t>(0x8000);

    uint16_t i16{ 0 };
    int16_t _i16{ 0 };

    EXPECT_EQ(castToSmallerType(ref64, i16), Status::ErrorValueOverflow);
    EXPECT_EQ(castToSmallerType(_ref64, _i16), Status::ErrorValueOverflow);

    EXPECT_EQ(castToSmallerType(ref16, i16), Status::NoError);
    EXPECT_EQ(ref16, i16);
    EXPECT_EQ(castToSmallerType(_ref16, _i16), Status::NoError);
    EXPECT_EQ(_ref16, _i16);

    uint8_t ref8{ 0xff };
    int8_t _ref8 = static_cast<int8_t>(0x80);

    uint8_t i8{ 0 };
    int8_t _i8{ 0 };

    EXPECT_EQ(castToSmallerType(ref64, i8), Status::ErrorValueOverflow);
    EXPECT_EQ(castToSmallerType(_ref64, _i8), Status::ErrorValueOverflow);

    EXPECT_EQ(castToSmallerType(ref8, i8), Status::NoError);
    EXPECT_EQ(ref8, i8);
    EXPECT_EQ(castToSmallerType(_ref8, _i8), Status::NoError);
    EXPECT_EQ(_ref8, _i8);
}

TEST(HelpersTests, CastToBiggerType)
{
    uint8_t ref8{ 0xff };
    int8_t _ref8 = static_cast<int8_t>(0x80);

    uint8_t i8{ 0 };
    int8_t _i8{ 0 };

    castToBiggerType(ref8, i8);
    EXPECT_EQ(ref8, i8);
    castToBiggerType(_ref8, _i8);
    EXPECT_EQ(_ref8, _i8);

    uint16_t ref16{ 0xffff };
    int16_t _ref16 = static_cast<int16_t>(0x8000);

    uint16_t i16{ 0 };
    int16_t _i16{ 0 };

    castToBiggerType(ref16, i16);
    EXPECT_EQ(ref16, i16);
    castToBiggerType(_ref16, _i16);
    EXPECT_EQ(_ref16, _i16);

    uint32_t ref32{ 0xffffffff };
    int32_t _ref32 = static_cast<int32_t>(0x80000000);

    uint32_t i32{ 0 };
    int32_t _i32{ 0 };

    castToBiggerType(ref32, i32);
    EXPECT_EQ(ref32, i32);
    castToBiggerType(_ref32, _i32);
    EXPECT_EQ(_ref32, _i32);

    uint64_t ref64{ 0xffffffffffffffff };
    int64_t _ref64 = static_cast<int64_t>(0x8000000000000000);

    uint64_t i64{ 0 };
    int64_t _i64{ 0 };

    castToBiggerType(ref64, i64);
    EXPECT_EQ(ref64, i64);
    castToBiggerType(_ref64, _i64);
    EXPECT_EQ(_ref64, _i64);
}

} // namespace
