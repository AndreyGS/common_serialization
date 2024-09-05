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
#include <common_serialization/allocation_managers/GenericAllocationManager.h>
#include <common_serialization/allocation_managers/typedefs.h>
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

    manager.destroyAndDeallocate(p, 3);
}

TEST(GenericAllocationManagerTests, AllocateAndConstructErrorInProcess)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<tests_special_types::ErrorProne>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    // Check that if there was an error in construction process, objects that were already constructed are destroyed properly
    
    // Let third object construction will fail
    tests_special_types::ErrorProne::counter = 0;
    tests_special_types::ErrorProne::errorOnCounter = 2;
    tests_special_types::ErrorProne::currentError = Status::ErrorDataCorrupted;
    tests_special_types::ErrorProne::destructorCalledCounter = 0;
    tests_special_types::ErrorProne::sumOfDeletedIndexes = 0;
    tests_special_types::ErrorProne arg;
    tests_special_types::ErrorProne* p = manager.allocateAndConstruct(requestedN, &allocatedN, arg);
    EXPECT_TRUE(p == nullptr);
    EXPECT_EQ(allocatedN, 0);
    // Check that two desctructors were called
    EXPECT_EQ(tests_special_types::ErrorProne::destructorCalledCounter, 2);
    // Check that those destructors were from first and second item (indexes 0 + 1)
    EXPECT_EQ(tests_special_types::ErrorProne::sumOfDeletedIndexes, 1);
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

    manager.destroyAndDeallocate(p, 1);
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

    manager.destroyAndDeallocate(p, 1);
}

TEST(GenericAllocationManagerTests, ConstructError)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<tests_special_types::ErrorProne>> manager;
    size_t requestedN{ 1 };
    tests_special_types::ErrorProne* p = manager.allocateStrict(requestedN);

    // Let object construction will fail
    tests_special_types::ErrorProne::counter = 0;
    tests_special_types::ErrorProne::errorOnCounter = 0;
    tests_special_types::ErrorProne::currentError = Status::ErrorDataCorrupted;
    tests_special_types::ErrorProne arg;
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

    manager.destroyAndDeallocate(p, 3);
}

TEST(GenericAllocationManagerTests, ConstructNErrorInProcess)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<tests_special_types::ErrorProne>> manager;
    size_t requestedN{ 3 };

    // Check that if there was an error in construction process, objects that were already constructed are destroyed properly

    // Let third object construction will fail
    tests_special_types::ErrorProne::counter = 0;
    tests_special_types::ErrorProne::errorOnCounter = 2;
    tests_special_types::ErrorProne::currentError = Status::ErrorDataCorrupted;
    tests_special_types::ErrorProne::destructorCalledCounter = 0;
    tests_special_types::ErrorProne::sumOfDeletedIndexes = 0;
    tests_special_types::ErrorProne arg;
    tests_special_types::ErrorProne* p = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.constructN(p, 3, arg), Status::ErrorDataCorrupted);
    // Check that two desctructors were called
    EXPECT_EQ(tests_special_types::ErrorProne::destructorCalledCounter, 2);
    // Check that those destructors were from first and second item (indexes 0 + 1)
    EXPECT_EQ(tests_special_types::ErrorProne::sumOfDeletedIndexes, 1);
}

TEST(GenericAllocationManagerTests, CopyToRaw)
{
    GenericAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };

    int* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    int* pDest = manager.allocateStrict(requestedN);

    EXPECT_EQ(manager.copyToRaw(pDest, pSrc, requestedN), Status::NoError);
    EXPECT_EQ(pDest[0], 1);
    EXPECT_EQ(pDest[1], 2);
    EXPECT_EQ(pDest[2], 3);

    manager.deallocate(pSrc);
    manager.deallocate(pDest);
}

TEST(GenericAllocationManagerTests, CopyToRawCtor)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<std::string>> manager;
    size_t requestedN{ 3 };

    std::string* pSrc = manager.allocateStrict(requestedN);
    pSrc[0] = 1;
    pSrc[1] = 2;
    pSrc[2] = 3;
    
}

} // namespace