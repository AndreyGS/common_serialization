/**
 * @file RawNoexceptAllocatorTests.cpp
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
#include <common_serialization/allocators/allocators.h>
#include <tests_special_types/special_types.h>

using namespace special_types;
using namespace common_serialization;

namespace
{

TEST(RawNoexceptAllocatorTests, MaxSizeT)
{
    RawNoexceptAllocator<int> rna;
    EXPECT_EQ(rna.max_size(), static_cast<size_t>(-1) / sizeof(int));
    RawNoexceptAllocator<short> rna2;
    EXPECT_EQ(rna2.max_size(), static_cast<size_t>(-1) / sizeof(short));
}

TEST(RawNoexceptAllocatorTests, AllocateT)
{
    RawNoexceptAllocator<int> rna;
    int* p = rna.allocate(5);
    EXPECT_TRUE(p != nullptr);
    for (int i = 0; i < 5; ++i)
        p[i] = i;
    rna.deallocate(p);

    p = rna.allocate(0xffffffffffffffff);
    EXPECT_TRUE(p == nullptr);
}

TEST(RawNoexceptAllocatorTests, ConstructT)
{
    RawNoexceptAllocator<int> rna;
    int* p = rna.allocate(1);
    EXPECT_EQ(rna.construct(p, 5), Status::NoError);
    EXPECT_EQ(*p, 5);
    rna.deallocate(p);

    RawNoexceptAllocator<PodStruct> rna2;
    PodStruct* p2 = rna2.allocate(1);
    EXPECT_EQ(rna2.construct(p2, 5), Status::NoError);
    EXPECT_EQ(p2->i, 5);
    rna2.deallocate(p2);
}

} // namespace