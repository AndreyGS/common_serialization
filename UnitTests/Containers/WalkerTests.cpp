#include <gtest/gtest.h>
#include "special_types.h"
#include "Containers/Walker.h"
#include "Containers/Vector.h"
#include "string"
#include "list"

using namespace special_types;
using namespace common_serialization;

namespace
{

// Explicitly set default allocator helper so we wouldn't depend on 
// possible future changes of default allocator helper of Vector
template<typename T>
using DefaultWalkerAllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T>>;

using size_type = typename Walker<int, DefaultWalkerAllocatorHelper<int>>::size_type;

template<typename T>
T g_data_array[] = { "123", "456", "789" };

template<typename T>
auto getStringsFilledContainer()
{
    static Walker<T, DefaultWalkerAllocatorHelper<T>> walker;

    if (walker.size() == 0)
        walker.pushBackN(g_data_array<T>, 3);

    EXPECT_EQ(walker.capacity(), 6); // check that nothing is changed in allocation strategy

    return walker;
}

template<>
auto getStringsFilledContainer<PodStruct>()
{
    static Walker<PodStruct, StrategicAllocatorHelper<PodStruct, RawNoexceptAllocator<PodStruct>>> walker;

    if (walker.size() == 0)
        walker.pushBackN(g_data_array<PodStruct>, 3);

    EXPECT_EQ(walker.capacity(), 6); // check that nothing is changed in allocation strategy

    return walker;
}

TEST(WalkerTest, Constructor)
{
    Walker<int, DefaultWalkerAllocatorHelper<int>> walker;

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

TEST(WalkerTest, Reserve)
{
    Walker<int, DefaultWalkerAllocatorHelper<int>> walker;
    walker.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::strictByDataSize);

    int i = 1;
    walker.pushBack(i);
    EXPECT_EQ(walker.capacity(), 1);

    bool b = walker.reserve(100);
    EXPECT_TRUE(b);
    EXPECT_EQ(walker.capacity(), 100);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);

    // test when memory couldn't be allocated
    b = walker.reserve(static_cast<size_type>(-1));
    EXPECT_FALSE(b);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(walker.capacity(), 100);
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);
}

TEST(WalkerTest, ReserveFromCurrentOffset)
{
    Walker<int, DefaultWalkerAllocatorHelper<int>> walker;
    walker.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::strictByDataSize);

    int i = 1;
    walker.pushBack(i);
    EXPECT_EQ(walker.capacity(), 1);

    bool b = walker.reserve_from_current_offset(100);
    EXPECT_TRUE(b);
    EXPECT_EQ(walker.capacity(), 101);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);

    // test when memory couldn't be allocated
    b = walker.reserve(static_cast<size_type>(-1));
    EXPECT_FALSE(b);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(walker.capacity(), 101);
    EXPECT_EQ(walker.size(), 1);
    EXPECT_EQ(walker[0], i);
}

TEST(WalkerTest, PushBack)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    // test l-value
    std::string str("123");
    auto n = walker.pushBack(str);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker[0], "123");
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(str.size(), 3);

    // test r-value
    n = walker.pushBack(std::move(str));
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker[1], "123");
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(str.size(), 0);
}

TEST(WalkerTest, PushBackNoMove)
{
    Walker<NoMoveConstructible, DefaultWalkerAllocatorHelper<NoMoveConstructible>> walker;

    // test l-value
    NoMoveConstructible str("123");
    auto n = walker.pushBack(str);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker[0], "123");
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(str.size, 3);

    // test r-value
    n = walker.pushBack(std::move(str));
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker[1], "123");
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(str.size, 3);
    EXPECT_TRUE(str.p != nullptr);
}

TEST(WalkerTest, PushBackPod)
{
    Walker<PodStruct, StrategicAllocatorHelper<PodStruct, RawNoexceptAllocator<PodStruct>>> walker_pod;

    // test l-value
    auto n = walker_pod.pushBack("123");
    EXPECT_EQ(n, 1);
    EXPECT_EQ(memcmp(walker_pod.data(), "123", sizeof(PodStruct)), 0);
    EXPECT_EQ(walker_pod.tell(), 1);

    // test r-value
    PodStruct ps("456");
    n = walker_pod.pushBack(std::move(ps));
    EXPECT_EQ(n, 1);
    EXPECT_EQ(memcmp(walker_pod.data() + 1, "456", sizeof(PodStruct)), 0);
    EXPECT_EQ(walker_pod.tell(), 2);
}

template<typename T>
void FPushBackN()
{
    auto walker = getStringsFilledContainer<T>();

    EXPECT_EQ(walker.size(), 3);

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    auto n = walker.pushBackN(g_data_array<T>, 3);
    EXPECT_EQ(n, 3);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    for (size_type i = 0; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i % 3]);

    // test with additional memory allocation
    T another_data_array[] = { "abc", "def", "ghi" };
    walker.pushBackN(another_data_array, 3);
    EXPECT_EQ(walker.size(), 9);

    for (size_type i = 0; i < 6; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i % 3]);

    for (size_type i = 6; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], another_data_array[i - 6]);

    // not valid data or data size tests
    walker.pushBackN(nullptr, 3);
    EXPECT_EQ(walker.size(), 9);

    walker.pushBackN(g_data_array<T>, static_cast<size_type>(-1));
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
}

template<typename T>
void FReplace()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    auto n = walker.replace(5, g_data_array<T>, 3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    n = walker.replace(1, another_data_array, 1);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 3);

    walker.pushBackN(another_data_array, 3);

    // test with additional memory allocation
    n = walker.replace(5, &another_data_array[1], 2);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(walker.tell(), 7);
    EXPECT_EQ(walker.size(), 7);

    for (size_type i = 5; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], another_data_array[i - 4]);
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
}

template<typename T>
void FInsert()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    auto n = walker.insert(5, g_data_array<T>, 3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    n = walker.insert(0, another_data_array, 3);
    EXPECT_EQ(n, 3);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    for (size_type i = 3; i < walker.size(); ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };

    n = walker.insert(2, another_data_array2, 3);
    EXPECT_EQ(n, 3);
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

    n = walker.insert(8, another_data_array3, 3);
    EXPECT_EQ(n, 3);
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

    // test not valid values
    n = walker.insert(13, another_data_array3, 3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 12);
    EXPECT_EQ(walker.size(), 12);

    n = walker.insert(10, nullptr, 3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 10);
    EXPECT_EQ(walker.size(), 12);

    n = walker.insert(10, another_data_array3, 0);
    EXPECT_EQ(n, 0);
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
}

template<typename T>
void FInsertIt()
{
    auto walker = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    auto n = walker.insert(walker.getVector().begin() + 5, g_data_array<T>, g_data_array<T> +3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 3);
    EXPECT_EQ(walker.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    std::list<T> l1;
    l1.insert(l1.begin(), another_data_array, another_data_array + 3);

    n = walker.insert(walker.getVector().begin(), l1.begin(), l1.end());
    EXPECT_EQ(n, 3);
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

    n = walker.insert(walker.getVector().begin() + 2, l2.begin(), l2.end());
    EXPECT_EQ(n, 3);
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

    n = walker.insert(walker.getVector().begin() + 8, l3.begin(), l3.end());
    EXPECT_EQ(n, 3);
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

    // try to insert by not valid iterator
    n = walker.insert(walker.getVector().end(), l3.begin(), l3.end());
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 12);
    EXPECT_EQ(walker.size(), 12);

    n = walker.insert(walker.getVector().begin() - 1, l3.begin(), l3.end());
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 12);
    EXPECT_EQ(walker.size(), 12);

    n = walker.insert(walker.getVector().end() + 1, l3.begin(), l3.end());
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 12);
    EXPECT_EQ(walker.size(), 12);
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
}

template<typename T>
void FErase()
{
    auto walker = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    walker.pushBackN(another_data_array, 3);

    EXPECT_EQ(walker.size(), 6);

    // try to erase by not valid offset
    auto n = walker.erase(walker.size() + 1, 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    // try to erase 0 elements
    n = walker.erase(1, 0);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    n = walker.erase(walker.size(), 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    n = walker.erase(0, 0);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 0);
    EXPECT_EQ(walker.size(), 6);

    // main usage
    n = walker.erase(4, 1);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 4);
    EXPECT_EQ(walker.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    EXPECT_EQ(walker[3], another_data_array[0]);
    EXPECT_EQ(walker[4], another_data_array[2]);

    n = walker.erase(1, 2);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 3);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    n = walker.erase(2, 1);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 2);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);

    // test that only right range is cut after last element
    n = walker.erase(1, 10);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 1);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);

    // test extra big n
    n = walker.erase(0, static_cast<size_type>(-1));
    EXPECT_EQ(n, 1);
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
    auto n = walker.erase(walker.getVector().begin() - 1, walker.getVector().begin() + 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    n = walker.erase(walker.getVector().begin(), walker.getVector().begin() - 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    n = walker.erase(walker.getVector().begin() + 1, walker.getVector().begin());
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    // try to erase 0 elements
    n = walker.erase(walker.getVector().end(), walker.getVector().end());
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 6);
    EXPECT_EQ(walker.size(), 6);

    n = walker.erase(walker.getVector().begin() + 1, walker.getVector().begin() + 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 6);

    // main usage
    n = walker.erase(walker.getVector().begin() + 4, walker.getVector().begin() + 5);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 4);
    EXPECT_EQ(walker.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], g_data_array<T>[i]);

    EXPECT_EQ(walker[3], another_data_array[0]);
    EXPECT_EQ(walker[4], another_data_array[2]);

    n = walker.erase(walker.getVector().begin() + 1, walker.getVector().begin() + 3);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(walker.tell(), 1);
    EXPECT_EQ(walker.size(), 3);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    n = walker.erase(walker.getVector().begin() + 2, walker.getVector().end());
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 2);

    EXPECT_EQ(walker[0], g_data_array<T>[0]);
    EXPECT_EQ(walker[1], another_data_array[0]);

    // test that only right range is cut after end()
    n = walker.erase(walker.getVector().begin() + 1, walker.getVector().end() + 10);
    EXPECT_EQ(n, 1);
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

    T another_data_array[] = { "abc", "def", "ghi" };
    walker.seek(1);
    auto n = walker.write(another_data_array, 1);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(walker.size(), 3);

    walker.pushBackN(another_data_array, 3);

    // test with additional memory allocation
    walker.seek(5);
    n = walker.write(&another_data_array[1], 2);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(walker.tell(), 7);
    EXPECT_EQ(walker.size(), 7);

    for (size_type i = 5; i < walker.size(); ++i)
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
}

template<typename T>
void FRead()
{
    auto walker = getStringsFilledContainer<T>();

    T another_data_array[3] = { T{}, T{}, T{} };
    
    walker.seek(0);
    auto n = walker.read(another_data_array, 3);
    EXPECT_EQ(n, 3);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    walker.seek(1);
    n = walker.read(another_data_array, 3);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(walker.tell(), 3);

    EXPECT_EQ(walker[1], another_data_array[0]);
    EXPECT_EQ(walker[2], another_data_array[1]);
    EXPECT_EQ(walker[2], another_data_array[2]);

    // copy more than walker has
    walker.seek(0);
    n = walker.read(another_data_array, 10);
    EXPECT_EQ(n, 3);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    // copy zero elements
    walker.seek(1);
    n = walker.read(another_data_array, 0);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 1);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    // try to copy with wrong offset
    walker.seek(3);
    n = walker.read(another_data_array, 1);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(walker[i], another_data_array[i]);

    // try to copy with nullptr as destination
    walker.seek(1);
    n = walker.read(nullptr, 3);
    EXPECT_EQ(n, 0);
    EXPECT_EQ(walker.tell(), 1);
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

TEST(WalkerTest, Data)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

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
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    EXPECT_EQ(walker.size(), 0);

    walker.pushBackN(g_data_array<std::string>, 3);

    EXPECT_EQ(walker.size(), 3);
}

TEST(WalkerTest, MaxSize)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    EXPECT_EQ(walker.max_size(), (ConstructorNoexceptAllocator<std::string>().max_size()));
}

TEST(WalkerTest, Capacity)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    EXPECT_EQ(walker.capacity(), 0);

    walker.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::strictByDataSize);

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

TEST(WalkerTest, GetAllocatorHelper)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    auto& allocator = walker.getAllocatorHelper();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(allocator)>, DefaultWalkerAllocatorHelper<std::string>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(allocator)>));
}

TEST(WalkerTest, GetVector)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;

    auto& vector = walker.getVector();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(vector)>, Vector<std::string, DefaultWalkerAllocatorHelper<std::string>>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(vector)>));
}

TEST(WalkerTest, Tell)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;
    EXPECT_EQ(walker.tell(), 0);

    walker.pushBackN(g_data_array<std::string>, 3);
    EXPECT_EQ(walker.tell(), 3);
}

TEST(WalkerTest, Seek)
{
    Walker<std::string, DefaultWalkerAllocatorHelper<std::string>> walker;
    walker.pushBackN(g_data_array<std::string>, 3);
    walker.seek(1);
    EXPECT_EQ(walker.tell(), 1);

    // try out of range;
    walker.seek(5);
    EXPECT_EQ(walker.tell(), 3);
}

TEST(WalkerTest, PushBackArithmeticValue)
{
    Walker<uint8_t, DefaultWalkerAllocatorHelper<uint8_t>> walker;
    walker.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::doubleOfDataSize); // as a precaution

    constexpr double value = 5.;

    walker.pushBackArithmeticValue(value);

    EXPECT_EQ(*reinterpret_cast<decltype(value)*>(walker.data()), value);
    EXPECT_EQ(walker.size(), sizeof(value));
    EXPECT_EQ(walker.capacity(), 2 * sizeof(value));
    EXPECT_EQ(walker.tell(), sizeof(value));
}

TEST(WalkerTest, ReadArithmeticValue)
{
    Walker<std::uint8_t, DefaultWalkerAllocatorHelper<std::uint8_t>> walker;
    walker.pushBack(1);
    walker.pushBack(2);
    walker.seek(0);

    constexpr short test = 1 | (2 << 8);
    short value = 0;

    EXPECT_EQ(walker.readArithmeticValue(value), sizeof(value));
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(value, test);

    value = 0;

    EXPECT_EQ(walker.readArithmeticValue(value), 0);
    EXPECT_EQ(walker.tell(), 2);
    EXPECT_EQ(value, 0);
}

} // namespace anonymous
