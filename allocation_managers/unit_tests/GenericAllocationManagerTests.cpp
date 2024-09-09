/**
 * @file common_serializaiton/allocation_managers/unit_tests/GenericAllocationManagerTests.cpp
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
#include <string>
#include <tuple>
#include <common_serialization/allocation_managers/allocation_managers_config.h>
#include <common_serialization/allocation_managers/GenericAllocationManager.h>
#include <common_serialization/tests_special_types/structs.h>

using namespace tests_special_types;
using namespace common_serialization;

namespace
{

TEST(GenericAllocationManagerTests, AllocateAndConstructRaw)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    int* p = manager.allocateAndConstruct(requestedN, &allocatedN);
    EXPECT_TRUE(p != nullptr);
    EXPECT_EQ(requestedN, allocatedN);

    manager.deallocate(p);
    constexpr int kArg{ 7 };
    allocatedN = 0;
    p = manager.allocateAndConstruct(requestedN, &allocatedN, kArg);

    EXPECT_EQ(requestedN, allocatedN);
    EXPECT_EQ(p[0], kArg);
    EXPECT_EQ(p[1], kArg);
    EXPECT_EQ(p[2], kArg);

    manager.deallocate(p);

    p = manager.allocateAndConstruct(requestedN, nullptr);
    EXPECT_TRUE(p != nullptr);
    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, AllocateAndConstructCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    std::string* p = manager.allocateAndConstruct(requestedN, &allocatedN);
    EXPECT_TRUE(p != nullptr);
    EXPECT_EQ(requestedN, allocatedN);

    manager.deallocate(p);
    const std::string kArg{ "123" };
    allocatedN = 0;
    p = manager.allocateAndConstruct(requestedN, &allocatedN, kArg);

    EXPECT_EQ(requestedN, allocatedN);
    EXPECT_EQ(p[0], kArg);
    EXPECT_EQ(p[1], kArg);
    EXPECT_EQ(p[2], kArg);

    manager.destroyAndDeallocate(p, requestedN);
}

TEST(GenericAllocationManagerTests, AllocateAndConstructErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    // Check that if there was an error in construction process, objects that were already constructed are destroyed properly
    
    // Let third object construction will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 2;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    ErrorProne arg;
    ErrorProne* p = manager.allocateAndConstruct(requestedN, &allocatedN, arg);
    EXPECT_TRUE(p == nullptr);
    EXPECT_EQ(allocatedN, 0);
    // Check that two desctructors were called
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2);
    // Check that those destructors were from first and second item (indexes 0 + 1)
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 1);
}

TEST(GenericAllocationManagerTests, Allocate)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    int* p = manager.allocate(requestedN, &allocatedN);
    EXPECT_TRUE(p != nullptr);
    EXPECT_EQ(requestedN, allocatedN);

    manager.deallocate(p);

    p = manager.allocate(requestedN, nullptr);
    EXPECT_TRUE(p != nullptr);
    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, AllocateStrict)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* p = manager.allocateStrict(requestedN);
    EXPECT_TRUE(p != nullptr);

    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, ConstructRaw)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 1 };
    int* p = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.construct(p), Status::NoError);

    constexpr int kArg{ 7 };
    EXPECT_EQ(manager.construct(p, kArg), Status::NoError);
    EXPECT_EQ(p[0], kArg);

    manager.destroyAndDeallocate(p, requestedN);
}

TEST(GenericAllocationManagerTests, ConstructCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::tuple<int, std::string, short>>> manager;
    size_t requestedN{ 1 };
    std::tuple<int, std::string, short>* p = manager.allocateStrict(requestedN);

    constexpr int kArg0{ 124152 };
    const std::string kArg1{ "123"};
    constexpr int kArg2{ 34 };
    EXPECT_EQ(manager.construct(p, kArg0, kArg1, kArg2), Status::NoError);
    EXPECT_EQ(std::get<0>(*p), kArg0);
    EXPECT_EQ(std::get<1>(*p), kArg1);
    EXPECT_EQ(std::get<2>(*p), kArg2);

    manager.destroyAndDeallocate(p, requestedN);
}

TEST(GenericAllocationManagerTests, ConstructErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 1 };
    ErrorProne* p = manager.allocateStrict(requestedN);

    // Let object construction will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 0;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne arg;
    EXPECT_EQ(manager.construct(p, arg), Status::ErrorDataCorrupted);

    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, ConstructNRaw)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* p = manager.allocateStrict(requestedN);

    constexpr int kArg{ 7 };
    EXPECT_EQ(manager.constructN(p, requestedN, kArg), Status::NoError);
    EXPECT_EQ(p[0], kArg);
    EXPECT_EQ(p[1], kArg);
    EXPECT_EQ(p[2], kArg);

    manager.deallocate(p);

    // Check that constructor is called with default parameters
    p = manager.allocateStrict(requestedN);
    p[0] = 1;
    p[1] = 2;
    p[2] = 3;
    constexpr int i{};
    EXPECT_EQ(manager.constructN(p, requestedN), Status::NoError);
    EXPECT_EQ(p[0], i);
    EXPECT_EQ(p[1], i);
    EXPECT_EQ(p[2], i);

    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, ConstructNCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* p = manager.allocateStrict(requestedN);

    const std::string kArg{ "123" };
    EXPECT_EQ(manager.constructN(p, requestedN, kArg), Status::NoError);
    EXPECT_EQ(p[0], kArg);
    EXPECT_EQ(p[1], kArg);
    EXPECT_EQ(p[2], kArg);

    manager.destroyAndDeallocate(p, requestedN);
}

TEST(GenericAllocationManagerTests, ConstructNErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    // Check that if there was an error in construction process, objects that were already constructed are destroyed properly

    // Let third object construction will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 2;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    ErrorProne arg;
    ErrorProne* p = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.constructN(p, 3, arg), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2);
    // Check that those destructors were from first and second item (indexes 0 + 1)
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 1);
}

TEST(GenericAllocationManagerTests, CopyToRawNoOverlap)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.copyToRawNoOverlap(pDest, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, CopyToRawNoOverlapCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN, kArg);

    std::string* pDest = manager.allocateStrict(requestedN);
    EXPECT_EQ(manager.copyToRawNoOverlap(pDest, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");
    EXPECT_EQ(pDest[2], "123");

    manager.destroyAndDeallocate(pSrc, requestedN);
    manager.destroyAndDeallocate(pDest, requestedN);
}

TEST(GenericAllocationManagerTests, CopyToRawNoOverlapErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne arg;
    manager.constructN(pSrc, 3, arg);

    ErrorProne* pDest = manager.allocateStrict(requestedN);
    // Let third object copying will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 2;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.copyToRawNoOverlap(pDest, pSrc, requestedN), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2);
    // Check that those destructors were from first and second item (indexes 0 + 1)
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 1);

    manager.destroyAndDeallocate(pSrc, requestedN);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, CopyToDirty)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = pSrc;

    EXPECT_EQ(manager.copyToDirty(pDest, pDest + 3, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
}

TEST(GenericAllocationManagerTests, CopyToDirtyCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN - 1, kArg);

    std::string* pDest = pSrc + 1;
    EXPECT_EQ(manager.copyToDirty(pDest, pDest + 1, pSrc, requestedN - 1), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");

    // Check that pSrc[0] element was keeped safe
    EXPECT_EQ(pSrc[0], "123");

    manager.destroyAndDeallocate(pSrc, requestedN);
}

TEST(GenericAllocationManagerTests, CopyToDirtyErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 4 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne::counter = 0;
    // Let third object copying will fail (3 objects would be constructed in pSrc, and 2 will be copied before fail)
    ErrorProne::errorOnCounter = 5;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne arg;
    manager.constructN(pSrc, requestedN - 1, arg);

    ErrorProne* pDest = pSrc + 1;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.copyToDirty(pDest, pDest + 2, pSrc, requestedN - 1), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    // 2 times destructor called on overlapping regions and 2 times after error on already copied objects
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2 + 2);
    // Check that those destructors were called for items 
    // pSrc[2] sum += 2, pSrc[1] sum += 1, pDest[2] sum += 3, pDest[1] sum +=4
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 10);

    manager.destroyAndDeallocate(pSrc, 1);
}

TEST(GenericAllocationManagerTests, CopyToDirtyNoOverlap)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.copyToDirtyNoOverlap(pDest, pDest + 3, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, CopyToDirtyNoOverlapCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN, kArg);

    std::string* pDest = manager.allocateStrict(requestedN);
    manager.constructN(pDest, requestedN - 1, kArg);

    EXPECT_EQ(manager.copyToDirtyNoOverlap(pDest, pDest + 2, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");
    EXPECT_EQ(pDest[2], "123");

    manager.destroyAndDeallocate(pSrc, requestedN);
    manager.destroyAndDeallocate(pDest, requestedN);
}

TEST(GenericAllocationManagerTests, CopyToDirtyNoOverlapErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne arg;
    manager.constructN(pSrc, requestedN, arg);
    ErrorProne* pDest = manager.allocateStrict(requestedN);

    // Let third object copying will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 4;
    manager.constructN(pDest, requestedN - 1, arg);

    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.copyToDirtyNoOverlap(pDest, pDest + 2, pSrc, requestedN), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    // 2 times destructor called on inited before regions and 2 times after error on already copied objects
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2 + 2);
    // Check that those destructors were called for items 
    // pDest[0] sum += 0, pDest[1] sum += 1, pDest[0] sum += 2, pDest[1] sum += 3
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 6);

    manager.destroyAndDeallocate(pSrc, requestedN);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, MoveToRawNoOverlap)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.moveToRawNoOverlap(pDest, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, MoveToRawNoOverlapCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN, kArg);

    std::string* pDest = manager.allocateStrict(requestedN);
    EXPECT_EQ(manager.moveToRawNoOverlap(pDest, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");
    EXPECT_EQ(pDest[2], "123");

    manager.deallocate(pSrc);
    manager.destroyAndDeallocate(pDest, 3);
}

TEST(GenericAllocationManagerTests, MoveToRawNoOverlapErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 111111;
    ErrorProne::currentError = Status::NoError;
    ErrorProne arg;
    manager.constructN(pSrc, 3, arg);

    ErrorProne* pDest = manager.allocateStrict(requestedN);
    // Let third object copying will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 2;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.moveToRawNoOverlap(pDest, pSrc, requestedN), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2);
    // pDest[0] sum += 0 pDest[1] += 1
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 1);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, MoveToDirty)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = pSrc;

    EXPECT_EQ(manager.moveToDirty(pDest, pDest + 3, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
}

TEST(GenericAllocationManagerTests, MoveToDirtyCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 4 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN, kArg);

    std::string* pDest = pSrc + 1;
    EXPECT_EQ(manager.moveToDirty(pDest, pDest + 3, pSrc, requestedN - 1), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");
    EXPECT_EQ(pDest[2], "123");

    manager.destroyN(pDest, requestedN - 1);
    manager.deallocate(pSrc);
}

TEST(GenericAllocationManagerTests, MoveToDirtyErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 6 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne::counter = 0;
    // Let forth object moving will fail (5 objects would be constructed in pSrc, and 3 will be moved before fail)
    ErrorProne::errorOnCounter = 8;
    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne arg;
    manager.constructN(pSrc, 5, arg);

    ErrorProne* pDest = pSrc + 2;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.moveToDirty(pDest, pDest + 3, pSrc, 4), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    // 3 times destructor called on overlapping regions and 3 times after error on already moved objects
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 3 + 3);
    // Check that those destructors were called for items 
    // pSrc[4] sum += 4, pSrc[3] sum += 3, pSrc[2] sum += 2, pDest[4] sum += 5, pDest[3] sum += 6, pDest[2] sum += 7
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 27);

    manager.destroyAndDeallocate(pSrc, 2);
}

TEST(GenericAllocationManagerTests, MoveToDirtyNoOverlap)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.moveToDirtyNoOverlap(pDest, pDest + 3, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, MoveToDirtyNoOverlapCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    const std::string kArg{ "123" };
    manager.constructN(pSrc, requestedN, kArg);

    std::string* pDest = manager.allocateStrict(requestedN);
    manager.constructN(pDest, requestedN - 1, kArg);

    EXPECT_EQ(manager.moveToDirtyNoOverlap(pDest, pDest + 2, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], "123");
    EXPECT_EQ(pDest[1], "123");
    EXPECT_EQ(pDest[2], "123");

    manager.deallocate(pSrc);
    manager.destroyAndDeallocate(pDest, requestedN);
}

TEST(GenericAllocationManagerTests, MoveToDirtyNoOverlapErrorOnConstruction)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne arg;
    manager.constructN(pSrc, requestedN, arg);
    ErrorProne* pDest = manager.allocateStrict(requestedN);

    // Let third object moving will fail
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 4;
    manager.constructN(pDest, requestedN - 1, arg);

    ErrorProne::currentError = Status::ErrorDataCorrupted;
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    EXPECT_EQ(manager.moveToDirtyNoOverlap(pDest, pDest + 2, pSrc, requestedN), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    // 2 times destructor called on inited before regions and 2 times after error on already copied objects
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2 + 2);
    // Check that those destructors were called for items 
    // pDest[0] sum += 0, pDest[1] sum += 1, pDest[0] sum += 2, pDest[1] sum += 3
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 6);

    manager.destroyN(pSrc + 2, 1);
    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, DestroyAndDeallocate)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1111111;
    ErrorProne arg;
    manager.constructN(pSrc, requestedN, arg);

    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    manager.destroyAndDeallocate(pSrc, requestedN);

    // Check that destructor called three times
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 3);
    // Check that those destructors were called for items 
    // pSrc[0] sum += 0, pSrc[1] sum += 1, pSrc[2] sum += 2
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 3);
}

TEST(GenericAllocationManagerTests, Destroy)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 1 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1111111;
    ErrorProne arg;
    manager.construct(pSrc, arg);

    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    manager.destroy(pSrc);

    // Check that destructor called three times
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 1);
    // Check that those destructors were called for items 
    // pSrc[0] sum += 0 2
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 0);

    manager.deallocate(pSrc);
}

TEST(GenericAllocationManagerTests, DestroyN)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<ErrorProne>> manager;
    size_t requestedN{ 3 };

    ErrorProne* pSrc = manager.allocateStrict(requestedN);
    ErrorProne::currentError = Status::NoError;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1111111;
    ErrorProne arg;
    manager.constructN(pSrc, requestedN, arg);

    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    manager.destroyN(pSrc, requestedN);

    // Check that destructor called three times
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 3);
    // Check that those destructors were called for items 
    // pSrc[0] sum += 0, pSrc[1] sum += 1, pSrc[2] sum += 2
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 3);

    manager.deallocate(pSrc);
}

} // namespace