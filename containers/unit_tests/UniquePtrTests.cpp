/**
 * @file common_serializaiton/containers/unit_tests/UniquePtrTests.cpp
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
#include <common_serialization/containers/containers.h>
#include <common_serialization/tests_special_types/structs.h>

namespace
{

using namespace tests_special_types;
using namespace common_serialization;


struct AnotherCustomDeleter
{
    static inline int i{ 0 };

    template<typename T>
    void operator()(T p)
    {
        i += 1;
        delete p;
    }
};

struct AnotherCustomArrDeleter
{
    static inline int i{ 0 };

    template<typename T>
    void operator()(T p)
    {
        i += 1;
        delete[] p;
    }
};

TEST(UniquePtrTest, Ctors)
{
    UniquePtr<int> uptr;
    EXPECT_EQ(uptr.get(), nullptr);

    UniquePtr<int> uptr1(makeUnique<int>(1));
    EXPECT_EQ(*uptr1.get(), 1);

    UniquePtr<CustomDeleterStruct> uptr2(new CustomDeleterStructDesc{ 2 });
    EXPECT_EQ(uptr2.get()->m_i, 2);

    CustomDeleter customDeleter;

    UniquePtr<CustomDeleterStruct, CustomDeleter> uptr3(new CustomDeleterStructDesc{ 3 }, customDeleter);
    EXPECT_EQ(uptr3.get()->m_i, 3);

    UniquePtr<CustomDeleterStruct, CustomDeleter> uptr4(new CustomDeleterStruct{ 4 }, CustomDeleter{});
    EXPECT_EQ(uptr4.get()->m_i, 4);

    CustomDeleter2 customDeleter2;

    UniquePtr<CustomDeleterStruct, CustomDeleter> uptr5(new CustomDeleterStructDesc{ 5 }, customDeleter2);
    EXPECT_EQ(uptr5.get()->m_i, 5);

    UniquePtr<CustomDeleterStruct, CustomDeleter> uptr6(new CustomDeleterStruct{ 6 }, CustomDeleter2{});
    EXPECT_EQ(uptr6.get()->m_i, 6);

    UniquePtr<CustomDeleterStruct> uptr7(makeUnique<CustomDeleterStructDesc>(7));
    EXPECT_EQ(uptr7.get()->m_i, 7);

    UniquePtr<CustomDeleterStructDesc, CustomDeleter2> uptr8a(new CustomDeleterStructDesc(8));

    UniquePtr<CustomDeleterStruct, CustomDeleter> uptr8(std::move(uptr8a));
    EXPECT_EQ(uptr8.get()->m_i, 8);
}

TEST(UniquePtrTest, AssignmentOperators)
{
    auto deleter1 = [](int* p)
    {
        static bool valueTested = false;
        
        if (!valueTested)
        {
            EXPECT_EQ(*(int*)p, 1);
            valueTested = true;
        }

        delete p;
    };

    auto deleter2 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 10);
        delete p;
    };

    UniquePtr<int, void(*)(int*)> uptr(new int(5), deleter1);
    UniquePtr<int, void(*)(int*)> uptr2(new int(10), deleter2);

    uptr2 = std::move(uptr);
    EXPECT_EQ(uptr.get(), nullptr);
    EXPECT_EQ(*uptr2.get(), 5);

    *uptr2.get() = 1;

    uptr = new int{ 6 };
    EXPECT_EQ(*uptr.get(), 6);

    // self-assignment test
    uptr = std::move(uptr);
    EXPECT_EQ(*uptr.get(), 6);
}

TEST(UniquePtrTest, Destructor)
{
    UniquePtr<int> uptr(makeUnique<int>(5));
    uptr.~UniquePtr<int>();

    EXPECT_EQ(uptr.get(), nullptr);

    // test custom deleter function
    AnotherCustomDeleter::i = 0;
    UniquePtr<int, AnotherCustomDeleter> uptr2(new int{ 5 });
    EXPECT_EQ(AnotherCustomDeleter::i, 0);
    uptr2.~UniquePtr<int, AnotherCustomDeleter>();
    EXPECT_EQ(uptr2.get(), nullptr);
    EXPECT_EQ(AnotherCustomDeleter::i, 1);
}

TEST(UniquePtrTest, Get)
{
    UniquePtr<int> uptr;
    EXPECT_EQ(uptr.get(), nullptr);

    uptr = makeUnique<int>(5);
    EXPECT_EQ(*uptr.get(), 5);
}

TEST(UniquePtrTest, Release)
{
    UniquePtr<int> uptr(makeUnique<int>(5));
    int* i = uptr.release();

    EXPECT_EQ(uptr.get(), nullptr);
    EXPECT_EQ(*i, 5);
    delete i;
}

TEST(UniquePtrTest, Reset)
{
    UniquePtr<int> uptr(makeUnique<int>(5));
    UniquePtr<int> uptr2(makeUnique<int>(10));
    uptr.reset(uptr2.release());

    EXPECT_EQ(*uptr.get(), 10);
}

TEST(UniquePtrTest, Swap)
{
    auto deleter1 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 1);
        delete p;
    };

    auto deleter2 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 0);
        delete p;
    };

    UniquePtr<int, void(*)(int*)> uptr(new int(5), deleter1);
    UniquePtr<int, void(*)(int*)> uptr2(new int(10), deleter2);
    uptr.swap(uptr2);

    EXPECT_EQ(*uptr.get(), 10);
    EXPECT_EQ(*uptr2.get(), 5);

    *uptr.get() = 0;
    *uptr2.get() = 1;

    uptr.reset(nullptr);
    uptr2.reset(nullptr);
}

TEST(UniquePtrTest, VariousOperators)
{
    UniquePtr<CustomDeleterStruct> uptr(makeUnique<CustomDeleterStruct>(5));

    EXPECT_EQ(uptr->m_i, 5);
    EXPECT_EQ((*uptr).m_i, 5);
    EXPECT_TRUE(static_cast<bool>(uptr));

    uptr.reset();

    EXPECT_FALSE(static_cast<bool>(uptr));
}

TEST(UniquePtrArrTest, Constructors)
{
    UniquePtr<int[]> uptr;
    EXPECT_EQ(uptr.get(), nullptr);

    UniquePtr<int[]> uptr1(makeUnique<int[]>(1,1));
    EXPECT_EQ(*uptr1.get(), 1);

    UniquePtr<VirtDistructorOwner[]> uptr2(new VirtDistructorOwnerDesc[2]{ 2 , 2 });
    EXPECT_EQ(uptr2.get()[0].i, 2);
    EXPECT_EQ(uptr2.get()[1].i, 2);
   
    CustomArrDeleter customDeleter;

    UniquePtr<VirtDistructorOwner[], CustomArrDeleter> uptr3(new VirtDistructorOwnerDesc[1]{3}, customDeleter);
    EXPECT_EQ(uptr3.get()->i, 3);
    
    UniquePtr<VirtDistructorOwner[], CustomArrDeleter> uptr4(new VirtDistructorOwner[1]{4}, CustomArrDeleter{});
    EXPECT_EQ(uptr4.get()->i, 4);

    CustomArrDeleter2 customDeleter2;

    UniquePtr<VirtDistructorOwner[], CustomArrDeleter> uptr5(new VirtDistructorOwnerDesc[1]{ 5 }, customDeleter2);
    EXPECT_EQ(uptr5.get()->i, 5);

    UniquePtr<VirtDistructorOwner[], CustomArrDeleter> uptr6(new VirtDistructorOwner[1]{ 6 }, CustomArrDeleter2{});
    EXPECT_EQ(uptr6.get()->i, 6);

    UniquePtr<VirtDistructorOwner[]> uptr7(makeUnique<VirtDistructorOwnerDesc[]>(1, 7));
    EXPECT_EQ(uptr7.get()->i, 7);

    UniquePtr<VirtDistructorOwnerDesc[], CustomArrDeleter2> uptr8a(new VirtDistructorOwnerDesc[1](8));

    UniquePtr<VirtDistructorOwner[], CustomArrDeleter> uptr8(std::move(uptr8a));
    EXPECT_EQ(uptr8.get()->i, 8);
}

TEST(UniquePtrArrTest, AssignmentOperators)
{
    auto deleter1 = [](int* p)
    {
        static bool valueTested = false;
        
        if (!valueTested)
        {
            EXPECT_EQ(*(int*)p, 1);
            valueTested = true;
        }

        delete[] p;
    };

    auto deleter2 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 10);
        delete[] p;
    };

    UniquePtr<int[], void(*)(int*)> uptr(new int[](5), deleter1);
    UniquePtr<int[], void(*)(int*)> uptr2(new int[](10), deleter2);

    uptr2 = std::move(uptr);
    EXPECT_EQ(uptr.get(), nullptr);
    EXPECT_EQ(*uptr2.get(), 5);

    *uptr2.get() = 1;

    uptr = new int[]{ 6 };
    EXPECT_EQ(*uptr.get(), 6);

    // self-assignment test
    uptr = std::move(uptr);
    EXPECT_EQ(*uptr.get(), 6);
}

TEST(UniquePtrArrTest, Destructor)
{
    UniquePtr<int[]> uptr(makeUnique<int[]>(5));
    uptr.~UniquePtr<int[]>();

    EXPECT_EQ(uptr.get(), nullptr);

    // test custom deleter function
    AnotherCustomArrDeleter::i = 0;
    UniquePtr<int[], AnotherCustomArrDeleter> uptr2(new int[] { 5 });
    EXPECT_EQ(AnotherCustomArrDeleter::i, 0);
    uptr2.~UniquePtr<int[], AnotherCustomArrDeleter>();
    EXPECT_EQ(uptr2.get(), nullptr);
    EXPECT_EQ(AnotherCustomArrDeleter::i, 1);
}

TEST(UniquePtrArrTest, Get)
{
    UniquePtr<int[]> uptr;
    EXPECT_EQ(uptr.get(), nullptr);

    uptr = makeUnique<int[]>(1, 5);
    EXPECT_EQ(*uptr.get(), 5);
}

TEST(UniquePtrArrTest, Release)
{
    UniquePtr<int[]> uptr(makeUnique<int[]>(1, 5));
    int* i = uptr.release();

    EXPECT_EQ(uptr.get(), nullptr);
    EXPECT_EQ(*i, 5);
    delete i;
}

TEST(UniquePtrArrTest, Reset)
{
    UniquePtr<int[]> uptr(makeUnique<int[]>(1, 5));
    UniquePtr<int[]> uptr2(makeUnique<int[]>(1, 10));
    uptr.reset(uptr2.release());

    EXPECT_EQ(*uptr.get(), 10);
}

TEST(UniquePtrArrTest, Swap)
{
    auto deleter1 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 1);
        delete[] p;
    };

    auto deleter2 = [](int* p)
    {
        EXPECT_EQ(*(int*)p, 0);
        delete[] p;
    };

    UniquePtr<int[], void(*)(int*)> uptr(new int[](5), deleter1);
    UniquePtr<int[], void(*)(int*)> uptr2(new int[](10), deleter2);
    uptr.swap(uptr2);

    EXPECT_EQ(*uptr.get(), 10);
    EXPECT_EQ(*uptr2.get(), 5);

    *uptr.get() = 0;
    *uptr2.get() = 1;

    uptr.reset(nullptr);
    uptr2.reset(nullptr);
}

TEST(UniquePtrArrTest, VariousOperators)
{
    UniquePtr<VirtDistructorOwner[]> uptr(makeUnique<VirtDistructorOwner[]>(1, 5));

    EXPECT_EQ(uptr->i, 5);
    EXPECT_EQ((*uptr).i, 5);
    EXPECT_TRUE(static_cast<bool>(uptr));

    uptr.reset();

    EXPECT_FALSE(static_cast<bool>(uptr));
}

} // namespace