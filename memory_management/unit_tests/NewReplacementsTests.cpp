/**
 * @file common_serializaiton/memory_management/unit_tests/NewReplacementsTests.cpp
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
#define AGS_CS_OPERATOR_NEW_REPLACEMENT
#define AGS_CS_OPERATOR_DELETE_REPLACEMENT
#include <common_serialization/memory_management/new_replacements.h>
#include <common_serialization/memory_management/delete_replacements.h>


namespace
{

TEST(NewReplacementsTests, NewSizeT)
{
    int* pI = new int(5);
    EXPECT_TRUE(pI != nullptr);
    EXPECT_EQ(*pI, 5);
}

TEST(NewReplacementsTests, NewSizeTArray)
{
    int* pI = new int[2](5);
    EXPECT_TRUE(pI != nullptr);
    EXPECT_EQ(pI[0], 5);
    EXPECT_EQ(pI[1], 0);
}

TEST(NewReplacementsTests, NewSizeTArrayOverflow)
{
    uint8_t* pI = new uint8_t[common_serialization::helpers::fixed_width_integer<sizeof(size_t), false>::max / 2];
    EXPECT_EQ(pI, nullptr);
}

// Tip: all replaced new operators are not throwing not only that are tested below
TEST(NewReplacementsTests, NewSizeTNoThrow)
{
    int* pI = new (std::nothrow) int(5);
    EXPECT_TRUE(pI != nullptr);
    EXPECT_EQ(*pI, 5);
}

TEST(NewReplacementsTests, NewSizeTArrayNoThrow)
{
    int* pI = new (std::nothrow) int[2](5);
    EXPECT_TRUE(pI != nullptr);
    EXPECT_EQ(pI[0], 5);
    EXPECT_EQ(pI[1], 0);
}

TEST(NewReplacementsTests, NewSizeTArrayOverflowNoThrow)
{
    uint8_t* pI = new (std::nothrow) uint8_t[common_serialization::helpers::fixed_width_integer<sizeof(size_t), false>::max / 2];
    EXPECT_EQ(pI, nullptr);
}

} // namespace