/**
 * @file RawKeeperAllocatorTests.cpp
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

using namespace special_types;
using namespace common_serialization;

namespace
{

template<typename T>
using ConstructorVectorAllocatorHelper = GenericAllocatorHelper<T, RawKeeperAllocator<T>>;

using size_type = typename Vector<int, ConstructorVectorAllocatorHelper<int>>::size_type;

constexpr size_t kArraySize = 3;
int g_array[kArraySize] = { 0 };

TEST(RawKeeperAllocatorTest, CopyConstuctorT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    int* p = rka.allocate(1);
    RawKeeperAllocator<int> rka2(rka);
    int* p2 = rka2.allocate(1);
    EXPECT_EQ(p2, nullptr);
    EXPECT_EQ(rka2.max_size(), 0);
}

TEST(RawKeeperAllocatorTest, MoveConstructorT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    int* p = rka.allocate(1);
    RawKeeperAllocator<int> rka2(std::move(rka));
    int* p2 = rka2.allocate(1);
    EXPECT_EQ(p2, p);
    EXPECT_EQ(rka2.max_size(), 1);
    EXPECT_EQ(rka2.construct(p, 5), Status::NoError);
    EXPECT_EQ(i, 5);
    EXPECT_EQ(rka2.construct(p + 1, 5), Status::ErrorInvalidArgument);
    p = rka.allocate(1);
    EXPECT_EQ(p, nullptr);

    RawKeeperAllocator<char> rka3(std::move(rka2));
    char* p3 = rka3.allocate(4);
    for (char i = 0; i < 4; ++i)
        p3[i] = i;
    EXPECT_EQ(rka3.construct(p3, 5), Status::NoError);
    EXPECT_EQ(*p3, 5);
    p = rka2.allocate(1);
    EXPECT_EQ(p, nullptr);
}

TEST(RawKeeperAllocatorTest, OperatorAssignT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    int* p = rka.allocate(1);
    RawKeeperAllocator<int> rka2 = rka;
    int* p2 = rka2.allocate(1);
    EXPECT_EQ(p2, nullptr);
    EXPECT_EQ(rka2.max_size(), 0);
}

TEST(RawKeeperAllocatorTest, OperatorMoveT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    int* p = rka.allocate(1);
    RawKeeperAllocator<int> rka2 = std::move(rka);
    int* p2 = rka2.allocate(1);
    EXPECT_EQ(p2, p);
    EXPECT_EQ(rka2.max_size(), 1);
    EXPECT_EQ(rka2.construct(p, 5), Status::NoError);
    EXPECT_EQ(i, 5);
    EXPECT_EQ(rka2.construct(p + 1, 5), Status::ErrorInvalidArgument);
    p = rka.allocate(1);
    EXPECT_EQ(p, nullptr);

    RawKeeperAllocator<char> rka3 = std::move(rka2);
    char* p3 = rka3.allocate(4);
    for (char i = 0; i < 4; ++i)
        p3[i] = i;
    EXPECT_EQ(rka3.construct(p3, 5), Status::NoError);
    EXPECT_EQ(*p3, 5);
    p = rka2.allocate(1);
    EXPECT_EQ(p, nullptr);
}

TEST(RawKeeperAllocatorTest, SetStorageT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    EXPECT_EQ(rka.max_size(), 1);
    rka.setStorage(nullptr, 10);
    EXPECT_EQ(rka.max_size(), 0);
    rka.setStorage(&i, 1);
    rka.setStorage(&i, 0);
    EXPECT_EQ(rka.max_size(), 0);
}

TEST(RawKeeperAllocatorTest, MaxSizeT)
{
    RawKeeperAllocator<int> rka;
    EXPECT_EQ(rka.max_size(), 0);
    int i{ 0 };
    rka.setStorage(&i, 1);
    EXPECT_EQ(rka.max_size(), 1);
}

TEST(RawKeeperAllocatorTest, AllocateT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);

    int* p = rka.allocate(1);
    EXPECT_TRUE(p == &i);
    p = rka.allocate(2);
    EXPECT_TRUE(p == nullptr);
    p = rka.allocate(0);
    EXPECT_TRUE(p == nullptr);
}

TEST(RawKeeperAllocatorTest, ConstructT)
{
    RawKeeperAllocator<int> rka;
    int i{ 0 };
    rka.setStorage(&i, 1);
    int* p = rka.allocate(1);
    EXPECT_EQ(rka.construct(p, 5), Status::NoError);
    EXPECT_EQ(i, 5);
    
    // Test for construct when out of storage bounds
    EXPECT_EQ(rka.construct(p - 1, 5), Status::ErrorInvalidArgument);
    EXPECT_EQ(rka.construct(p + 1, 5), Status::ErrorInvalidArgument);
    
    // Test for construct when pointer not aligned by type size in underlying array
    EXPECT_EQ(rka.construct(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(p) + 1), 5), Status::ErrorInvalidArgument);
}

TEST(RawKeeperAllocatorTest, VectorWorkTest)
{
    Vector<int, ConstructorVectorAllocatorHelper<int>> vec;

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.max_size(), 0);
    EXPECT_EQ(vec.data(), nullptr);

    vec.getAllocatorHelper().getAllocator().setStorage(g_array, kArraySize);
    EXPECT_EQ(vec.max_size(), kArraySize);

    vec.pushBack(0);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(reinterpret_cast<void*>(vec.data()), reinterpret_cast<void*>(g_array));

    vec.clear();

    // try to add more than we can hold
    for (int i = 0; i < kArraySize + 1; ++i)
        vec.pushBack(i);

    EXPECT_EQ(vec.size(), kArraySize);
    EXPECT_EQ(vec.capacity(), kArraySize);

    for (int i = 0; i < kArraySize; ++i)
        EXPECT_EQ(vec[i], i);
}

} // namespace
