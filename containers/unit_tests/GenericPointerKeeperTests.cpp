/**
 * @file common_serializaiton/containers/unit_tests/GenericPointerKeeperTests.cpp
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
#include <utility>
#include <common_serialization/containers/containers_config.h>
#include <common_serialization/containers/GenericPointerKeeper.h>
#include <common_serialization/tests_special_types/structs.h>

namespace
{

using namespace tests_special_types;
using namespace common_serialization;

TEST(GenericPointerKeeperTests, MoveCtor)
{
    GenericPointerKeeper gpk1;
    int* p = gpk1.allocateAndConstructOne<int>(5);
    GenericPointerKeeper gpk2(std::move(gpk1));
    EXPECT_EQ(gpk2.get<int>(), p);
    EXPECT_EQ(gpk1.get<int>(), nullptr);
}

TEST(GenericPointerKeeperTests, MoveAssignmentOperator)
{
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 111111;
    
    GenericPointerKeeper gpk1;
    ErrorProne* p1 = gpk1.allocateAndConstructOne<ErrorProne>();
    GenericPointerKeeper gpk2;
    ErrorProne* p2 = gpk2.allocateAndConstructOne<ErrorProne>();

    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;

    gpk2 = std::move(gpk1);
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 1);
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 0);
    EXPECT_EQ(gpk2.get<ErrorProne>(), p1);
    EXPECT_EQ(gpk1.get<ErrorProne>(), nullptr);
}

TEST(GenericPointerKeeperTests, Dtor)
{
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 111111;

    GenericPointerKeeper gpk1;
    ErrorProne* p1 = gpk1.allocateAndConstructOne<ErrorProne>();
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    gpk1.~GenericPointerKeeper();

    EXPECT_EQ(ErrorProne::destructorCalledCounter, 1);
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 0);
}

TEST(GenericPointerKeeperTests, AllocateAndConstruct)
{
    GenericPointerKeeper gpk;
    ErrorProne ref;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 111111;
    ErrorProne* p1 = gpk.allocateAndConstruct<CtorGenAllocationManagerT<ErrorProne>>(2, ref);
    EXPECT_EQ(p1, gpk.get<ErrorProne>());
    EXPECT_EQ(2, gpk.size());
    EXPECT_EQ(reinterpret_cast<typename GenericPointerKeeper::DestroyAndDeallocateFunc>(GenericPointerKeeper::destroyAndDeallocateHelper<CtorGenAllocationManagerT<ErrorProne>>), gpk.destroyer());
    
    // Check that previous items are destroyed
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::sumOfDeletedIndexes = 0;
    int* p2 = gpk.allocateAndConstruct<RawGenAllocationManagerT<int>>(1);
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 2);
    // 0 + 1
    EXPECT_EQ(ErrorProne::sumOfDeletedIndexes, 1);

    // Check new values
    EXPECT_EQ(p2, gpk.get<int>());
    EXPECT_EQ(1, gpk.size());
    EXPECT_EQ(reinterpret_cast<typename GenericPointerKeeper::DestroyAndDeallocateFunc>(GenericPointerKeeper::destroyAndDeallocateHelper<RawGenAllocationManagerT<int>>), gpk.destroyer());
}

TEST(GenericPointerKeeperTests, AllocateAndConstructErrorOnMaxSize)
{
    GenericPointerKeeper gpk;
    int* p = gpk.allocateAndConstruct<RawKeeperGenAllocationManagerT<int>>(2);
    EXPECT_EQ(p, nullptr);
    EXPECT_EQ(0, gpk.size());
    EXPECT_EQ(reinterpret_cast<typename GenericPointerKeeper::DestroyAndDeallocateFunc>(0), gpk.destroyer());
}

} // namespace