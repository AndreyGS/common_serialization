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

    manager.deallocate(p);
}

TEST(GenericAllocationManagerTests, AllocateAndConstructErrorInProcess)
{
    GenericAllocationManager<ConstructorNoexceptAllocatorT<tests_special_types::ErrorProne>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    // Check that if there was an error in construction process, objects that were already constructed are destroyed properly
    
    // Let third object construction will fail
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

} // namespace