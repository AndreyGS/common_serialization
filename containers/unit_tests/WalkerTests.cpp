/**
 * @file WalkerTests.cpp
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
#include <tests_special_types/special_types.h>

namespace
{

using namespace special_types;
using namespace common_serialization;

template<typename T>
using DefaultAllocationManager = CtorStratAllocationManagerT<T>;

using size_type = typename Walker<int, DefaultAllocationManager<int>>::size_type;

template<typename T>
T g_data_array[] = { "123", "456", "789" };

template<typename T>
auto getStringsFilledContainer()
{
    Walker<T, DefaultAllocationManager<T>> walker;
    walker.pushBackN(g_data_array<T>, 3);

    EXPECT_EQ(walker.capacity(), 6); // check that nothing is changed in allocation strategy

    return walker;
}

template<>
auto getStringsFilledContainer<PodStruct>()
{
    Walker<PodStruct, RawStratAllocationManagerT<PodStruct>> walker;
    walker.pushBackN(g_data_array<PodStruct>, 3);

    EXPECT_EQ(walker.capacity(), 6); // check that nothing is changed in allocation strategy

    return walker;
}

TEST(WalkerTest, Constructor)
{
    Walker<int, DefaultAllocationManager<int>> walker;

    EXPECT_EQ(walker.size(), 0);
    EXPECT_EQ(walker.capacity(), 0);
    EXPECT_EQ(walker.data(), nullptr);
    EXPECT_EQ(walker.tell(), 0);
}

template<typename T>
void FCopyConstructor()
{
    auto walker1 = getStringsFilledContainer<T>();
    walker1.seek(1);
    auto walker2(walker1);

    EXPECT_EQ(walker1.size(), walker2.size());
    EXPECT_EQ(walker2.tell(), 1);
    EXPECT_EQ(walker1.tell(), walker2.tell());

    for (size_type i = 0; i < walker1.size(); ++i)
        EXPECT_EQ(walker1[i], walker2[i]);
}

TEST(WalkerTest, CopyConstructor)
{
    FCopyConstructor<std::string>();
}

TEST(WalkerTest, CopyConstructorNoMove)
{
    FCopyConstructor<NoMoveConstructible>();
}

TEST(WalkerTest, CopyConstructorPod)
{
    FCopyConstructor<PodStruct>();
}

template<typename T>
void FMoveConstructor()
{
    auto walker1 = getStringsFilledContainer<T>();
    walker1.seek(1);
    auto walker2(std::move(walker1));

    EXPECT_EQ(walker1.size(), 0);
    EXPECT_EQ(walker1.capacity(), 0);
    EXPECT_EQ(walker1.data(), nullptr);

    EXPECT_EQ(walker2.size(), 3);
    EXPECT_EQ(walker2.tell(), 1);
    for (size_type i = 0; i < walker2.size(); ++i)
        EXPECT_EQ(walker2[i], g_data_array<T>[i]);
}

TEST(WalkerTest, MoveConstructor)
{
    FMoveConstructor<std::string>();
}

TEST(WalkerTest, MoveConstructorNoMove)
{
    FMoveConstructor<NoMoveConstructible>();
}

TEST(WalkerTest, MoveConstructorPod)
{
    FMoveConstructor<PodStruct>();
}

template<typename T>
void FAssignmentCopyOperator()
{
    auto walker1 = getStringsFilledContainer<T>();
    walker1.seek(1);
    auto walker2 = walker1;

    EXPECT_EQ(walker1.size(), walker2.size());
    EXPECT_EQ(walker2.tell(), 1);
    EXPECT_EQ(walker1.tell(), walker2.tell());

    for (size_type i = 0; i < walker1.size(); ++i)
        EXPECT_EQ(walker1[i], walker2[i]);
}

TEST(WalkerTest, AssignmentCopyOperator)
{
    FAssignmentCopyOperator<std::string>();
}

TEST(WalkerTest, AssignmentCopyOperatorNoMove)
{
    FAssignmentCopyOperator<NoMoveConstructible>();
}

TEST(WalkerTest, AssignmentCopyOperatorPod)
{
    FAssignmentCopyOperator<PodStruct>();
}

template<typename T>
void FAssignmentMoveOperator()
{
    auto walker1 = getStringsFilledContainer<T>();
    walker1.seek(1);
    auto walker2 = std::move(walker1);

    EXPECT_EQ(walker1.size(), 0);
    EXPECT_EQ(walker1.capacity(), 0);
    EXPECT_EQ(walker1.data(), nullptr);

    EXPECT_EQ(walker2.size(), 3);
    EXPECT_EQ(walker2.tell(), 1);
    for (size_type i = 0; i < walker2.size(); ++i)
        EXPECT_EQ(walker2[i], g_data_array<T>[i]);
}

TEST(WalkerTest, AssignmentMoveOperator)
{
    FAssignmentMoveOperator<std::string>();
}

TEST(WalkerTest, AssignmentMoveOperatorNoMove)
{
    FAssignmentMoveOperator<NoMoveConstructible>();
}

TEST(WalkerTest, AssignmentMoveOperatorPod)
{
    FAssignmentMoveOperator<PodStruct>();
}

TEST(WalkerTest, Destructor)
{
    auto vec = getStringsFilledContainer<std::string>();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_TRUE(vec.capacity() >= 3);
    EXPECT_TRUE(vec.data() != nullptr);
    EXPECT_EQ(vec.tell(), 3);

    vec.~Walker();

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.data(), nullptr);
}

template<typename T>
void FInit()
{
    auto walker1 = getStringsFilledContainer<T>();
    decltype(walker1) walker2;

    EXPECT_EQ(walker2.init(walker1), Status::NoError);

    EXPECT_EQ(walker1.size(), walker2.size());
    EXPECT_EQ(walker1.tell(), walker2.tell());

    for (size_type i = 0; i < walker1.size(); ++i)
        EXPECT_EQ(walker1[i], walker2[i]);

    walker1.invalidate();

    // init by empty Vector
    EXPECT_EQ(walker2.init(walker1), Status::NoError);
    EXPECT_EQ(walker2.size(), 0);
    EXPECT_EQ(walker2.tell(), 0);
}

TEST(WalkerTest, init)
{
    FInit<std::string>();
}

TEST(WalkerTest, InitNoMove)
{
    FInit<NoMoveConstructible>();
}

TEST(WalkerTest, InitPod)
{
    FInit<PodStruct>();
}

TEST(WalkerTest, InitNoMemory)
{
    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker1;
    PodStruct ps[1] = { { } };
    walker1.getAllocationManager().getAllocator().setStorage(ps, 1);
    walker1.pushBack("123");

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker2;
    EXPECT_EQ(walker2.init(walker1), Status::ErrorNoMemory);
}

TEST(WalkerTest, InitErrorPropagation)
{
    Walker<ErrorProne, DefaultAllocationManager<ErrorProne>> walker1;
    ErrorProne::errorOnCounter = 100;
    walker1.pushBack(ErrorProne{});

    Walker<ErrorProne, DefaultAllocationManager<ErrorProne>> walker2;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorOverflow;

    EXPECT_EQ(walker2.init(walker1), Status::ErrorOverflow);
}

template<typename T>
void FInitMove()
{
    auto walker1 = getStringsFilledContainer<T>();
    decltype(walker1) walker2;

    EXPECT_EQ(walker2.init(std::move(walker1)), Status::NoError);

    EXPECT_EQ(walker1.size(), 0);
    EXPECT_EQ(walker1.capacity(), 0);
    EXPECT_EQ(walker1.data(), nullptr);

    EXPECT_EQ(walker2.size(), 3);
    EXPECT_EQ(walker2.tell(), 3);

    for (size_type i = 0; i < walker2.size(); ++i)
        EXPECT_EQ(walker2[i], g_data_array<T>[i]);

    // init by empty Vector
    EXPECT_EQ(walker2.init(std::move(walker1)), Status::NoError);
    EXPECT_EQ(walker2.size(), 0);
    EXPECT_EQ(walker2.tell(), 0);
}

TEST(WalkerTest, InitMove)
{
    FInitMove<std::string>();
}

TEST(WalkerTest, InitMoveNoMove)
{
    FInitMove<NoMoveConstructible>();
}

TEST(WalkerTest, InitMovePod)
{
    FInitMove<PodStruct>();

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker1;
    PodStruct ps[1] = { { } };
    walker1.getAllocationManager().getAllocator().setStorage(ps, 1);
    walker1.pushBack("123");

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker2;
    EXPECT_EQ(walker2.init(std::move(walker1)), Status::NoError);
}

TEST(WalkerTest, SetSize)
{
    uint8_t buffer[32]{ 0 };
    Walker<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> walker;
    walker.getAllocationManager().getAllocator().setStorage(buffer, 32);
    EXPECT_EQ(walker.tell(), 0);

    EXPECT_EQ(walker.setSize(32), Status::NoError);
    EXPECT_EQ(walker.size(), 32);
    EXPECT_EQ(walker.tell(), 32);
    EXPECT_EQ(walker.setSize(33), Status::ErrorNoMemory);
    EXPECT_EQ(walker.size(), 32);
    EXPECT_EQ(walker.tell(), 32);
}

TEST(WalkerTest, Reserve)
{
    Walker<int, DefaultAllocationManager<int>> walker;
    walker.getAllocationManager().setAllocationStrategy(AllocationStrategy::StrictByDataSize);

    int i = 1;
    walker.pushBack(i);
    EXPECT_EQ(walker.capacity(), 1);

    EXPECT_EQ(walker.reserve(100), Status::NoError);
    EXPECT_EQ(walker.capacity(), 100);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);

    // test when memory couldn't be allocated
    EXPECT_EQ(walker.reserve(static_cast<size_type>(-1)), Status::ErrorNoMemory);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(walker.capacity(), 100);
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);
}

TEST(WalkerTest, ReserveFromCurrentOffset)
{
    Walker<int, DefaultAllocationManager<int>> walker;
    walker.getAllocationManager().setAllocationStrategy(AllocationStrategy::StrictByDataSize);

    int i = 1;
    walker.pushBack(i);
    EXPECT_EQ(walker.capacity(), 1);

    EXPECT_EQ(walker.reserve_from_current_offset(100), Status::NoError);
    EXPECT_EQ(walker.capacity(), 101);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);

    // test when memory couldn't be allocated
    walker.seek(1);
    EXPECT_EQ(walker.reserve_from_current_offset(static_cast<size_type>(-1)), Status::ErrorOverflow);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(walker.capacity(), 101);
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);
}

TEST(WalkerTest, PushBack)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;
    std::string str("123");

    // test l-value
    EXPECT_EQ(walker.pushBack(str), Status::NoError);
    EXPECT_EQ(walker[0], "123");
    EXPECT_EQ(walker.tell(), 0);
    EXPECT_EQ(str.size(), 3);

    // test r-value
    EXPECT_EQ(walker.pushBack(std::move(str)), Status::NoError);
    EXPECT_EQ(walker[1], "123");
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(str.size(), 0);
}

TEST(WalkerTest, PushBackNoMove)
{
    Walker<NoMoveConstructible, DefaultAllocationManager<NoMoveConstructible>> walker;

    // test l-value
    NoMoveConstructible str("123");
    EXPECT_EQ(walker.pushBack(str), Status::NoError);
    EXPECT_EQ(walker[0], "123");
    EXPECT_EQ(walker.tell(), 0);
    EXPECT_EQ(str.m_str.size(), 3);

    // test r-value
    EXPECT_EQ(walker.pushBack(std::move(str)), Status::NoError);
    EXPECT_EQ(walker[1], "123");
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(str.m_str.size(), 3);
}

TEST(WalkerTest, PushBackPod)
{
    Walker<PodStruct, RawStratAllocationManagerT<PodStruct>> walker_pod;

    // test l-value
    EXPECT_EQ(walker_pod.pushBack("123"), Status::NoError);
    EXPECT_EQ(memcmp(walker_pod.data(), "123", sizeof(PodStruct)), 0);
    EXPECT_EQ(walker_pod.tell(), 1);

    // test r-value
    PodStruct ps("456");
    EXPECT_EQ(walker_pod.pushBack(std::move(ps)), Status::NoError);
    EXPECT_EQ(memcmp(walker_pod.data() + 1, "456", sizeof(PodStruct)), 0);
    EXPECT_EQ(walker_pod.tell(), 2);

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker_pod2;
    walker_pod2.getAllocationManager().getAllocator().setStorage(&ps, 1);
    walker_pod2.pushBack(ps);
    walker_pod2.seek(0);

    // test l-value
    EXPECT_EQ(walker_pod2.pushBack(ps), Status::ErrorNoMemory);
    EXPECT_EQ(walker_pod2.size(), 1);
    EXPECT_EQ(walker_pod2.tell(), 1);

    // test r-value
    EXPECT_EQ(walker_pod2.pushBack(std::move(ps)), Status::ErrorNoMemory);
    EXPECT_EQ(walker_pod2.size(), 1);
    EXPECT_EQ(walker_pod2.tell(), 1);
}

template<typename T>
void FPushBackN()
{
    auto walker = getStringsFilledContainer<T>();

    EXPECT_EQ(walker.size(), 3);

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    EXPECT_EQ(walker.pushBackN(g_data_array<T>, 3), Status::NoError);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i % 3]);

    // test with additional memory allocation
    T another_data_array[] = { "abc", "def", "ghi" };
    EXPECT_EQ(walker.pushBackN(another_data_array, 3), Status::NoError);
    EXPECT_EQ(walker.size(), 9);

    for (size_type i = 0; i < 6; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i % 3]);

    for (size_type i = 6; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], another_data_array[i - 6]);

    // not valid data or data size tests
    EXPECT_EQ(walker.pushBackN(nullptr, 3), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.size(), 9);

    EXPECT_EQ(walker.pushBackN(g_data_array<T>, static_cast<size_type>(-1)), Status::ErrorOverflow);
    EXPECT_EQ(walker.size(), 9);
}

TEST(WalkerTest, PushBackN)
{
    FPushBackN<std::string>();
}

TEST(WalkerTest, PushBackNNoMove)
{
    FPushBackN<NoMoveConstructible>();
}

TEST(WalkerTest, PushBackNPod)
{
    FPushBackN<PodStruct>();

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker_pod;
    PodStruct ps("456");
    walker_pod.getAllocationManager().getAllocator().setStorage(&ps, 1);
    walker_pod.pushBack(ps);
    walker_pod.seek(0);

    EXPECT_EQ(walker_pod.pushBackN(&ps, 1), Status::ErrorNoMemory);
    EXPECT_EQ(walker_pod.size(), 1);
    EXPECT_EQ(walker_pod.tell(), 1);
}

TEST(WalkerTest, PushBackArithmeticValue)
{
    Walker<uint8_t, DefaultAllocationManager<uint8_t>> walker;
    walker.getAllocationManager().setAllocationStrategy(AllocationStrategy::DoubleOfDataSize); // as a precaution

    double value = 5.;

    walker.pushBackArithmeticValue(value);

    EXPECT_EQ(*reinterpret_cast<decltype(value)*>(walker.data()), value);
    EXPECT_EQ(walker.size(), sizeof(value));
    EXPECT_EQ(walker.capacity(), 2 * sizeof(value));
    EXPECT_EQ(walker.tell(), sizeof(value));

    Walker<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> walker2;
    uint8_t storage[sizeof(double)]{ 0 };
    walker2.getAllocationManager().getAllocator().setStorage(storage, 1);
    walker2.pushBack(value);
    walker2.seek(0);

    EXPECT_EQ(walker2.pushBackArithmeticValue(value), Status::ErrorNoMemory);
    EXPECT_EQ(walker2.size(), 1);
    EXPECT_EQ(walker2.tell(), 1);
}

template<typename T>
void FReplace()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    EXPECT_EQ(walker.replace(g_data_array<T>, 3, 4), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    // check for invalid arguments
    EXPECT_EQ(walker.replace(nullptr, 3, 4), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    // check for valid nullptr
    EXPECT_EQ(walker.replace(nullptr, 0, 3), Status::NoError);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    // check for invalid arguments
    EXPECT_EQ(walker.replace(nullptr, 3, 4), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    EXPECT_EQ(walker.replace(another_data_array, 1, 1), Status::NoError);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 3);

    EXPECT_EQ(walker.pushBackN(another_data_array, 3), Status::NoError);

    // test with additional memory allocation
    EXPECT_EQ(walker.replace(&another_data_array[1], 2, 5), Status::NoError);
    EXPECT_EQ(walker.tell(), 7);
    EXPECT_EQ(walker.size(), 7);

    for (size_type i = 5; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], another_data_array[i - 4]);

    // test on offest == size
    EXPECT_EQ(walker.replace(&another_data_array[1], 2, 7), Status::NoError);
    EXPECT_EQ(walker.size(), 9);
    EXPECT_EQ(walker.tell(), 9);
}

TEST(WalkerTest, Replace)
{
    FReplace<std::string>();
}

TEST(WalkerTest, ReplaceNoMove)
{
    FReplace<NoMoveConstructible>();
}

TEST(WalkerTest, ReplacePod)
{
    FReplace<PodStruct>();

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker;

    PodStruct psArr[1] = { "456" };
    walker.getAllocationManager().getAllocator().setStorage(psArr, sizeof(psArr) / sizeof(PodStruct));
    walker.pushBack(psArr[0]);

    EXPECT_EQ(walker.replace(psArr, 2, 0), Status::ErrorNoMemory);
    EXPECT_EQ(walker.tell(), 0);
}

template<typename T>
void FInsert()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    EXPECT_EQ(walker.insert(g_data_array<T>, 3, 5), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    size_type newOffset = 0;
    EXPECT_EQ(walker.insert(another_data_array, 3, 0), Status::NoError);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 3; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };

    EXPECT_EQ(walker.insert(another_data_array2, 3, 2), Status::NoError);
    EXPECT_EQ(walker.tell(), 5);
    EXPECT_EQ(walker.size(), 9);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(walker[i], another_data_array2[i - 2]);

    EXPECT_EQ(walker[5], another_data_array[2]);

    for (size_type i = 6; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };

    EXPECT_EQ(walker.insert(another_data_array3, 3, 8), Status::NoError);
    EXPECT_EQ(walker.tell(), 11);
    EXPECT_EQ(walker.size(), 12);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(walker[i], another_data_array2[i - 2]);

    EXPECT_EQ(walker[5], another_data_array[2]);

    for (size_type i = 6; i < 8; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 6]);

    for (size_type i = 8; i < 11; ++i)
        EXPECT_EQ(walker[i], another_data_array3[i - 8]);

    EXPECT_EQ(walker[11], g_data_array<T>[2]);

    // test zero items
    EXPECT_EQ(walker.insert(another_data_array3, 0, 10), Status::NoError);
    EXPECT_EQ(walker.tell(), 10);
    EXPECT_EQ(walker.size(), 12);

    EXPECT_EQ(walker.insert(nullptr, 0, 10), Status::NoError);
    EXPECT_EQ(walker.tell(), 10);
    EXPECT_EQ(walker.size(), 12);

    // test not valid values
    EXPECT_EQ(walker.insert(another_data_array3, 3, 13), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 12);
    EXPECT_EQ(walker.size(), 12);

    EXPECT_EQ(walker.insert(nullptr, 3, 10), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.tell(), 10);
    EXPECT_EQ(walker.size(), 12);
}

TEST(WalkerTest, Insert)
{
    FInsert<std::string>();
}

TEST(WalkerTest, InsertNoMove)
{
    FInsert<NoMoveConstructible>();
}

TEST(WalkerTest, InsertPod)
{
    FInsert<PodStruct>();

    Walker<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> walker;
    uint8_t i = 1;

    EXPECT_EQ(walker.insert(&i, 1, 0), Status::ErrorNoMemory);
    EXPECT_EQ(walker.size(), 0);
    EXPECT_EQ(walker.tell(), 0);
}

template<typename T>
void FInsertIt()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    EXPECT_EQ(walker.insert(g_data_array<T>, g_data_array<T> +3, walker.getVector().begin() + 5), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    std::list<T> l1;
    l1.insert(l1.begin(), another_data_array, another_data_array + 3);

    auto it = walker.getVector().end();
    EXPECT_EQ(walker.insert(l1.begin(), l1.end(), walker.getVector().begin(), &it), Status::NoError);
    EXPECT_EQ(*it, *(walker.getVector().begin() + 3));
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 3; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };
    std::list<T> l2;
    l2.insert(l2.begin(), another_data_array2, another_data_array2 + 3);

    EXPECT_EQ(walker.insert(l2.begin(), l2.end(), walker.getVector().begin() + 2, &it), Status::NoError);
    EXPECT_EQ(*it, *(walker.getVector().begin() + 5));
    EXPECT_EQ(walker.tell(), 5);
    EXPECT_EQ(walker.size(), 9);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(walker[i], another_data_array2[i - 2]);

    EXPECT_EQ(walker[5], another_data_array[2]);

    for (size_type i = 6; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };
    std::list<T> l3;
    l3.insert(l3.begin(), another_data_array3, another_data_array3 + 3);

    EXPECT_EQ(walker.insert(l3.begin(), l3.end(), walker.getVector().begin() + 8, &it), Status::NoError);
    EXPECT_EQ(*it, *(walker.getVector().begin() + 11));
    EXPECT_EQ(walker.tell(), 11);
    EXPECT_EQ(walker.size(), 12);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(walker[i], another_data_array2[i - 2]);

    EXPECT_EQ(walker[5], another_data_array[2]);

    for (size_type i = 6; i < 8; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 6]);

    for (size_type i = 8; i < 11; ++i)
        EXPECT_EQ(walker[i], another_data_array3[i - 8]);

    EXPECT_EQ(walker[11], g_data_array<T>[2]);

    // try to insert to the end
    EXPECT_EQ(walker.insert(l3.begin(), l3.end(), walker.getVector().end(), &it), Status::NoError);
    EXPECT_EQ(it, walker.getVector().begin() + 15);
    EXPECT_EQ(walker.size(), 15);

    for (size_type i = 12; i < 15; ++i)
        EXPECT_EQ(walker[i], another_data_array3[i - 12]);

    // try to not pass optional arg
    EXPECT_EQ(walker.insert(l3.begin(), l3.begin(), walker.getVector().begin()), Status::NoError);

    it = walker.getVector().begin() + 14;

    // try to insert by not valid iterator
    EXPECT_EQ(walker.insert(l3.begin(), l3.end(), walker.getVector().begin() - 1, &it), Status::ErrorOverflow);
    EXPECT_EQ(it, walker.getVector().begin() + 14);
    EXPECT_EQ(walker.size(), 15);
    EXPECT_EQ(walker.tell(), 15);

    EXPECT_EQ(walker.insert(l3.begin(), l3.end(), walker.getVector().end() + 1, &it), Status::ErrorOverflow);
    EXPECT_EQ(it, walker.getVector().begin() + 14);
    EXPECT_EQ(walker.size(), 15);
    EXPECT_EQ(walker.tell(), 15);
}

TEST(WalkerTest, InsertIt)
{
    FInsertIt<std::string>();
}

TEST(WalkerTest, InsertItNoMove)
{
    FInsertIt<NoMoveConstructible>();
}

TEST(WalkerTest, InsertItPod)
{
    FInsertIt<PodStruct>();

    Walker<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> walker;
    uint8_t i = 0;
    walker.getAllocationManager().getAllocator().setStorage(&i, 1);
    walker.pushBack(i);

    std::list<uint8_t> l1;
    l1.push_back(i);
    EXPECT_EQ(walker.insert(l1.begin(), l1.end(), walker.getVector().begin()), Status::ErrorNoMemory);
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker.tell(), 0);
}

template<typename T>
void FErase()
{
    auto walker = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    walker.pushBackN(another_data_array, 3);

    EXPECT_EQ(walker.size(), 6);

    // try to erase by not valid offset
    EXPECT_EQ(walker.erase(walker.size(), 1), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    // try to erase 0 elements
    EXPECT_EQ(walker.erase(1, 0), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    EXPECT_EQ(walker.erase(1, 0), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    // main usage
    EXPECT_EQ(walker.erase(4, 1), Status::NoError);
    EXPECT_EQ(walker.tell(), 4);
    EXPECT_EQ(walker.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    EXPECT_EQ(walker[3], another_data_array[0]);
    EXPECT_EQ(walker[4], another_data_array[2]);

    EXPECT_EQ(walker.erase(1, 2), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 3);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    EXPECT_EQ(walker.erase(2, walker.size()), Status::NoError);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 2);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);

    // test that only right range is cut after last element
    EXPECT_EQ(walker.erase(1, 10), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 1);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);

    // test extra big n
    EXPECT_EQ(walker.erase(0, static_cast<size_type>(-1)), Status::NoError);
    EXPECT_EQ(walker.tell(), 0);
    EXPECT_EQ(walker.size(), 0);
}

TEST(WalkerTest, Erase)
{
    FErase<std::string>();
}

TEST(WalkerTest, EraseNoMove)
{
    FErase<NoMoveConstructible>();
}

TEST(WalkerTest, ErasePod)
{
    FErase<PodStruct>();
}

template<typename T>
void FEraseIt()
{
    auto walker = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    walker.pushBackN(another_data_array, 3);

    EXPECT_EQ(walker.size(), 6);

    // try to erase by not valid iterator
    EXPECT_EQ(walker.erase(walker.getVector().begin() - 1, walker.getVector().begin() + 1), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    EXPECT_EQ(walker.erase(walker.getVector().begin(), walker.getVector().begin() - 1), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 0);
    EXPECT_EQ(walker.size(), 6);

    EXPECT_EQ(walker.erase(walker.getVector().begin() + 1, walker.getVector().begin()), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    // try to erase 0 elements
    EXPECT_EQ(walker.erase(walker.getVector().end(), walker.getVector().end()), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    EXPECT_EQ(walker.erase(walker.getVector().begin() + 1, walker.getVector().begin() + 1), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    // main usage
    EXPECT_EQ(walker.erase(walker.getVector().begin() + 4, walker.getVector().begin() + 5), Status::NoError);
    EXPECT_EQ(walker.tell(), 4);
    EXPECT_EQ(walker.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    EXPECT_EQ(walker[3], another_data_array[0]);
    EXPECT_EQ(walker[4], another_data_array[2]);

    EXPECT_EQ(walker.erase(walker.getVector().begin() + 1, walker.getVector().begin() + 3), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 3);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    EXPECT_EQ(walker.erase(walker.getVector().begin() + 2, walker.getVector().end()), Status::NoError);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 2);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);

    // test that only right range is cut after end()
    EXPECT_EQ(walker.erase(walker.getVector().begin() + 1, walker.getVector().end() + 10), Status::NoError);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 1);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
}

TEST(WalkerTest, EraseIt)
{
    FEraseIt<std::string>();
}

TEST(WalkerTest, EraseItNoMove)
{
    FEraseIt<NoMoveConstructible>();
}

TEST(WalkerTest, EraseItPod)
{
    FEraseIt<PodStruct>();
}

template<typename T>
void FWrite()
{
    auto walker = getStringsFilledContainer<T>();

    // check for invalid arguments
    EXPECT_EQ(walker.write(nullptr, 3), Status::ErrorInvalidArgument);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    EXPECT_EQ(walker.write(another_data_array, 1), Status::NoError);
    EXPECT_EQ(walker.tell(), 4);
    EXPECT_EQ(walker.size(), 4);

    // test with additional memory allocation
    EXPECT_EQ(walker.write(another_data_array, 3), Status::NoError);
    EXPECT_EQ(walker.tell(), 7);
    EXPECT_EQ(walker.size(), 7);

    for (size_type i = 4; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], another_data_array[i - 4]);
}

TEST(WalkerTest, Write)
{
    FWrite<std::string>();
}

TEST(WalkerTest, WriteNoMove)
{
    FWrite<NoMoveConstructible>();
}

TEST(WalkerTest, WritePod)
{
    FWrite<PodStruct>();

    Walker<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> walker;

    PodStruct psArr{ "456" };

    EXPECT_EQ(walker.write(&psArr, 2), Status::ErrorNoMemory);
    EXPECT_EQ(walker.tell(), 0);
}

template<typename T>
void FRead()
{
    auto walker = getStringsFilledContainer<T>();

    T another_data_array[3] = { T{}, T{}, T{} };
    
    size_type nRead = 0;
    walker.seek(0);
    EXPECT_EQ(walker.read(another_data_array, 3, &nRead), Status::NoError);
    EXPECT_EQ(nRead, 3);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    nRead = 10;
    walker.seek(1);
    EXPECT_EQ(walker.read(another_data_array, 3, &nRead), Status::NoError);
    EXPECT_EQ(nRead, 2);
    EXPECT_EQ(walker.tell(), 3);

    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[1]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    // copy more than walker has
    nRead = 10;
    walker.seek(0);
    EXPECT_EQ(walker.read(another_data_array, 10, &nRead), Status::NoError);
    EXPECT_EQ(nRead, 3);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    // copy zero elements
    nRead = 10;
    walker.seek(1);
    EXPECT_EQ(walker.read(another_data_array, 0, &nRead), Status::NoError);
    EXPECT_EQ(nRead, 0);
    EXPECT_EQ(walker.tell(), 1);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    nRead = 10;
    EXPECT_EQ(walker.read(nullptr, 0, &nRead), Status::NoError);
    EXPECT_EQ(nRead, 0);
    EXPECT_EQ(walker.tell(), 2);

    // try to copy with wrong offset
    nRead = 10;
    walker.seek(3);
    EXPECT_EQ(walker.read(another_data_array, 1, &nRead), Status::ErrorOverflow);
    EXPECT_EQ(nRead, 10);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    // try to copy with nullptr as destination
    nRead = 10;
    walker.seek(1);
    EXPECT_EQ(walker.read(nullptr, 3, &nRead), Status::ErrorInvalidArgument);
    EXPECT_EQ(nRead, 10);
    EXPECT_EQ(walker.tell(), 1);

    // try to not pass optional arg
    nRead = 10;
    walker.seek(0);
    EXPECT_EQ(walker.read(another_data_array, 3), Status::NoError);
}

TEST(WalkerTest, Read)
{
    FRead<std::string>();
}

TEST(WalkerTest, ReadNoMove)
{
    FRead<NoMoveConstructible>();
}

TEST(WalkerTest, ReadPod)
{
    FRead<PodStruct>();
}

TEST(WalkerTest, ReadArithmeticValue)
{
    Walker<std::uint8_t, DefaultAllocationManager<std::uint8_t>> walker;
    walker.pushBack(1);
    walker.pushBack(2);
    walker.seek(0);

    constexpr short test = 1 | (2 << 8);
    short value = 0;

    EXPECT_EQ(walker.readArithmeticValue(value), Status::NoError);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(value, test);

    value = 0;

    EXPECT_EQ(walker.readArithmeticValue(value), Status::ErrorOverflow);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(value, 0);
}

TEST(WalkerTest, Data)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    EXPECT_EQ(walker.data(), nullptr);

    walker.pushBackN(g_data_array<std::string>, 3);

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(*(walker.data() + i), g_data_array<std::string>[i]);
}

TEST(VectorTest, OperatorAt)
{
    auto walker = getStringsFilledContainer<std::string>();

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<std::string>[i]);
}

TEST(WalkerTest, Size)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    EXPECT_EQ(walker.size(), 0);

    walker.pushBackN(g_data_array<std::string>, 3);

    EXPECT_EQ(walker.size(), 3);
}

TEST(WalkerTest, MaxSize)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    EXPECT_EQ(walker.max_size(), (ConstructorNoexceptAllocator<std::string>().max_size()));
}

TEST(WalkerTest, Capacity)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    EXPECT_EQ(walker.capacity(), 0);

    walker.getAllocationManager().setAllocationStrategy(AllocationStrategy::StrictByDataSize);

    walker.pushBackN(g_data_array<std::string>, 3);

    EXPECT_EQ(walker.capacity(), 3);
}

TEST(WalkerTest, Clear)
{
    auto walker = getStringsFilledContainer<std::string>();

    auto old_capacity = walker.capacity();

    walker.clear();

    EXPECT_NE(walker.data(), nullptr);
    EXPECT_EQ(walker.capacity(), old_capacity);
    EXPECT_EQ(walker.size(), 0);
    EXPECT_EQ(walker.tell(), 0);
}

TEST(WalkerTest, Invalidate)
{
    auto walker = getStringsFilledContainer<std::string>();

    walker.invalidate();

    EXPECT_EQ(walker.data(), nullptr);
    EXPECT_EQ(walker.capacity(), 0);
    EXPECT_EQ(walker.size(), 0);
    EXPECT_EQ(walker.tell(), 0);
}

TEST(WalkerTest, Release)
{
    auto walker = getStringsFilledContainer<std::string>();

    auto p1 = walker.data();
    auto p2 = walker.release();

    EXPECT_EQ(p1, p2);

    EXPECT_EQ(walker.data(), nullptr);
    EXPECT_EQ(walker.capacity(), 0);
    EXPECT_EQ(walker.size(), 0);
    EXPECT_EQ(walker.tell(), 0);
}

TEST(WalkerTest, GetAllocationManager)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    auto& allocator = walker.getAllocationManager();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(allocator)>, DefaultAllocationManager<std::string>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(allocator)>));
}

TEST(WalkerTest, GetVector)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;

    auto& vector = walker.getVector();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(vector)>, Vector<std::string, DefaultAllocationManager<std::string>>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(vector)>));
}

TEST(WalkerTest, Tell)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;
    EXPECT_EQ(walker.tell(), 0);

    walker.pushBackN(g_data_array<std::string>, 3);
    EXPECT_EQ(walker.tell(), 3);
}

TEST(WalkerTest, Seek)
{
    Walker<std::string, DefaultAllocationManager<std::string>> walker;
    walker.pushBackN(g_data_array<std::string>, 3);
    walker.seek(1);
    EXPECT_EQ(walker.tell(), 1);

    // try out of range;
    walker.seek(5);
    EXPECT_EQ(walker.tell(), 3);
}

} // namespace
