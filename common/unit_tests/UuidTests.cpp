/**
 * @file common_serializaiton/common/unit_tests/UuidTests.cpp
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
#include <common_serialization/common/Uuid.h>

using namespace common_serialization;

namespace
{

TEST(UuidTests, Ctor)
{
    constexpr uint64_t high = 0x0123456789abcdef;
    constexpr uint64_t low = 0x02468ace13579bdf;
    Uuid uuid{ high, low };

    EXPECT_EQ(uuid.m_high, helpers::reverseEndianessUInt64(high));
    EXPECT_EQ(uuid.m_low, helpers::reverseEndianessUInt64(low));
}

TEST(UuidTests, Ctor2)
{
    constexpr uint32_t ui32 = 0x01234567;
    constexpr uint16_t ui16_1 = 0x89ab;
    constexpr uint16_t ui16_2 = 0xcdef;
    constexpr uint16_t ui16_3 = 0x0246;
    constexpr uint64_t ui64 = 0x00008ace13579bdf;
    Uuid uuid{ ui32, ui16_1, ui16_2, ui16_3, ui64 };

    EXPECT_EQ(uuid.m_high, helpers::reverseEndianessUInt64(0x0123456789abcdef));
    EXPECT_EQ(uuid.m_low, helpers::reverseEndianessUInt64(0x02468ace13579bdf));
}

TEST(UuidTests, OperatorLt)
{

    Uuid uuid1{ 0x0123456789abcdef, 0x02468ace13579bdf };
    Uuid uuid2{ 0x0123456789abcdef, 0x02468ace13579bde };

    EXPECT_TRUE(uuid2 < uuid1);
    EXPECT_FALSE(uuid1 < uuid2);

    Uuid uuid3{ 0x0123456789abcdee, 0x02468ace13579bdf };

    EXPECT_TRUE(uuid3 < uuid1);
    EXPECT_FALSE(uuid1 < uuid3);
}

TEST(UuidTests, OperatorEq)
{

    Uuid uuid1{ 0x0123456789abcdef, 0x02468ace13579bdf };
    EXPECT_TRUE(uuid1 == uuid1);

    Uuid uuid2{ 0x0123456789abcdef, 0x02468ace13579bde };
    EXPECT_FALSE(uuid1 == uuid2);

    Uuid uuid3{ 0x0123456789abcdee, 0x02468ace13579bdf };
    EXPECT_FALSE(uuid1 == uuid3);
}


TEST(UuidTests, StdHash)
{
    Uuid uuid1{ 0x0123456789abcdef, 0x02468ace13579bdf };
    size_t hash1 = std::hash<Uuid>()(uuid1);
    EXPECT_EQ(hash1, 0x000700000002a000);

    Uuid uuid2{ 0x0123456789abcdef, 0x02468ace13579bde };
    size_t hash2 = std::hash<Uuid>()(uuid2);
    EXPECT_EQ(hash2, 0x000701000002a24f);

    Uuid uuid3{ 0x0123456789abcdee, 0x02468ace13579bdf };
    size_t hash3 = std::hash<Uuid>()(uuid3);
    EXPECT_EQ(hash3, 0x000701000002a1cf);
}

} // namespace
