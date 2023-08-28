#include <gtest/gtest.h>
#include "Containers/Vector.h"
#include "string"
#include "list"

using namespace common_serialization;

namespace
{

// Explicitly set default allocator helper so we wouldn't depend on 
// possible future changes of default allocator helper of Vector
template<typename T>
using DefaultVectorAllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawHeapAllocator>>;

struct NoMoveConstructible
{
    char* p;
    size_t size;

    NoMoveConstructible(const char* in_p)
    {
        size = strlen(in_p);
        p = new char[size + 1];
        memcpy(p, in_p, size + 1);
    }

    NoMoveConstructible(const NoMoveConstructible& rhs)
    {
        p = new char[rhs.size + 1];
        memcpy(p, rhs.p, rhs.size + 1);
        size = rhs.size;
    }

    ~NoMoveConstructible()
    {
        if (p)
            delete[] p;
    }

    bool operator==(const NoMoveConstructible& rhs) const noexcept
    {
        if (p && rhs.p)
            return size == rhs.size && memcmp(p, rhs.p, size) == 0;
        else
            return false;
    }
};

struct PodStruct
{
    int i;
    PodStruct(const char* p) noexcept
    {
        i = *reinterpret_cast<const int*>(p);
    }
    bool operator ==(const PodStruct& rhs) const noexcept
    {
        return i == rhs.i;
    }
};

template<typename T>
T g_data_array[] = { "123", "456", "789" };

template<typename T>
auto getStringsFilledContainer()
{
    static Vector<T, DefaultVectorAllocatorHelper<T>> vbc;
    
    if (vbc.size() == 0)
        vbc.push_back_n(g_data_array<T>, 3);

    EXPECT_EQ(vbc.capacity(), 6); // check that nothing is changed in allocation strategy

    return vbc;
}

template<>
auto getStringsFilledContainer<PodStruct>()
{
    static Vector<PodStruct, StrategicAllocatorHelper<PodStruct, RawHeapAllocator>> vbc;

    if (vbc.size() == 0)
        vbc.push_back_n(g_data_array<PodStruct>, 3);

    EXPECT_EQ(vbc.capacity(), 6); // check that nothing is changed in allocation strategy

    return vbc;
}

} // namespace anonymous

TEST(VectorTest, Constructor)
{
    Vector<int, DefaultVectorAllocatorHelper<int>> vbc;

    EXPECT_EQ(vbc.size(), 0);
    EXPECT_EQ(vbc.capacity(), 0);
    EXPECT_EQ(vbc.data(), nullptr);
}

template<typename T>
void FCopyConstructor()
{
    auto vbc1 = getStringsFilledContainer<T>();
    auto vbc2(vbc1);

    EXPECT_EQ(vbc1.size(), vbc2.size());

    for (size_t i = 0; i < vbc1.size(); ++i)
        EXPECT_EQ(vbc1[i], vbc2[i]);
}

TEST(VectorTest, CopyConstructor)
{
    FCopyConstructor<std::string>();
}

TEST(VectorTest, CopyConstructorNoMove)
{
    FCopyConstructor<NoMoveConstructible>();
}

TEST(VectorTest, CopyConstructorPod)
{
    FCopyConstructor<PodStruct>();
}

template<typename T>
void FMoveConstructor()
{
    auto vbc1 = getStringsFilledContainer<T>();
    auto vbc2(std::move(vbc1));

    EXPECT_EQ(vbc1.size(), 0);
    EXPECT_EQ(vbc1.capacity(), 0);
    EXPECT_EQ(vbc1.data(), nullptr);

    EXPECT_EQ(vbc2.size(), 3);
    for (size_t i = 0; i < vbc2.size(); ++i)
        EXPECT_EQ(vbc2[i], g_data_array<T>[i]);
}

TEST(VectorTest, MoveConstructor)
{
    FMoveConstructor<std::string>();
}

TEST(VectorTest, MoveConstructorNoMove)
{
    FMoveConstructor<NoMoveConstructible>();
}

TEST(VectorTest, MoveConstructorPod)
{
    FMoveConstructor<PodStruct>();
}

template<typename T>
void FAssignmentCopyOperator()
{
    auto vbc1 = getStringsFilledContainer<T>();
    auto vbc2 = vbc1;

    EXPECT_EQ(vbc1.size(), vbc2.size());

    for (size_t i = 0; i < vbc1.size(); ++i)
        EXPECT_EQ(vbc1[i], vbc2[i]);
}

TEST(VectorTest, AssignmentCopyOperator)
{
    FAssignmentCopyOperator<std::string>();
}

TEST(VectorTest, AssignmentCopyOperatorNoMove)
{
    FAssignmentCopyOperator<NoMoveConstructible>();
}

TEST(VectorTest, AssignmentCopyOperatorPod)
{
    FAssignmentCopyOperator<PodStruct>();
}

template<typename T>
void FAssignmentMoveOperator()
{
    auto vbc1 = getStringsFilledContainer<T>();
    auto vbc2 = std::move(vbc1);

    EXPECT_EQ(vbc1.size(), 0);
    EXPECT_EQ(vbc1.capacity(), 0);
    EXPECT_EQ(vbc1.data(), nullptr);

    EXPECT_EQ(vbc2.size(), 3);
    for (size_t i = 0; i < vbc2.size(); ++i)
        EXPECT_EQ(vbc2[i], g_data_array<T>[i]);
}

TEST(VectorTest, AssignmentMoveOperator)
{
    FAssignmentMoveOperator<std::string>();
}

TEST(VectorTest, AssignmentMoveOperatorNoMove)
{
    FAssignmentMoveOperator<NoMoveConstructible>();
}

TEST(VectorTest, AssignmentMoveOperatorPod)
{
    FAssignmentMoveOperator<PodStruct>();
}

TEST(VectorTest, Reserve)
{
    Vector<int, DefaultVectorAllocatorHelper<int>> vbc;
    vbc.get_allocator_helper().setAllocationStrategy(AllocationStrategy::strictByDataSize);
    
    int i = 1;
    vbc.push_back(i);
    EXPECT_EQ(vbc.capacity(), 1);

    bool b = vbc.reserve(100);
    EXPECT_TRUE(b);
    EXPECT_EQ(vbc.capacity(), 100);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(vbc.size(), 1);
    EXPECT_EQ(vbc[0], i);

    // test when memory couldn't be allocated
    b = vbc.reserve(static_cast<size_t>(-1));
    EXPECT_FALSE(b);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(vbc.capacity(), 100);
    EXPECT_EQ(vbc.size(), 1);
    EXPECT_EQ(vbc[0], i);
}

TEST(VectorTest, PushBack)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    // test l-value
    std::string str("123");
    auto& vbc_ref1 = vbc.push_back(str);
    EXPECT_EQ(vbc[0], "123");
    EXPECT_EQ(&vbc_ref1, &vbc);
    EXPECT_EQ(str.size(), 3);

    // test r-value
    auto& vbc_ref2 = vbc.push_back(std::move(str));

    EXPECT_EQ(vbc[1], "123");
    EXPECT_EQ(&vbc_ref2, &vbc);
    EXPECT_EQ(str.size(), 0);

    EXPECT_EQ(&vbc_ref1, &vbc_ref2);
}

TEST(VectorTest, PushBackNoMove)
{
    Vector<NoMoveConstructible, DefaultVectorAllocatorHelper<NoMoveConstructible>> vbc;

    // test l-value
    NoMoveConstructible str("123");
    auto& vbc_ref1 = vbc.push_back(str);
    EXPECT_EQ(vbc[0], "123");
    EXPECT_EQ(&vbc_ref1, &vbc);
    EXPECT_EQ(str.size, 3);

    // test r-value
    auto& vbc_ref2 = vbc.push_back(std::move(str));

    EXPECT_EQ(vbc[1], "123");
    EXPECT_EQ(&vbc_ref2, &vbc);
    EXPECT_EQ(str.size, 3);
    EXPECT_TRUE(str.p != nullptr);

    EXPECT_EQ(&vbc_ref1, &vbc_ref2);
}

TEST(VectorTest, PushBackPod)
{
    Vector<PodStruct, StrategicAllocatorHelper<PodStruct, RawHeapAllocator>> vbc_pod;

    // test l-value
    auto& vbc_ref1 = vbc_pod.push_back("123");
    EXPECT_EQ(memcmp(vbc_pod.data(), "123", sizeof(PodStruct)), 0);
    EXPECT_EQ(&vbc_ref1, &vbc_pod);

    // test r-value
    PodStruct ps("456");
    auto& vbc_ref2 = vbc_pod.push_back(std::move(ps));

    EXPECT_EQ(memcmp(vbc_pod.data() + 1, "456", sizeof(PodStruct)), 0);
    EXPECT_EQ(&vbc_ref2, &vbc_pod);

    EXPECT_EQ(&vbc_ref1, &vbc_ref2);
}

template<typename T>
void FPushBackN()
{
    auto vbc = getStringsFilledContainer<T>();

    EXPECT_EQ(vbc.size(), 3);

    for (size_t i = 0; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i]);

    auto& vbc_ref = vbc.push_back_n(g_data_array<T>, 3);
    EXPECT_EQ(&vbc_ref, &vbc);
    EXPECT_EQ(vbc.size(), 6);

    for (size_t i = 0; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i % 3]);

    // test with additional memory allocation
    T another_data_array[] = { "abc", "def", "ghi" };
    vbc.push_back_n(another_data_array, 3);
    EXPECT_EQ(vbc.size(), 9);

    for (size_t i = 0; i < 6; ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i % 3]);

    for (size_t i = 6; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], another_data_array[i - 6]);

    // not valid data or data size tests
    vbc.push_back_n(nullptr, 3);
    EXPECT_EQ(vbc.size(), 9);

    vbc.push_back_n(g_data_array<T>, static_cast<size_t>(-1));
    EXPECT_EQ(vbc.size(), 9);
}

TEST(VectorTest, PushBackN)
{
    FPushBackN<std::string>();
}

TEST(VectorTest, PushBackNNoMove)
{
    FPushBackN<NoMoveConstructible>();
}

TEST(VectorTest, PushBackNPod)
{
    FPushBackN<PodStruct>();
}

template<typename T>
void FReplace()
{
    auto vbc = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    size_t newOffset = vbc.replace(5, g_data_array<T>, 3);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vbc.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    newOffset = vbc.replace(1, another_data_array, 1);

    EXPECT_EQ(newOffset, 2);
    EXPECT_EQ(vbc.size(), 3);

    vbc.push_back_n(another_data_array, 3);

    // test with additional memory allocation
    newOffset = vbc.replace(5, &another_data_array[1], 2);
    EXPECT_EQ(newOffset, 7);
    EXPECT_EQ(vbc.size(), 7);

    for (size_t i = 5; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], another_data_array[i - 4]);
}

TEST(VectorTest, Replace)
{
    FReplace<std::string>();
}

TEST(VectorTest, ReplaceNoMove)
{
    FReplace<NoMoveConstructible>();
}

TEST(VectorTest, ReplacePod)
{
    FReplace<PodStruct>();
}

template<typename T>
void FInsert()
{
    auto vbc = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    size_t newOffset = vbc.insert(5, g_data_array<T>, 3);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vbc.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    newOffset = vbc.insert(0, another_data_array, 3);

    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vbc.size(), 6);
    for (size_t i = 0; i < 3; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 3; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };

    newOffset = vbc.insert(2, another_data_array2, 3);
    EXPECT_EQ(newOffset, 5);
    EXPECT_EQ(vbc.size(), 9);

    for (size_t i = 0; i < 2; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 2; i < 5; ++i)
        EXPECT_EQ(vbc[i], another_data_array2[i - 2]);

    EXPECT_EQ(vbc[5], another_data_array[2]);

    for (size_t i = 6; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };

    newOffset = vbc.insert(8, another_data_array3, 3);

    EXPECT_EQ(newOffset, 11);
    EXPECT_EQ(vbc.size(), 12);

    for (size_t i = 0; i < 2; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 2; i < 5; ++i)
        EXPECT_EQ(vbc[i], another_data_array2[i - 2]);

    EXPECT_EQ(vbc[5], another_data_array[2]);

    for (size_t i = 6; i < 8; ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 6]);

    for (size_t i = 8; i < 11; ++i)
        EXPECT_EQ(vbc[i], another_data_array3[i - 8]);

    EXPECT_EQ(vbc[11], g_data_array<T>[2]);

    // test not valid values
    newOffset = vbc.insert(13, another_data_array3, 3);

    EXPECT_EQ(newOffset, 12);
    EXPECT_EQ(vbc.size(), 12);

    newOffset = vbc.insert(10, nullptr, 3);

    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vbc.size(), 12);

    newOffset = vbc.insert(10, another_data_array3, 0);

    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vbc.size(), 12);
}

TEST(VectorTest, Insert)
{
    FInsert<std::string>();
}

TEST(VectorTest, InsertNoMove)
{
    FInsert<NoMoveConstructible>();
}

TEST(VectorTest, InsertPod)
{
    FInsert<PodStruct>();
}

template<typename T>
void FInsertIt()
{
    auto vbc = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    auto it = vbc.insert(vbc.begin() + 5, g_data_array<T>, g_data_array<T> + 3);
    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    std::list<T> l1;
    l1.insert(l1.begin(), another_data_array, another_data_array + 3);

    it = vbc.insert(vbc.begin(), l1.begin(), l1.end());

    EXPECT_EQ(*it, *(vbc.begin() + 3));
    EXPECT_EQ(vbc.size(), 6);
    for (size_t i = 0; i < 3; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 3; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 3]);
    
    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };
    std::list<T> l2;
    l2.insert(l2.begin(), another_data_array2, another_data_array2 + 3);

    it = vbc.insert(vbc.begin() + 2, l2.begin(), l2.end());

    EXPECT_EQ(*it, *(vbc.begin() + 5));
    EXPECT_EQ(vbc.size(), 9);

    for (size_t i = 0; i < 2; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 2; i < 5; ++i)
        EXPECT_EQ(vbc[i], another_data_array2[i - 2]);

    EXPECT_EQ(vbc[5], another_data_array[2]);

    for (size_t i = 6; i < vbc.size(); ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };
    std::list<T> l3;
    l3.insert(l3.begin(), another_data_array3, another_data_array3 + 3);

    it = vbc.insert(vbc.begin() + 8, l3.begin(), l3.end());

    EXPECT_EQ(*it, *(vbc.begin() + 11));
    EXPECT_EQ(vbc.size(), 12);

    for (size_t i = 0; i < 2; ++i)
        EXPECT_EQ(vbc[i], another_data_array[i]);

    for (size_t i = 2; i < 5; ++i)
        EXPECT_EQ(vbc[i], another_data_array2[i - 2]);

    EXPECT_EQ(vbc[5], another_data_array[2]);

    for (size_t i = 6; i < 8; ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i - 6]);

    for (size_t i = 8; i < 11; ++i)
        EXPECT_EQ(vbc[i], another_data_array3[i - 8]);

    EXPECT_EQ(vbc[11], g_data_array<T>[2]);

    // try to insert by not valid iterator
    it = vbc.insert(vbc.end(), l3.begin(), l3.end());

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 12);

    it = vbc.insert(vbc.begin() - 1, l3.begin(), l3.end());

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 12);

    it = vbc.insert(vbc.end() + 1, l3.begin(), l3.end());

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 12);
}

TEST(VectorTest, InsertIt)
{
    FInsertIt<std::string>();
}

TEST(VectorTest, InsertItNoMove)
{
    FInsertIt<NoMoveConstructible>();
}

TEST(VectorTest, InsertItPod)
{
    FInsertIt<PodStruct>();
}

template<typename T>
void FErase()
{
    auto vbc = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vbc.push_back_n(another_data_array, 3);

    EXPECT_EQ(vbc.size(), 6);

    // try to erase by not valid offset
    auto newOffset = vbc.erase(vbc.size() + 1, 1);

    EXPECT_EQ(newOffset, vbc.size());
    EXPECT_EQ(vbc.size(), 6);

    // try to erase 0 elements
    newOffset = vbc.erase(1, 0);

    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vbc.size(), 6);

    newOffset = vbc.erase(vbc.size(), 1);

    EXPECT_EQ(newOffset, vbc.size());
    EXPECT_EQ(vbc.size(), 6);

    newOffset = vbc.erase(0, 0);

    EXPECT_EQ(newOffset, 0);
    EXPECT_EQ(vbc.size(), 6);

    // main usage
    newOffset = vbc.erase(4, 1);
    EXPECT_EQ(newOffset, 4);
    EXPECT_EQ(vbc.size(), 5);

    for (size_t i = 0; i < 3; ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i]);

    EXPECT_EQ(vbc[3], another_data_array[0]);
    EXPECT_EQ(vbc[4], another_data_array[2]);

    newOffset = vbc.erase(1, 2);
    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vbc.size(), 3);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
    EXPECT_EQ(vbc[1], another_data_array[0]);
    EXPECT_EQ(vbc[2], another_data_array[2]);

    newOffset = vbc.erase(2, vbc.size());
    EXPECT_EQ(newOffset, 2);
    EXPECT_EQ(vbc.size(), 2);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
    EXPECT_EQ(vbc[1], another_data_array[0]);

    // test that only right range is cut after last element
    newOffset = vbc.erase(1, 10);

    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vbc.size(), 1);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
}

TEST(VectorTest, Erase)
{
    FErase<std::string>();
}

TEST(VectorTest, EraseNoMove)
{
    FErase<NoMoveConstructible>();
}

TEST(VectorTest, ErasePod)
{
    FErase<PodStruct>();
}

template<typename T>
void FEraseIt()
{
    auto vbc = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vbc.push_back_n(another_data_array, 3);

    EXPECT_EQ(vbc.size(), 6);

    // try to erase by not valid iterator
    auto it = vbc.erase(vbc.begin() - 1, vbc.begin() + 1);

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 6);

    it = vbc.erase(vbc.begin(), vbc.begin() - 1);

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 6);

    it = vbc.erase(vbc.begin() + 1, vbc.begin());

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 6);

    // try to erase 0 elements
    it = vbc.erase(vbc.end(), vbc.end());

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 6);

    it = vbc.erase(vbc.begin() + 1, vbc.begin() + 1);

    EXPECT_EQ(it, vbc.begin() + 1);
    EXPECT_EQ(vbc.size(), 6);

    // main usage
    it = vbc.erase(vbc.begin() + 4, vbc.begin() + 5);
    EXPECT_EQ(it, vbc.begin() + 4);
    EXPECT_EQ(vbc.size(), 5);

    for (size_t i = 0; i < 3; ++i)
        EXPECT_EQ(vbc[i], g_data_array<T>[i]);

    EXPECT_EQ(vbc[3], another_data_array[0]);
    EXPECT_EQ(vbc[4], another_data_array[2]);

    it = vbc.erase(vbc.begin() + 1, vbc.begin() + 3);
    EXPECT_EQ(it, vbc.begin() + 1);
    EXPECT_EQ(vbc.size(), 3);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
    EXPECT_EQ(vbc[1], another_data_array[0]);
    EXPECT_EQ(vbc[2], another_data_array[2]);

    it = vbc.erase(vbc.begin() + 2, vbc.end());
    EXPECT_EQ(it, vbc.begin() + 2);
    EXPECT_EQ(vbc.size(), 2);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
    EXPECT_EQ(vbc[1], another_data_array[0]);

    // test that only right range is cut after end()
    it = vbc.erase(vbc.begin() + 1, vbc.end() + 10);

    EXPECT_EQ(it, vbc.end());
    EXPECT_EQ(vbc.size(), 1);

    EXPECT_EQ(vbc[0], g_data_array<T>[0]);
}

TEST(VectorTest, EraseIt)
{
    FEraseIt<std::string>();
}

TEST(VectorTest, EraseItNoMove)
{
    FEraseIt<NoMoveConstructible>();
}

TEST(VectorTest, EraseItPod)
{
    FEraseIt<PodStruct>();
}

TEST(VectorTest, Data)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    EXPECT_EQ(vbc.data(), nullptr);

    vbc.push_back_n(g_data_array<std::string>, 3);

    for (size_t i = 0; i < vbc.size(); ++i)
        EXPECT_EQ(*(vbc.data() + i), g_data_array<std::string>[i]);
}

TEST(VectorTest, Size)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    EXPECT_EQ(vbc.size(), 0);

    vbc.push_back_n(g_data_array<std::string>, 3);

    EXPECT_EQ(vbc.size(), 3);
}

TEST(VectorTest, MaxSize)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    EXPECT_EQ(vbc.max_size(), RawHeapAllocator().max_size());
}

TEST(VectorTest, Capacity)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    EXPECT_EQ(vbc.capacity(), 0);

    vbc.get_allocator_helper().setAllocationStrategy(AllocationStrategy::strictByDataSize);

    vbc.push_back_n(g_data_array<std::string>, 3);

    EXPECT_EQ(vbc.capacity(), 3);
}

TEST(VectorTest, Clear)
{
    auto vbc = getStringsFilledContainer<std::string>();

    size_t old_capacity = vbc.capacity();

    vbc.clear();

    EXPECT_NE(vbc.data(), nullptr);
    EXPECT_EQ(vbc.capacity(), old_capacity);
    EXPECT_EQ(vbc.size(), 0);
}

TEST(VectorTest, Invalidate)
{
    auto vbc = getStringsFilledContainer<std::string>();

    vbc.invalidate();

    EXPECT_EQ(vbc.data(), nullptr);
    EXPECT_EQ(vbc.capacity(), 0);
    EXPECT_EQ(vbc.size(), 0);
}

TEST(VectorTest, Release)
{
    auto vbc = getStringsFilledContainer<std::string>();

    auto p1 = vbc.data();
    auto p2 = vbc.release();

    EXPECT_EQ(p1, p2);

    EXPECT_EQ(vbc.data(), nullptr);
    EXPECT_EQ(vbc.capacity(), 0);
    EXPECT_EQ(vbc.size(), 0);
}

TEST(VectorTest, GetAllocatorHelper)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vbc;

    auto allocator = vbc.get_allocator_helper();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(allocator)>, DefaultVectorAllocatorHelper<std::string>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(vbc.get_allocator_helper())>));
}
