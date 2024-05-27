/**
 * @file ConstructorNoexceptAllocatorTests.cpp
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

using namespace special_types;
using namespace common_serialization;

namespace
{

TEST(ConstructorNoexceptAllocatorTests, MaxSizeT)
{
    ConstructorNoexceptAllocator<int> rna;
    EXPECT_EQ(rna.max_size(), static_cast<size_t>(-1) / sizeof(int));
    ConstructorNoexceptAllocator<std::string> rna2;
    EXPECT_EQ(rna2.max_size(), static_cast<size_t>(-1) / sizeof(std::string));
}

TEST(ConstructorNoexceptAllocatorTests, AllocateT)
{
    ConstructorNoexceptAllocator<std::string> rna;
    std::string* p = rna.allocate(5);
    EXPECT_TRUE(p != nullptr);
    rna.deallocate(p);

    std::string* pFail = rna.allocate(0xffffffffffffffff);
    EXPECT_TRUE(pFail == nullptr);
}

TEST(ConstructorNoexceptAllocatorTests, ConstructT)
{
    ConstructorNoexceptAllocator<std::string> rna;
    std::string* p = rna.allocate(1);
    EXPECT_EQ(rna.construct(p, "123"), Status::NoError);
    EXPECT_EQ(*p, "123");
    rna.destroy(p);
    rna.deallocate(p);

    // Test constructor for initable
    ConstructorNoexceptAllocator<ErrorProne> rna2;
    ErrorProne* p2 = rna2.allocate(2);
    ErrorProne ref;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1;
    ErrorProne::currentError = Status::ErrorInternal;
    EXPECT_EQ(rna2.construct(p2, ref), Status::NoError);
    EXPECT_EQ(rna2.construct(p2 + 1, ref), Status::ErrorInternal);
    rna2.destroy(p2);
    rna2.destroy(p2 + 1);
    rna2.deallocate(p2);
}

TEST(ConstructorNoexceptAllocatorTests, DestroyT)
{
    ConstructorNoexceptAllocator<ErrorProne> rna;
    ErrorProne* p = rna.allocate(1);
    ErrorProne::destructorCalledCounter = 0;
    ErrorProne::counter = 0;
    ErrorProne::errorOnCounter = 1;
    ErrorProne ref;
    EXPECT_EQ(rna.construct(p, ref), Status::NoError);
    rna.destroy(p);
    EXPECT_EQ(ErrorProne::destructorCalledCounter, 1);
    rna.deallocate(p);
}

} // namespace