/**
 * @file common_serializaiton/allocation_managers/unit_tests/StrategicAllocationManagerTests.cpp
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
#include <common_serialization/allocation_managers/StrategicAllocationManager.h>
#include <common_serialization/tests_special_types/structs.h>

using namespace tests_special_types;
using namespace common_serialization;

namespace
{

TEST(StrategicAllocationManagerTests, Allocate)
{
    StrategicAllocationManager<RawNoexceptAllocatorT<int>> manager;
    size_t requestedN{ 3 };
    size_t allocatedN{ 0 };

    int* p = manager.allocate(requestedN, &allocatedN);
    EXPECT_TRUE(p != nullptr);
    EXPECT_EQ(requestedN * 2, allocatedN);

    manager.deallocate(p);

    manager.setAllocationStrategy(AllocationStrategy::StrictByDataSize);

    p = manager.allocate(requestedN, &allocatedN);

    EXPECT_TRUE(p != nullptr);
    EXPECT_EQ(requestedN, allocatedN);

    manager.deallocate(p);
}

TEST(StrategicAllocationManagerTests, GetAndSetAllocationStrategy)
{
    StrategicAllocationManager<RawNoexceptAllocatorT<int>> manager;
    EXPECT_EQ(manager.getAllocationStrategy(), AllocationStrategy::DoubleOfDataSize);
    manager.setAllocationStrategy(AllocationStrategy::StrictByDataSize);
    EXPECT_EQ(manager.getAllocationStrategy(), AllocationStrategy::StrictByDataSize);
}

} // namespace