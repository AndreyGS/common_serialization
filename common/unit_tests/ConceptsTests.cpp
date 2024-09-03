/**
 * @file common_serializaiton/common/unit_tests/ConceptsTests.cpp
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
#include <common_serialization/common/concepts.h>
#include <common_serialization/tests_special_types/structs.h>

using namespace common_serialization;

namespace
{

struct InitableTest
{
    InitableTest(const InitableTest&) {}
    Status init(const InitableTest&) { return Status::NoError; }
    Status init(int) { return Status::NoError; }
};

struct InitableTest2
{
    InitableTest2(const InitableTest2&) {}
    Status init(const InitableTest&) { return Status::NoError; }
};

TEST(ConceptsTests, Initable_)
{
    EXPECT_TRUE(Initable<InitableTest>);
    EXPECT_FALSE(Initable<InitableTest2>);
}

TEST(ConceptsTests, InitableBySpecialClass_)
{
    EXPECT_FALSE((InitableBySpecialClass<InitableTest, InitableTest2>));
    EXPECT_TRUE((InitableBySpecialClass<InitableTest2, InitableTest>));
    EXPECT_TRUE((InitableBySpecialClass<InitableTest, int>));
    EXPECT_FALSE((InitableBySpecialClass<InitableTest2, int>));
}

struct NotPointerTest
{
    static void f();
    void g();

    static constexpr int m_i{ 0 };
    static constexpr int* m_pI{ nullptr };
    int m_j;
    int* m_pJ;
};

void fTest() {}

TEST(ConceptsTests, NotPointer_)
{
    NotPointerTest nptest;

    EXPECT_TRUE(NotPointer<int>);
    EXPECT_TRUE(NotPointer<decltype(nptest)>);
    EXPECT_FALSE(NotPointer<int*>);
    EXPECT_FALSE(NotPointer<int**>);
    EXPECT_FALSE(NotPointer<int***>);
    EXPECT_FALSE(NotPointer<decltype(&nptest)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::f)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::g)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::m_i)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::m_j)>);
    EXPECT_FALSE(NotPointer<decltype(NotPointerTest::m_pI)>);
    EXPECT_FALSE(NotPointer<decltype(NotPointerTest::m_pJ)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::m_pI)>);
    EXPECT_FALSE(NotPointer<decltype(&NotPointerTest::m_pJ)>);
    EXPECT_FALSE(NotPointer<decltype(fTest)>);
}

struct WithLessOp
{
    bool operator<(const WithLessOp&) const { return true; }
};

struct WithoutLessOp
{
};

bool operator<(const WithoutLessOp&, const WithoutLessOp&) { return true; }

struct WithoutLessOp2
{
};

TEST(ConceptsTests, HasLessOperator_)
{
    EXPECT_TRUE(HasLessOperator<WithLessOp>);
    EXPECT_TRUE(HasLessOperator<WithoutLessOp>);
    EXPECT_FALSE(HasLessOperator<WithoutLessOp2>);
}

struct WithEqOp
{
    bool operator==(const WithEqOp&) const { return true; }
};

struct WithoutEqOp
{
};

bool operator==(const WithoutEqOp&, const WithoutEqOp&) { return true; }

struct WithoutEqOp2
{
};

TEST(ConceptsTests, HasEqualityOperator_)
{
    EXPECT_TRUE(HasEqualityOperator<WithEqOp>);
    EXPECT_TRUE(HasEqualityOperator<WithoutEqOp>);
    EXPECT_FALSE(HasEqualityOperator<WithoutEqOp2>);
}

TEST(ConceptsTests, EndiannessReversable_)
{
    EXPECT_FALSE(EndiannessReversable<char>);
    EXPECT_FALSE(EndiannessReversable<signed char>);
    EXPECT_FALSE(EndiannessReversable<unsigned char>);
    EXPECT_TRUE(EndiannessReversable<short>);
    EXPECT_TRUE(EndiannessReversable<unsigned short>);
    EXPECT_TRUE(EndiannessReversable<int>);
    EXPECT_TRUE(EndiannessReversable<unsigned int>);
    EXPECT_TRUE(EndiannessReversable<long>);
    EXPECT_TRUE(EndiannessReversable<unsigned long>);
    EXPECT_TRUE(EndiannessReversable<long long>);
    EXPECT_TRUE(EndiannessReversable<unsigned long long>);

    EXPECT_FALSE(EndiannessReversable<char8_t>);
    EXPECT_TRUE(EndiannessReversable<char16_t>);
    EXPECT_TRUE(EndiannessReversable<char32_t>);

    EXPECT_TRUE(EndiannessReversable<float>);
    EXPECT_TRUE(EndiannessReversable<double>);

    if constexpr (sizeof(long double) == 8)
        EXPECT_TRUE(EndiannessReversable<long double>);
    else
        EXPECT_FALSE(EndiannessReversable<long double>);

    struct Test { int i; };

    EXPECT_FALSE(EndiannessReversable<Test>);

    enum TestEnumSmall : uint8_t { testVal };

    EXPECT_FALSE(EndiannessReversable<TestEnumSmall>);

    enum TestEnum { testVal2 };

    EXPECT_TRUE(EndiannessReversable<TestEnum>);

    enum class TestEnumSmallClass : uint8_t { testVal };

    EXPECT_FALSE(EndiannessReversable<TestEnumSmallClass>);

    enum class TestEnumClass { testVal };

    EXPECT_TRUE(EndiannessReversable<TestEnumClass>);
}

TEST(ConceptsTests, Signed_)
{
    EXPECT_TRUE(Signed<char>);
    EXPECT_TRUE(Signed<signed char>);
    EXPECT_FALSE(Signed<unsigned char>);
    EXPECT_TRUE(Signed<short>);
    EXPECT_FALSE(Signed<unsigned short>);
    EXPECT_TRUE(Signed<int>);
    EXPECT_FALSE(Signed<unsigned int>);
    EXPECT_TRUE(Signed<long>);
    EXPECT_FALSE(Signed<unsigned long>);
    EXPECT_TRUE(Signed<long long>);
    EXPECT_FALSE(Signed<unsigned long long>);

    EXPECT_FALSE(Signed<char8_t>);
    EXPECT_FALSE(Signed<char16_t>);
    EXPECT_FALSE(Signed<char32_t>);

    EXPECT_TRUE(Signed<float>);
    EXPECT_TRUE(Signed<double>);
    EXPECT_TRUE(Signed<long double>);
   
    struct Test { int i; };

    EXPECT_FALSE(Signed<Test>);

    enum TestEnumChar : char { test0 };
    enum TestEnumCharU : unsigned char { test1 };
    enum TestEnumCharS : signed char { test2 };
    enum TestEnumShort : short { test3 };
    enum TestEnumShortU : unsigned short { test4 };
    enum TestEnumInt : int { test5 };
    enum TestEnumIntU : unsigned int { test6 };
    enum TestEnumLong : long { test7 };
    enum TestEnumLongU : unsigned long { test8 };
    enum TestEnumLongLong : long long { test9 };
    enum TestEnumLongLongU : unsigned long long { test10 };

    EXPECT_TRUE(Signed<TestEnumChar>);
    EXPECT_FALSE(Signed<TestEnumCharU>);
    EXPECT_TRUE(Signed<TestEnumCharS>);
    EXPECT_TRUE(Signed<TestEnumShort>);
    EXPECT_FALSE(Signed<TestEnumShortU>);
    EXPECT_TRUE(Signed<TestEnumInt>);
    EXPECT_FALSE(Signed<TestEnumIntU>);
    EXPECT_TRUE(Signed<TestEnumLong>);
    EXPECT_FALSE(Signed<TestEnumLongU>);
    EXPECT_TRUE(Signed<TestEnumLongLong>);
    EXPECT_FALSE(Signed<TestEnumLongLongU>);

    enum class TestEnumClassChar : char { test0 };
    enum class TestEnumClassCharU : unsigned char { test1 };
    enum class TestEnumClassCharS : signed char { test2 };
    enum class TestEnumClassShort : short { test3 };
    enum class TestEnumClassShortU : unsigned short { test4 };
    enum class TestEnumClassInt : int { test5 };
    enum class TestEnumClassIntU : unsigned int { test6 };
    enum class TestEnumClassLong : long { test7 };
    enum class TestEnumClassLongU : unsigned long { test8 };
    enum class TestEnumClassLongLong : long long { test9 };
    enum class TestEnumClassLongLongU : unsigned long long { test10 };

    EXPECT_TRUE(Signed<TestEnumClassChar>);
    EXPECT_FALSE(Signed<TestEnumClassCharU>);
    EXPECT_TRUE(Signed<TestEnumClassCharS>);
    EXPECT_TRUE(Signed<TestEnumClassShort>);
    EXPECT_FALSE(Signed<TestEnumClassShortU>);
    EXPECT_TRUE(Signed<TestEnumClassInt>);
    EXPECT_FALSE(Signed<TestEnumIntU>);
    EXPECT_TRUE(Signed<TestEnumClassLong>);
    EXPECT_FALSE(Signed<TestEnumClassLongU>);
    EXPECT_TRUE(Signed<TestEnumClassLongLong>);
    EXPECT_FALSE(Signed<TestEnumClassLongLongU>);
}

} // namespace
