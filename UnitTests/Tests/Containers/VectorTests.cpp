/**
 * @file VectorTests.cpp
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

namespace
{

using namespace special_types;
using namespace common_serialization;

// Explicitly set default allocator helper so we wouldn't depend on 
// possible future changes of default allocator helper of Vector
template<typename T>
using DefaultAllocationManager = CtorStratAllocationManagerT<T>;

using size_type = typename Vector<int, DefaultAllocationManager<int>>::size_type;

template<typename T>
T g_data_array[] = { "123", "456", "789" };

template<typename T>
auto getStringsFilledContainer()
{
    Vector<T, DefaultAllocationManager<T>> vec;
    vec.pushBackN(g_data_array<T>, 3);

    EXPECT_EQ(vec.capacity(), 6); // check that nothing is changed in allocation strategy

    return vec;
}

template<>
auto getStringsFilledContainer<PodStruct>()
{
    Vector<PodStruct, RawStratAllocationManagerT<PodStruct>> vec;
    vec.pushBackN(g_data_array<PodStruct>, 3);

    EXPECT_EQ(vec.capacity(), 6); // check that nothing is changed in allocation strategy

    return vec;
}

TEST(VectorTest, Constructor)
{
    Vector<int, DefaultAllocationManager<int>> vec;

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

TEST(VectorTest, Destructor)
{
    auto vec = getStringsFilledContainer<std::string>();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_TRUE(vec.capacity() >= 3);
    EXPECT_TRUE(vec.data() != nullptr);

    vec.~Vector();

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.data(), nullptr);
}

template<typename T>
void FInit()
{
    auto vec1 = getStringsFilledContainer<T>();
    decltype(vec1) vec2;

    EXPECT_EQ(vec2.init(vec1), Status::NoError);

    EXPECT_EQ(vec1.size(), vec2.size());

    for (size_type i = 0; i < vec1.size(); ++i)
        EXPECT_EQ(vec1[i], vec2[i]);

    vec1.invalidate();

    // init by empty Vector
    EXPECT_EQ(vec2.init(vec1), Status::NoError);
    EXPECT_EQ(vec2.size(), 0);
}

TEST(VectorTest, init)
{
    FInit<std::string>();
}

TEST(VectorTest, InitNoMove)
{
    FInit<NoMoveConstructible>();
}

TEST(VectorTest, InitPod)
{
    FInit<PodStruct>();

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec1;
    PodStruct ps[1] = { { } };
    vec1.getAllocationManager().getAllocator().setStorage(ps, 1);
    vec1.pushBack("123");

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec2;
    EXPECT_EQ(vec2.init(vec1), Status::ErrorNoMemory);
}

TEST(VectorTest, InitErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec1;
    ErrorProne::errorOnCounter = 100;
    vec1.pushBack(ErrorProne{});

    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec2;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorOverflow;
    
    EXPECT_EQ(vec2.init(vec1), Status::ErrorOverflow);
}

template<typename T>
void FInitMove()
{
    auto vec1 = getStringsFilledContainer<T>();
    decltype(vec1) vec2;

    EXPECT_EQ(vec2.init(std::move(vec1)), Status::NoError);

    EXPECT_EQ(vec1.size(), 0);
    EXPECT_EQ(vec1.capacity(), 0);
    EXPECT_EQ(vec1.data(), nullptr);

    EXPECT_EQ(vec2.size(), 3);
    for (size_type i = 0; i < vec2.size(); ++i)
        EXPECT_EQ(vec2[i], g_data_array<T>[i]);

    // init by empty Vector
    EXPECT_EQ(vec2.init(std::move(vec1)), Status::NoError);
    EXPECT_EQ(vec2.size(), 0);
}

TEST(VectorTest, InitMove)
{
    FInitMove<std::string>();
}

TEST(VectorTest, InitMoveNoMove)
{
    FInitMove<NoMoveConstructible>();
}

TEST(VectorTest, InitMovePod)
{
    FInitMove<PodStruct>();

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec1;
    PodStruct ps[1] = { { } };
    vec1.getAllocationManager().getAllocator().setStorage(ps, 1);
    vec1.pushBack("123");

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec2;
    EXPECT_EQ(vec2.init(std::move(vec1)), Status::NoError);
}

TEST(VectorTest, SetSize)
{
    uint8_t buffer[32]{ 0 };
    Vector<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> vec;
    vec.getAllocationManager().getAllocator().setStorage(buffer, 32);

    EXPECT_EQ(vec.setSize(32), Status::NoError);
    EXPECT_EQ(vec.size(), 32);
    EXPECT_EQ(vec.setSize(33), Status::ErrorNoMemory);
    EXPECT_EQ(vec.size(), 32);
}

TEST(VectorTest, Reserve)
{
    Vector<int, DefaultAllocationManager<int>> vec;
    vec.getAllocationManager().setAllocationStrategy(AllocationStrategy::StrictByDataSize);
    
    int i = 1;
    vec.pushBack(i);
    EXPECT_EQ(vec.capacity(), 1);

    EXPECT_EQ(vec.reserve(100), Status::NoError);
    EXPECT_EQ(vec.capacity(), 100);

    // Check that previously added element in not gone after reallocating
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], i);

    // test when memory couldn't be allocated
    // 1
    EXPECT_EQ(vec.reserve(static_cast<size_type>(-1)), Status::ErrorNoMemory);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(vec.capacity(), 100);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], i);

    // 2
    vec.getAllocationManager().setAllocationStrategy(AllocationStrategy::DoubleOfDataSize);
    EXPECT_EQ(vec.reserve(static_cast<size_type>(-1)), Status::ErrorNoMemory);

    // Check that after false memory allocation container not lost its contents
    EXPECT_EQ(vec.capacity(), 100);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], i);

    // Error propagation test
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vecE;
    ErrorProne::errorOnCounter = 100;
    vecE.pushBack(ErrorProne{});

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorNoMemory;
    EXPECT_EQ(vecE.reserve(100), Status::ErrorNoMemory);
}

TEST(VectorTest, PushBack)
{
    Vector<std::string, DefaultAllocationManager<std::string>> vec;
    std::string str("123");

    // test l-value
    EXPECT_EQ(vec.pushBack(str), Status::NoError);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], "123");
    EXPECT_EQ(str.size(), 3);

    // test r-value
    EXPECT_EQ(vec.pushBack(std::move(str)), Status::NoError);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], "123");
    EXPECT_EQ(str.size(), 0);

    // Error propagation test
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vecE;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidArgument;

    // test l-value
    ErrorProne ep{};
    EXPECT_EQ(vecE.pushBack(ep), Status::ErrorInvalidArgument);

    // test r-value
    ErrorProne::counter = 0;
    EXPECT_EQ(vecE.pushBack(ErrorProne{}), Status::ErrorInvalidArgument);
}

TEST(VectorTest, PushBackNoMove)
{
    Vector<NoMoveConstructible, DefaultAllocationManager<NoMoveConstructible>> vec;

    // test l-value
    NoMoveConstructible str("123");
    EXPECT_EQ(vec.pushBack(str), Status::NoError);
    EXPECT_EQ(vec[0], "123");
    EXPECT_EQ(str.m_str.size(), 3);

    // test r-value
    EXPECT_EQ(vec.pushBack(std::move(str)), Status::NoError);
    EXPECT_EQ(vec[1], "123");
    EXPECT_EQ(str.m_str.size(), 3);
}

TEST(VectorTest, PushBackPod)
{
    Vector<PodStruct, RawStratAllocationManagerT<PodStruct>> vec_pod;

    // test l-value
    EXPECT_EQ(vec_pod.pushBack("123"), Status::NoError);
    EXPECT_EQ(memcmp(vec_pod.data(), "123", sizeof(PodStruct)), 0);

    // test r-value
    PodStruct ps("456");

    EXPECT_EQ(vec_pod.pushBack(std::move(ps)), Status::NoError);
    EXPECT_EQ(memcmp(vec_pod.data() + 1, "456", sizeof(PodStruct)), 0);

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec_pod2;

    // test l-value
    EXPECT_EQ(vec_pod2.pushBack(ps), Status::ErrorNoMemory);
    EXPECT_EQ(vec_pod2.size(), 0);

    // test r-value
    EXPECT_EQ(vec_pod2.pushBack(std::move(ps)), Status::ErrorNoMemory);
    EXPECT_EQ(vec_pod2.size(), 0);
}

template<typename T>
void FPushBackN()
{
    auto vec = getStringsFilledContainer<T>();

    EXPECT_EQ(vec.size(), 3);

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    EXPECT_EQ(vec.pushBackN(g_data_array<T>, 3), Status::NoError);
    EXPECT_EQ(vec.size(), 6);

    for (size_type i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i % 3]);

    // test with additional memory allocation
    T another_data_array[] = { "abc", "def", "ghi" };
    EXPECT_EQ(vec.pushBackN(another_data_array, 3), Status::NoError);
    EXPECT_EQ(vec.size(), 9);

    for (size_type i = 0; i < 6; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i % 3]);

    for (size_type i = 6; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], another_data_array[i - 6]);

    // not valid data or data size tests
    EXPECT_EQ(vec.pushBackN(nullptr, 3), Status::ErrorInvalidArgument);
    EXPECT_EQ(vec.size(), 9);

    EXPECT_EQ(vec.pushBackN(nullptr, 0), Status::NoError);
    EXPECT_EQ(vec.size(), 9);

    EXPECT_EQ(vec.pushBackN(g_data_array<T>, static_cast<size_type>(-1)), Status::ErrorOverflow);
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

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec_pod;
    PodStruct ps("456");

    EXPECT_EQ(vec_pod.pushBackN(&ps, 1), Status::ErrorNoMemory);
    EXPECT_EQ(vec_pod.size(), 0);
}

TEST(VectorTest, PushBackNErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;
    ErrorProne::errorOnCounter = 100;
    ErrorProne ep[2] = {{}, {}};

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1; // second item instantiation shall produce error
    ErrorProne::currentError = Status::ErrorInvalidArgument;
    EXPECT_EQ(vec.pushBackN(ep, 2), Status::ErrorInvalidArgument);
}

TEST(VectorTest, PushBackArithmeticValue)
{
    Vector<uint8_t, DefaultAllocationManager<uint8_t>> vec;
    vec.getAllocationManager().setAllocationStrategy(AllocationStrategy::DoubleOfDataSize); // as a precaution

    constexpr double value = 5.;

    EXPECT_EQ(vec.pushBackArithmeticValue(value), Status::NoError);
    EXPECT_EQ(*reinterpret_cast<decltype(value)*>(vec.data()), value);
    EXPECT_EQ(vec.size(), sizeof(value));
    EXPECT_EQ(vec.capacity(), 2 * sizeof(value));

    Vector<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> vec2;

    EXPECT_EQ(vec2.pushBackArithmeticValue(value), Status::ErrorNoMemory);
    EXPECT_EQ(vec2.size(), 0);
}

template<typename T>
void FReplace()
{
    auto vec = getStringsFilledContainer<T>();
    size_type newOffset = 0;

    // check that sparse construction is not allowed
    EXPECT_EQ(vec.replace(g_data_array<T>, 3, 4, &newOffset), Status::ErrorOverflow);
    EXPECT_EQ(newOffset, 0);
    EXPECT_EQ(vec.size(), 3);

    // check for invalid arguments
    EXPECT_EQ(vec.replace(nullptr, 3, 4, &newOffset), Status::ErrorInvalidArgument);
    EXPECT_EQ(newOffset, 0);
    EXPECT_EQ(vec.size(), 3);

    // check for valid nullptr
    EXPECT_EQ(vec.replace(nullptr, 0, 3, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    
    EXPECT_EQ(vec.replace(another_data_array, 1, 1, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 2);
    EXPECT_EQ(vec.size(), 3);

    EXPECT_EQ(vec.pushBackN(another_data_array, 3), Status::NoError);

    // test with additional memory allocation
    EXPECT_EQ(vec.replace(&another_data_array[1], 2, 5, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 7);
    EXPECT_EQ(vec.size(), 7);

    for (size_type i = 5; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], another_data_array[i - 4]);

    // test on offest == size
    EXPECT_EQ(vec.replace(&another_data_array[1], 2, 7, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 9);
    EXPECT_EQ(vec.size(), 9);

    // try to not pass optional arg
    EXPECT_EQ(vec.replace(another_data_array, 0, 0), Status::NoError);
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

    Vector<PodStruct, RawKeeperGenAllocationManagerT<PodStruct>> vec_pod;
    PodStruct psArr{ "456" };

    EXPECT_EQ(vec_pod.replace(&psArr, 1, 0), Status::ErrorNoMemory);
}

TEST(VectorTest, ReplaceErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;
    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});

    ErrorProne ep[2] = { {}, {} };
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidArgument;
    EXPECT_EQ(vec.replace(ep, 1, 0), Status::ErrorInvalidArgument);
}

template<typename T>
void FInsert()
{
    auto vec = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    EXPECT_EQ(vec.insert(g_data_array<T>, 3, 5), Status::ErrorOverflow);
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    size_type newOffset = 0;
    EXPECT_EQ(vec.insert(another_data_array, 3, 0, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 3);
    EXPECT_EQ(vec.size(), 6);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array[i]);

    for (size_type i = 3; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i - 3]);

    // test with additional memory allocation
    T another_data_array2[] = { "###", "$$$", "%%%" };

    EXPECT_EQ(vec.insert(another_data_array2, 3, 2, &newOffset), Status::NoError);
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

    EXPECT_EQ(vec.insert(another_data_array3, 3, 8, &newOffset), Status::NoError);
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

    // try to not pass optional arg
    EXPECT_EQ(vec.insert(another_data_array3, 0, 0), Status::NoError);

    // test zero items
    EXPECT_EQ(vec.insert(another_data_array3, 0, 10, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vec.size(), 12);

    EXPECT_EQ(vec.insert(nullptr, 0, 10, &newOffset), Status::NoError);
    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vec.size(), 12);

    // test not valid values
    EXPECT_EQ(vec.insert(another_data_array3, 3, 13, &newOffset), Status::ErrorOverflow);
    EXPECT_EQ(newOffset, 10);
    EXPECT_EQ(vec.size(), 12);

    EXPECT_EQ(vec.insert(nullptr, 3, 10, &newOffset), Status::ErrorInvalidArgument);
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

    Vector<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> vec;
    uint8_t i = 1;

    EXPECT_EQ(vec.insert(&i, 1, 0), Status::ErrorNoMemory);
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, InsertErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;
    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});

    ErrorProne ep[2] = { {}, {} };
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidArgument;
    EXPECT_EQ(vec.insert(ep, 1, 0), Status::ErrorInvalidArgument);
}

template<typename T>
void FInsertIt()
{
    auto vec = getStringsFilledContainer<T>();

    // check that sparse construction is not allowed
    EXPECT_EQ(vec.insert(g_data_array<T>, g_data_array<T> + 3, vec.begin() + 5), Status::ErrorOverflow);
    EXPECT_EQ(vec.size(), 3);

    T another_data_array[] = { "abc", "def", "ghi" };

    std::list<T> l1;
    l1.insert(l1.begin(), another_data_array, another_data_array + 3);

    auto it = vec.end();
    EXPECT_EQ(vec.insert(l1.begin(), l1.end(), vec.begin(), &it), Status::NoError);
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

    EXPECT_EQ(vec.insert(l2.begin(), l2.end(), vec.begin() + 2, &it), Status::NoError);
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

    EXPECT_EQ(vec.insert(l3.begin(), l3.end(), vec.begin() + 8, &it), Status::NoError);
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

    // try to insert to the end
    EXPECT_EQ(vec.insert(l3.begin(), l3.end(), vec.end(), &it), Status::NoError);
    EXPECT_EQ(it, vec.begin() + 15);
    EXPECT_EQ(vec.size(), 15);

    for (size_type i = 12; i < 15; ++i)
        EXPECT_EQ(vec[i], another_data_array3[i - 12]);

    // try to not pass optional arg
    EXPECT_EQ(vec.insert(l3.begin(), l3.begin(), vec.begin()), Status::NoError);

    it = vec.begin() + 14;

    // try to insert to not valid iterators range
    EXPECT_EQ(vec.insert(l3.begin(), l3.end(), vec.begin() - 1, &it), Status::ErrorOverflow);
    EXPECT_EQ(it, vec.begin() + 14);
    EXPECT_EQ(vec.size(), 15);

    EXPECT_EQ(vec.insert(l3.begin(), l3.end(), vec.end() + 1, &it), Status::ErrorOverflow);
    EXPECT_EQ(it, vec.begin() + 14);
    EXPECT_EQ(vec.size(), 15);
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

    Vector<uint8_t, RawKeeperGenAllocationManagerT<uint8_t>> vec;
    uint8_t i = 0;
    vec.getAllocationManager().getAllocator().setStorage(&i, 1);
    vec.pushBack(i);

    std::list<uint8_t> l1;
    l1.push_back(i);
    EXPECT_EQ(vec.insert(l1.begin(), l1.end(), vec.begin()), Status::ErrorNoMemory);
    EXPECT_EQ(vec.size(), 1);
}

TEST(VectorTest, InsertItErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;

    std::list<ErrorProne> list;
    ErrorProne::errorOnCounter = 100;
    list.push_back(ErrorProne{});

    ErrorProne ep[2] = { {}, {} };
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidArgument;
    EXPECT_EQ(vec.insert(list.begin(), list.end(), vec.begin()), Status::ErrorInvalidArgument);
}

template<typename T>
void FErase()
{
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vec.pushBackN(another_data_array, 3);

    EXPECT_EQ(vec.size(), 6);

    // try to erase by not valid offset
    EXPECT_EQ(vec.erase(vec.size(), 1), Status::ErrorOverflow);
    EXPECT_EQ(vec.size(), 6);

    EXPECT_EQ(vec.erase(vec.size(), 0), Status::NoError);
    EXPECT_EQ(vec.size(), 6);

    // try to erase 0 elements
    EXPECT_EQ(vec.erase(1, 0), Status::NoError);
    EXPECT_EQ(vec.size(), 6);

    // main usage
    EXPECT_EQ(vec.erase(4, 1), Status::NoError);
    EXPECT_EQ(vec.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    EXPECT_EQ(vec[3], another_data_array[0]);
    EXPECT_EQ(vec[4], another_data_array[2]);

    EXPECT_EQ(vec.erase(1, 2), Status::NoError);
    EXPECT_EQ(vec.size(), 3);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);
    EXPECT_EQ(vec[2], another_data_array[2]);

    EXPECT_EQ(vec.erase(2, vec.size()), Status::NoError);
    EXPECT_EQ(vec.size(), 2);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);

    // test that only right range is cut after last element
    EXPECT_EQ(vec.erase(1, 10), Status::NoError);
    EXPECT_EQ(vec.size(), 1);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);

    // test extra big n
    EXPECT_EQ(vec.erase(0, static_cast<size_type>(-1)), Status::NoError);
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

TEST(VectorTest, EraseErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;
    
    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});
    vec.pushBack(ErrorProne{});

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidHash;
    EXPECT_EQ(vec.erase(0, 1), Status::ErrorInvalidHash);
}

template<typename T>
void FEraseIt()
{
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[] = { "abc", "def", "ghi" };
    vec.pushBackN(another_data_array, 3);

    EXPECT_EQ(vec.size(), 6);

    // try to erase by not valid iterator
    EXPECT_EQ(vec.erase(vec.begin() - 1, vec.begin() + 1), Status::ErrorInvalidArgument);
    EXPECT_EQ(vec.size(), 6);

    EXPECT_EQ(vec.erase(vec.begin(), vec.begin() - 1), Status::ErrorOverflow);
    EXPECT_EQ(vec.size(), 6);

    EXPECT_EQ(vec.erase(vec.begin() + 1, vec.begin()), Status::ErrorOverflow);
    EXPECT_EQ(vec.size(), 6);

    // try to erase 0 elements
    EXPECT_EQ(vec.erase(vec.end(), vec.end()), Status::ErrorInvalidArgument);
    EXPECT_EQ(vec.size(), 6);

    EXPECT_EQ(vec.erase(vec.begin() + 1, vec.begin() + 1), Status::NoError);
    EXPECT_EQ(vec.size(), 6);

    // main usage
    EXPECT_EQ(vec.erase(vec.begin() + 4, vec.begin() + 5), Status::NoError);
    EXPECT_EQ(vec.size(), 5);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], g_data_array<T>[i]);

    EXPECT_EQ(vec[3], another_data_array[0]);
    EXPECT_EQ(vec[4], another_data_array[2]);

    EXPECT_EQ(vec.erase(vec.begin() + 1, vec.begin() + 3), Status::NoError);
    EXPECT_EQ(vec.size(), 3);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);
    EXPECT_EQ(vec[2], another_data_array[2]);

    EXPECT_EQ(vec.erase(vec.begin() + 2, vec.end()), Status::NoError);
    EXPECT_EQ(vec.size(), 2);

    EXPECT_EQ(vec[0], g_data_array<T>[0]);
    EXPECT_EQ(vec[1], another_data_array[0]);

    // test that only right range is cut after end()
    EXPECT_EQ(vec.erase(vec.begin() + 1, vec.end() + 10), Status::NoError);
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

TEST(VectorTest, EraseItErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;

    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});
    vec.pushBack(ErrorProne{});

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidHash;
    EXPECT_EQ(vec.erase(vec.begin(), vec.begin() + 1), Status::ErrorInvalidHash);
}

template<typename T>
void FCopyN()
{
    auto vec = getStringsFilledContainer<T>();

    auto another_data_array = std::make_unique_for_overwrite<T[]>(3);

    T* p = nullptr;
    EXPECT_EQ(vec.copyN(0, 3, another_data_array.get(), &p), Status::NoError);
    EXPECT_EQ(p, another_data_array.get() + 3);

    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array.get()[i]);

    EXPECT_EQ(vec.copyN(1, 3, another_data_array.get(), &p), Status::NoError);
    EXPECT_EQ(p, another_data_array.get() + 2);
    EXPECT_EQ(vec[1], another_data_array.get()[0]);
    EXPECT_EQ(vec[2], another_data_array.get()[1]);

    // try to not pass optional arg
    p = nullptr;
    EXPECT_EQ(vec.copyN(0, 0, another_data_array.get()), Status::NoError);

    // copy more than vec has
    p = nullptr;
    EXPECT_EQ(vec.copyN(0, 10, another_data_array.get(), &p), Status::NoError);
    EXPECT_EQ(p, another_data_array.get() + 3);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array.get()[i]);

    // copy zero elements
    p = nullptr;
    EXPECT_EQ(vec.copyN(1, 0, another_data_array.get(), &p), Status::NoError);
    EXPECT_EQ(p, another_data_array.get());
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array.get()[i]);

    p = reinterpret_cast<T*>(1ll);
    EXPECT_EQ(vec.copyN(1, 0, nullptr, &p), Status::NoError);
    EXPECT_EQ(p, nullptr);

    // try to copy with wrong offset
    p = nullptr;
    EXPECT_EQ(vec.copyN(3, 1, another_data_array.get(), &p), Status::ErrorOverflow);
    EXPECT_EQ(p, nullptr);
    for (size_type i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], another_data_array.get()[i]);

    // copy offset > size && n == 0
    p = nullptr;
    EXPECT_EQ(vec.copyN(4, 0, another_data_array.get(), &p), Status::ErrorOverflow);
    EXPECT_EQ(p, nullptr);

    // copy offset == size && n == 0
    p = nullptr;
    EXPECT_EQ(vec.copyN(3, 0, another_data_array.get(), &p), Status::NoError);
    EXPECT_EQ(p, another_data_array.get());

    // try to copy with nullptr as destination
    p = nullptr;
    EXPECT_EQ(vec.copyN(1, 3, nullptr, &p), Status::ErrorInvalidArgument);
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

TEST(VectorTest, CopyNErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;

    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});

    ErrorProne ep[1] = { {} };

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidHash;
    EXPECT_EQ(vec.copyN(0, 1, ep), Status::ErrorInvalidHash);
}

template<typename T>
void FCopyNIt()
{
    auto vec = getStringsFilledContainer<T>();

    T another_data_array[3] = { T{}, T{}, T{} };
    std::list<T> l(another_data_array, another_data_array + 3);

    auto it = l.end();

    EXPECT_EQ(vec.copyN(vec.begin(), vec.end(), l.begin(), &it), Status::NoError);
    EXPECT_EQ(it, l.end());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    EXPECT_EQ(vec.copyN(vec.begin() + 1, vec.end(), l.begin(), &it), Status::NoError); 
    auto tempIt = ++l.begin();
    EXPECT_EQ(it, ++tempIt);

    tempIt = l.begin();
    EXPECT_EQ(vec[1], *tempIt);
    EXPECT_EQ(vec[2], *++tempIt);
    EXPECT_EQ(vec[2], *++tempIt);

    // try to not pass optional arg
    EXPECT_EQ(vec.copyN(vec.begin(), vec.begin(), l.begin()), Status::NoError);

    // copy more than vec has
    EXPECT_EQ(vec.copyN(vec.begin(), vec.end() + 1, l.begin(), &it), Status::NoError);
    EXPECT_EQ(it, l.end());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    // copy zero elements
    it = l.begin();
    EXPECT_EQ(vec.copyN(vec.begin(), vec.begin(), l.begin(), &it), Status::NoError);
    EXPECT_EQ(it, l.begin());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    // try invalid range
    EXPECT_EQ(vec.copyN(vec.begin() + 1, vec.begin(), l.begin(), &it), Status::ErrorOverflow);
    EXPECT_EQ(it, l.begin());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), l.begin()));

    // try to copy with wrong start offset
    EXPECT_EQ(vec.copyN(vec.end(), vec.end(), l.begin(), &it), Status::ErrorInvalidArgument);
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

TEST(VectorTest, CopyNItErrorPropagation)
{
    Vector<ErrorProne, DefaultAllocationManager<ErrorProne>> vec;

    ErrorProne::errorOnCounter = 100;
    vec.pushBack(ErrorProne{});

    std::list<ErrorProne> list;
    list.push_back(ErrorProne{});

    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorInvalidHash;
    EXPECT_EQ(vec.copyN(vec.begin(), vec.end(), list.begin()), Status::ErrorInvalidHash);
}

TEST(VectorTest, Data)
{
    Vector<std::string, DefaultAllocationManager<std::string>> vec;

    EXPECT_EQ(vec.data(), nullptr);

    vec.pushBackN(g_data_array<std::string>, 3);

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
    Vector<std::string, DefaultAllocationManager<std::string>> vec;

    EXPECT_EQ(vec.size(), 0);

    vec.pushBackN(g_data_array<std::string>, 3);

    EXPECT_EQ(vec.size(), 3);
}

TEST(VectorTest, MaxSize)
{
    Vector<std::string, DefaultAllocationManager<std::string>> vec1;

    EXPECT_EQ(vec1.max_size(), (ConstructorNoexceptAllocator<std::string>().max_size()));

    Vector<uint8_t, RawStratAllocationManagerT<uint8_t>> vec2;

    EXPECT_EQ(vec2.max_size(), RawNoexceptAllocator<uint8_t>().max_size());
}

TEST(VectorTest, Capacity)
{
    Vector<std::string, DefaultAllocationManager<std::string>> vec;

    EXPECT_EQ(vec.capacity(), 0);

    vec.getAllocationManager().setAllocationStrategy(AllocationStrategy::StrictByDataSize);

    vec.pushBackN(g_data_array<std::string>, 3);

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

TEST(VectorTest, GetAllocationManager)
{
    Vector<std::string, DefaultAllocationManager<std::string>> vec;

    auto& allocator = vec.getAllocationManager();

    EXPECT_TRUE((std::is_same_v<std::decay_t<decltype(allocator)>, DefaultAllocationManager<std::string>>));
    EXPECT_TRUE((std::is_lvalue_reference_v<decltype(allocator)>));
}

} // namespace
