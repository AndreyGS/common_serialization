#include <gtest/gtest.h>
#include "special_types.h"
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
using DefaultVectorAllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawHeapAllocator>>;

using size_type = typename Vector<int, DefaultVectorAllocatorHelper<int>>::size_type;

template<typename T>
T g_data_array[] = { "123", "456", "789" };

template<typename T>
auto getStringsFilledContainer()
{
    static Vector<T, DefaultVectorAllocatorHelper<T>> vec;
    
    if (vec.size() == 0)
        vec.push_back_n(g_data_array<T>, 3);

    EXPECT_EQ(vec.capacity(), 6); // check that nothing is changed in allocation strategy

    return vec;
}

template<>
auto getStringsFilledContainer<PodStruct>()
{
    static Vector<PodStruct, StrategicAllocatorHelper<PodStruct, RawHeapAllocator>> vec;

    if (vec.size() == 0)
        vec.push_back_n(g_data_array<PodStruct>, 3);

    EXPECT_EQ(vec.capacity(), 6); // check that nothing is changed in allocation strategy

    return vec;
}

} // namespace anonymous

TEST(VectorTest, Constructor)
{
    Vector<int, DefaultVectorAllocatorHelper<int>> vec;

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.data(), nullptr);
}

template<typename T>
void FCopyConstructor()
{
    auto vec1 = getStringsFilledContainer<T>();
    auto vec2(vec1);

    EXPECT_EQ(vec1.size(), vec2.size());

    for (size_type i = 0; i < vec1.size(); ++i)
        EXPECT_EQ(vec1[i], vec2[i]);
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
    auto vec1 = getStringsFilledContainer<T>();
    auto vec2(std::move(vec1));

    EXPECT_EQ(vec1.size(), 0);
    EXPECT_EQ(vec1.capacity(), 0);
    EXPECT_EQ(vec1.data(), nullptr);

    EXPECT_EQ(vec2.size(), 3);
    for (size_type i = 0; i < vec2.size(); ++i)
        EXPECT_EQ(vec2[i], g_data_array<T>[i]);
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
    auto vec1 = getStringsFilledContainer<T>();
    auto vec2 = vec1;

    EXPECT_EQ(vec1.size(), vec2.size());

    for (size_type i = 0; i < vec1.size(); ++i)
        EXPECT_EQ(vec1[i], vec2[i]);
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
    auto vec1 = getStringsFilledContainer<T>();
    auto vec2 = std::move(vec1);

    EXPECT_EQ(vec1.size(), 0);
    EXPECT_EQ(vec1.capacity(), 0);
    EXPECT_EQ(vec1.data(), nullptr);

    EXPECT_EQ(vec2.size(), 3);
    for (size_type i = 0; i < vec2.size(); ++i)
        EXPECT_EQ(vec2[i], g_data_array<T>[i]);
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
    Vector<int, DefaultVectorAllocatorHelper<int>> vec;
    vec.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::strictByDataSize);
    
    int i = 1;
    vec.push_back(i);
    EXPECT_EQ(vec.capacity(), 1);

    bool b = vec.reserve(100);
    EXPECT_TRUE(b);
    EXPECT_EQ(vec.capacity(), 100);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], i);

    // test when memory couldn't be allocated
    b = vec.reserve(static_cast<size_type>(-1));
    EXPECT_FALSE(b);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(vec.capacity(), 100);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], i);
}

TEST(VectorTest, PushBack)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    // test l-value
    std::string str("123");
    auto& vec_ref1 = vec.push_back(str);
    EXPECT_EQ(vec[0], "123");
    EXPECT_EQ(&vec_ref1, &vec);
    EXPECT_EQ(str.size(), 3);

    // test r-value
    auto& vec_ref2 = vec.push_back(std::move(str));

    EXPECT_EQ(vec[1], "123");
    EXPECT_EQ(&vec_ref2, &vec);
    EXPECT_EQ(str.size(), 0);

    EXPECT_EQ(&vec_ref1, &vec_ref2);
}

TEST(VectorTest, PushBackNoMove)
{
    Vector<NoMoveConstructible, DefaultVectorAllocatorHelper<NoMoveConstructible>> vec;

    // test l-value
    NoMoveConstructible str("123");
    auto& vec_ref1 = vec.push_back(str);
    EXPECT_EQ(vec[0], "123");
    EXPECT_EQ(&vec_ref1, &vec);
    EXPECT_EQ(str.size, 3);

    // test r-value
    auto& vec_ref2 = vec.push_back(std::move(str));

    EXPECT_EQ(vec[1], "123");
    EXPECT_EQ(&vec_ref2, &vec);
    EXPECT_EQ(str.size, 3);
    EXPECT_TRUE(str.p != nullptr);

    EXPECT_EQ(&vec_ref1, &vec_ref2);
}

TEST(VectorTest, PushBackPod)
{
    Vector<PodStruct, StrategicAllocatorHelper<PodStruct, RawHeapAllocator>> vec_pod;

    // test l-value
    auto& vec_ref1 = vec_pod.push_back("123");
    EXPECT_EQ(memcmp(vec_pod.data(), "123", sizeof(PodStruct)), 0);
    EXPECT_EQ(&vec_ref1, &vec_pod);

    // test r-value
    PodStruct ps("456");
    auto& vec_ref2 = vec_pod.push_back(std::move(ps));

    EXPECT_EQ(memcmp(vec_pod.data() + 1, "456", sizeof(PodStruct)), 0);
    EXPECT_EQ(&vec_ref2, &vec_pod);

    EXPECT_EQ(&vec_ref1, &vec_ref2);
}

template<typename T>
void FPushBackN()
{
    auto vec = getStringsFilledContainer<T>();

    EXPECT_EQ(vec.size(), 3);

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    auto& vec_ref = vec.push_back_n(g_data_array<T>, 3);
    EXPECT_EQ(&vec_ref, &vec);
    EXPECT_EQ(vec.size(), 6);

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i % 3]);

    // test with additional memory allocation
    T another_data_array[] = { "abc", "def", "ghi" };
    vec.push_back_n(another_data_array, 3);
    EXPECT_EQ(vec.size(), 9);

    for (size_type i = 0; i < 6; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i % 3]);

    for (size_type i = 6; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], another_data_array[i - 6]);

    // not valid data or data size tests
    vec.push_back_n(nullptr, 3);
    EXPECT_EQ(vec.size(), 9);

    vec.push_back_n(g_data_array<T>, static_cast<size_type>(-1));
    EXPECT_EQ(vec.size(), 9);
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
    auto vec = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    size_type newOffset = vec.replace(5, g_data_array<T>, 3);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    newOffset = vec.replace(1, another_data_array, 1);

    EXPECT_EQ(newOffset, 2);
    EXPECT_EQ(vec.size(), 3);

    vec.push_back_n(another_data_array, 3);

    // test with additional memory allocation
    newOffset = vec.replace(5, &another_data_array[1], 2);
    EXPECT_EQ(newOffset, 7);
    EXPECT_EQ(vec.size(), 7);

    for (size_type i = 5; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], another_data_array[i - 4]);
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
    auto vec = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    size_type newOffset = vec.insert(5, g_data_array<T>, 3);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    newOffset = vec.insert(0, another_data_array, 3);

    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vec.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 3; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };

    newOffset = vec.insert(2, another_data_array2, 3);
    EXPECT_EQ(newOffset, 5);
    EXPECT_EQ(vec.size(), 9);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(vec[i], another_data_array2[i - 2]);

    EXPECT_EQ(vec[5], another_data_array[2]);

    for (size_type i = 6; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };

    newOffset = vec.insert(8, another_data_array3, 3);

    EXPECT_EQ(newOffset, 11);
    EXPECT_EQ(vec.size(), 12);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(vec[i], another_data_array2[i - 2]);

    EXPECT_EQ(vec[5], another_data_array[2]);

    for (size_type i = 6; i < 8; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 6]);

    for (size_type i = 8; i < 11; ++i)
        EXPECT_EQ(vec[i], another_data_array3[i - 8]);

    EXPECT_EQ(vec[11], g_data_array<T>[2]);

    // test not valid values
    newOffset = vec.insert(13, another_data_array3, 3);

    EXPECT_EQ(newOffset, 12);
    EXPECT_EQ(vec.size(), 12);

    newOffset = vec.insert(10, nullptr, 3);

    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vec.size(), 12);

    newOffset = vec.insert(10, another_data_array3, 0);

    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vec.size(), 12);
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
    auto vec = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    auto it = vec.insert(vec.begin() + 5, g_data_array<T>, g_data_array<T> + 3);
    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    std::list<T> l1;
    l1.insert(l1.begin(), another_data_array, another_data_array + 3);

    it = vec.insert(vec.begin(), l1.begin(), l1.end());

    EXPECT_EQ(*it, *(vec.begin() + 3));
    EXPECT_EQ(vec.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 3; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 3]);
    
    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };
    std::list<T> l2;
    l2.insert(l2.begin(), another_data_array2, another_data_array2 + 3);

    it = vec.insert(vec.begin() + 2, l2.begin(), l2.end());

    EXPECT_EQ(*it, *(vec.begin() + 5));
    EXPECT_EQ(vec.size(), 9);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(vec[i], another_data_array2[i - 2]);

    EXPECT_EQ(vec[5], another_data_array[2]);

    for (size_type i = 6; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 6]);

    T another_data_array3[] = { "+++", "---", "***" };
    std::list<T> l3;
    l3.insert(l3.begin(), another_data_array3, another_data_array3 + 3);

    it = vec.insert(vec.begin() + 8, l3.begin(), l3.end());

    EXPECT_EQ(*it, *(vec.begin() + 11));
    EXPECT_EQ(vec.size(), 12);

    for (size_type i = 0; i < 2; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 2; i < 5; ++i)
        EXPECT_EQ(vec[i], another_data_array2[i - 2]);

    EXPECT_EQ(vec[5], another_data_array[2]);

    for (size_type i = 6; i < 8; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 6]);

    for (size_type i = 8; i < 11; ++i)
        EXPECT_EQ(vec[i], another_data_array3[i - 8]);

    EXPECT_EQ(vec[11], g_data_array<T>[2]);

    // try to insert by not valid iterator
    it = vec.insert(vec.end(), l3.begin(), l3.end());

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 12);

    it = vec.insert(vec.begin() - 1, l3.begin(), l3.end());

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 12);

    it = vec.insert(vec.end() + 1, l3.begin(), l3.end());

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 12);
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
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vec.push_back_n(another_data_array, 3);

    EXPECT_EQ(vec.size(), 6);

    // try to erase by not valid offset
    auto newOffset = vec.erase(vec.size() + 1, 1);

    EXPECT_EQ(newOffset, vec.size());
    EXPECT_EQ(vec.size(), 6);

    // try to erase 0 elements
    newOffset = vec.erase(1, 0);

    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vec.size(), 6);

    newOffset = vec.erase(vec.size(), 1);

    EXPECT_EQ(newOffset, vec.size());
    EXPECT_EQ(vec.size(), 6);

    newOffset = vec.erase(0, 0);

    EXPECT_EQ(newOffset, 0);
    EXPECT_EQ(vec.size(), 6);

    // main usage
    newOffset = vec.erase(4, 1);
    EXPECT_EQ(newOffset, 4);
    EXPECT_EQ(vec.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    EXPECT_EQ(vec[3], another_data_array[0]);
    EXPECT_EQ(vec[4], another_data_array[2]);

    newOffset = vec.erase(1, 2);
    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vec.size(), 3);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);
    EXPECT_EQ(vec[2], another_data_array[2]);

    newOffset = vec.erase(2, vec.size());
    EXPECT_EQ(newOffset, 2);
    EXPECT_EQ(vec.size(), 2);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);

    // test that only right range is cut after last element
    newOffset = vec.erase(1, 10);

    EXPECT_EQ(newOffset, 1);
    EXPECT_EQ(vec.size(), 1);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);

    // test extra big n
    newOffset = vec.erase(0, static_cast<size_type>(-1));

    EXPECT_EQ(newOffset, 0);
    EXPECT_EQ(vec.size(), 0);
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
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vec.push_back_n(another_data_array, 3);

    EXPECT_EQ(vec.size(), 6);

    // try to erase by not valid iterator
    auto it = vec.erase(vec.begin() - 1, vec.begin() + 1);

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 6);

    it = vec.erase(vec.begin(), vec.begin() - 1);

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 6);

    it = vec.erase(vec.begin() + 1, vec.begin());

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 6);

    // try to erase 0 elements
    it = vec.erase(vec.end(), vec.end());

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 6);

    it = vec.erase(vec.begin() + 1, vec.begin() + 1);

    EXPECT_EQ(it, vec.begin() + 1);
    EXPECT_EQ(vec.size(), 6);

    // main usage
    it = vec.erase(vec.begin() + 4, vec.begin() + 5);
    EXPECT_EQ(it, vec.begin() + 4);
    EXPECT_EQ(vec.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    EXPECT_EQ(vec[3], another_data_array[0]);
    EXPECT_EQ(vec[4], another_data_array[2]);

    it = vec.erase(vec.begin() + 1, vec.begin() + 3);
    EXPECT_EQ(it, vec.begin() + 1);
    EXPECT_EQ(vec.size(), 3);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);
    EXPECT_EQ(vec[2], another_data_array[2]);

    it = vec.erase(vec.begin() + 2, vec.end());
    EXPECT_EQ(it, vec.begin() + 2);
    EXPECT_EQ(vec.size(), 2);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);

    // test that only right range is cut after end()
    it = vec.erase(vec.begin() + 1, vec.end() + 10);

    EXPECT_EQ(it, vec.end());
    EXPECT_EQ(vec.size(), 1);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
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

template<typename T>
void FCopyN()
{
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[3] = { T{}, T{}, T{} };

    auto p = vec.copy_n(0, 3, another_data_array);

    EXPECT_EQ(p, another_data_array + 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    p = vec.copy_n(1, 3, another_data_array);

    EXPECT_EQ(p, another_data_array + 2);
    EXPECT_EQ(vec[1], another_data_array[0]);
    EXPECT_EQ(vec[2], another_data_array[1]);
    EXPECT_EQ(vec[2], another_data_array[2]);

    // copy more than vec has
    p = vec.copy_n(0, 10, another_data_array);

    EXPECT_EQ(p, another_data_array + 3);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    // copy zero elements
    p = vec.copy_n(1, 0, another_data_array);

    EXPECT_EQ(p, another_data_array);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    // try to copy with wrong offset
    p = vec.copy_n(3, 1, another_data_array);

    EXPECT_EQ(p, another_data_array);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    // try to copy with nullptr as destination
    p = vec.copy_n(1, 3, nullptr);

    EXPECT_EQ(p, nullptr);
}

TEST(VectorTest, CopyN)
{
    FCopyN<std::string>();
}

TEST(VectorTest, CopyNNoMove)
{
    FCopyN<NoMoveConstructible>();
}

TEST(VectorTest, CopyNPod)
{
    FCopyN<PodStruct>();
}

template<typename T>
void FCopyNIt()
{
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[3] = { T{}, T{}, T{} };
    std::list<T> l(another_data_array, another_data_array + 3);

    auto it = vec.copy_n(vec.begin(), vec.end(), l.begin());
   
    EXPECT_EQ(it, l.end());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    it = vec.copy_n(vec.begin() + 1, vec.end(), l.begin());
    
    auto tempIt = ++l.begin();
    EXPECT_EQ(it, ++tempIt);

    tempIt = l.begin();
    EXPECT_EQ(vec[1], *tempIt);
    EXPECT_EQ(vec[2], *++tempIt);
    EXPECT_EQ(vec[2], *++tempIt);
    
    // copy more than vec has
    it = vec.copy_n(vec.begin(), vec.end() + 1, l.begin());

    EXPECT_EQ(it, l.end());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    // copy zero elements
    it = vec.copy_n(vec.begin(), vec.begin(), l.begin());

    EXPECT_EQ(it, l.begin());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    // try to copy with wrong offset
    it = vec.copy_n(vec.end(), vec.begin(), l.begin());
    EXPECT_EQ(it, l.begin());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    it = vec.copy_n(vec.end(), vec.end(), l.begin());
    EXPECT_EQ(it, l.begin());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));
}

TEST(VectorTest, CopyNIt)
{
    FCopyNIt<std::string>();
}

TEST(VectorTest, CopyNItNoMove)
{
    FCopyNIt<NoMoveConstructible>();
}

TEST(VectorTest, CopyNItPod)
{
    FCopyNIt<PodStruct>();
}

TEST(VectorTest, Data)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    EXPECT_EQ(vec.data(), nullptr);

    vec.push_back_n(g_data_array<std::string>, 3);

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(*(vec.data() + i), g_data_array<std::string>[i]);
}

TEST(VectorTest, OperatorAt)
{
    auto vec = getStringsFilledContainer<std::string>();

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<std::string>[i]);
}

TEST(VectorTest, Size)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    EXPECT_EQ(vec.size(), 0);

    vec.push_back_n(g_data_array<std::string>, 3);

    EXPECT_EQ(vec.size(), 3);
}

TEST(VectorTest, MaxSize)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    EXPECT_EQ(vec.max_size(), RawHeapAllocator().max_size());
}

TEST(VectorTest, Capacity)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    EXPECT_EQ(vec.capacity(), 0);

    vec.getAllocatorHelper().setAllocationStrategy(AllocationStrategy::strictByDataSize);

    vec.push_back_n(g_data_array<std::string>, 3);

    EXPECT_EQ(vec.capacity(), 3);
}

TEST(VectorTest, Clear)
{
    auto vec = getStringsFilledContainer<std::string>();

    size_type old_capacity = vec.capacity();

    vec.clear();

    EXPECT_NE(vec.data(), nullptr);
    EXPECT_EQ(vec.capacity(), old_capacity);
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, Invalidate)
{
    auto vec = getStringsFilledContainer<std::string>();

    vec.invalidate();

    EXPECT_EQ(vec.data(), nullptr);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, Release)
{
    auto vec = getStringsFilledContainer<std::string>();

    auto p1 = vec.data();
    auto p2 = vec.release();

    EXPECT_EQ(p1, p2);

    EXPECT_EQ(vec.data(), nullptr);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, GetAllocatorHelper)
{
    Vector<std::string, DefaultVectorAllocatorHelper<std::string>> vec;

    auto allocator = vec.getAllocatorHelper();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(allocator)>, DefaultVectorAllocatorHelper<std::string>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(vec.getAllocatorHelper())>));
}
