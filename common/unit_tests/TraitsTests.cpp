/**
 * @file common_serializaiton/common/unit_tests/TraitsTests.cpp
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
#include <common_serialization/common/traits.h>
#include <common_serialization/tests_special_types/structs.h>

using namespace common_serialization;

namespace
{

template<typename T>
struct CrtpTest {};

template<typename T>
struct CrtpTest2 {};

TEST(ConceptsTests, GetCrtpMainType_)
{
    EXPECT_TRUE((std::is_same_v<CrtpTest<void>, GetCrtpMainType<CrtpTest<void>, void>>));
    EXPECT_FALSE((std::is_same_v<CrtpTest<void>, GetCrtpMainType<CrtpTest<CrtpTest2<void>>, void>>));
}

struct TestMemberPointer
{
    int m_i;
    static constexpr short m_j{ 0 };
};

TEST(ConceptsTests, RemoveMemberPointerT)
{
    // Removing pointer from TestMemberPointer object member
    EXPECT_TRUE((std::is_same_v<remove_member_pointer_t<decltype(&TestMemberPointer::m_i)>, int>));
    // Not removing any pointer from static member - returns original type
    EXPECT_TRUE((std::is_same_v<remove_member_pointer_t<decltype(&TestMemberPointer::m_j)>, const short*>));
    // Returns original type
    EXPECT_TRUE((std::is_same_v<remove_member_pointer_t<char>, char>));
}

TEST(ConceptsTests, FromPtrToConstToTtrT)
{
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int*>, int*>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int*>, int*>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int* const>, int*>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int* const>, int*>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int**>, int**>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int**>, int**>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int* const *>, int**>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int** const>, int**>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int* const * const>, int**>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<int***>, int***>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int***>, int***>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int* const **>, int***>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int* const * const *>, int***>));
    EXPECT_TRUE((std::is_same_v<from_ptr_to_const_to_ptr_t<const int* const* const* const>, int***>));

}

TEST(ConceptsTests, NormalizeT)
{
    struct Test {};
    const volatile Test test;
    const volatile Test& testR = test;

    EXPECT_TRUE((std::is_same_v<normalize_t<decltype(test)>, Test>));
    EXPECT_TRUE((std::is_same_v<normalize_t<decltype(testR)>, Test>));
    EXPECT_TRUE((std::is_same_v<normalize_t<decltype(std::move(test))>, Test>));
}

template<typename T> struct TestTempl {};
template<typename... Ts> struct TestTempl2 {};

TEST(ConceptsTests, IsTemplateV)
{
    EXPECT_TRUE(is_template_v<TestTempl<int>>);
    EXPECT_TRUE((is_template_v<TestTempl2<short, Test, int>>));

    struct Test {};
    EXPECT_FALSE(is_template_v<Test>);
}

} // namespace
