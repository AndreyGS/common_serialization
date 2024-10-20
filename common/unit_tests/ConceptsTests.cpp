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
    InitableTest() = default;
    InitableTest(const InitableTest&) {}
    Status init(const InitableTest&) { return Status::NoError; }
    Status init(int) { return Status::NoError; }
};

struct InitableTest2
{
    InitableTest2() = default;
    InitableTest2(const InitableTest2&) {}
    Status init(const InitableTest&) { return Status::NoError; }
};

struct InitableTest3
{
    Status init(InitableTest3&&) { return Status::NoError; }
    Status init(InitableTest3&&, int) { return Status::NoError; }
};

TEST(ConceptsTests, Initable)
{
    InitableTest initableTestLhs, initableTestRhs;
    initableTestLhs.init(initableTestRhs);
    EXPECT_TRUE(Initable<const InitableTest&>);
    EXPECT_TRUE(Initable<InitableTest>);

    initableTestLhs.init(std::move(initableTestRhs));
    EXPECT_TRUE(Initable<InitableTest&&>);

    EXPECT_FALSE(Initable<InitableTest2>);

    InitableTest3 initableTest3Lhs, initableTest3Rhs;
    initableTest3Lhs.init(std::move(initableTest3Rhs));
    EXPECT_TRUE(Initable<InitableTest3&&>);
    EXPECT_TRUE(Initable<InitableTest3>);

    EXPECT_FALSE(Initable<const InitableTest3&>);
}

TEST(ConceptsTests, InitableBySpecialArgs)
{
    EXPECT_TRUE((InitableBySpecialArgs<InitableTest, InitableTest>));
    EXPECT_FALSE((InitableBySpecialArgs<const InitableTest&, const InitableTest&>));
    EXPECT_TRUE((InitableBySpecialArgs<InitableTest&, const InitableTest&>));
    EXPECT_TRUE((InitableBySpecialArgs<InitableTest&, InitableTest&&>));

    EXPECT_FALSE((InitableBySpecialArgs<InitableTest, InitableTest2>));
    EXPECT_TRUE((InitableBySpecialArgs<InitableTest2, InitableTest>));
    EXPECT_TRUE((InitableBySpecialArgs<InitableTest, int>));
    EXPECT_FALSE((InitableBySpecialArgs<InitableTest2, int>));

    EXPECT_TRUE((InitableBySpecialArgs<InitableTest3&, InitableTest3, int>));
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

TEST(ConceptsTests, NotPointer)
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

TEST(ConceptsTests, HasLessOperator)
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

TEST(ConceptsTests, HasEqualityOperator)
{
    EXPECT_TRUE(HasEqualityOperator<WithEqOp>);
    EXPECT_TRUE(HasEqualityOperator<WithoutEqOp>);
    EXPECT_FALSE(HasEqualityOperator<WithoutEqOp2>);
}

struct Test { int i; };

enum TestEnumChar : char { etest0 };
enum TestEnumCharU : unsigned char { etest1 };
enum TestEnumCharS : signed char { etest2 };
enum TestEnumShort : short { etest3 };
enum TestEnumShortU : unsigned short { etest4 };
enum TestEnumInt : int { etest5 };
enum TestEnumIntU : unsigned int { etest6 };
enum TestEnumLong : long { etest7 };
enum TestEnumLongU : unsigned long { etest8 };
enum TestEnumLongLong : long long { etest9 };
enum TestEnumLongLongU : unsigned long long { etest10 };
enum TestEnumChar8 : char8_t { etest11 };
enum TestEnumChar16 : char16_t { etest12 };
enum TestEnumChar32 : char32_t { etest13 };

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
enum class TestEnumClassChar8 : char8_t { test11 };
enum class TestEnumClassChar16 : char16_t { test12 };
enum class TestEnumClassChar32 : char32_t { test13 };

TEST(ConceptsTests, EndiannessReversable)
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

    EXPECT_FALSE(EndiannessReversable<Test>);

    EXPECT_FALSE(EndiannessReversable<TestEnumChar>);
    EXPECT_FALSE(EndiannessReversable<TestEnumCharU>);
    EXPECT_FALSE(EndiannessReversable<TestEnumCharS>);
    EXPECT_TRUE(EndiannessReversable<TestEnumShort>);
    EXPECT_TRUE(EndiannessReversable<TestEnumShortU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumInt>);
    EXPECT_TRUE(EndiannessReversable<TestEnumIntU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumLong>);
    EXPECT_TRUE(EndiannessReversable<TestEnumLongU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumLongLong>);
    EXPECT_TRUE(EndiannessReversable<TestEnumLongLongU>);
    EXPECT_FALSE(EndiannessReversable<TestEnumChar8>);
    EXPECT_TRUE(EndiannessReversable<TestEnumChar16>);
    EXPECT_TRUE(EndiannessReversable<TestEnumChar32>);

    EXPECT_FALSE(EndiannessReversable<TestEnumClassChar>);
    EXPECT_FALSE(EndiannessReversable<TestEnumClassCharU>);
    EXPECT_FALSE(EndiannessReversable<TestEnumClassCharS>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassShort>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassShortU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassInt>);
    EXPECT_TRUE(EndiannessReversable<TestEnumIntU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassLong>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassLongU>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassLongLong>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassLongLongU>);
    EXPECT_FALSE(EndiannessReversable<TestEnumClassChar8>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassChar16>);
    EXPECT_TRUE(EndiannessReversable<TestEnumClassChar32>);
}

TEST(ConceptsTests, Signed)
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

    EXPECT_FALSE(Signed<Test>);

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
    EXPECT_FALSE(Signed<TestEnumChar8>);
    EXPECT_FALSE(Signed<TestEnumChar16>);
    EXPECT_FALSE(Signed<TestEnumChar32>);

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
    EXPECT_FALSE(Signed<TestEnumClassChar8>);
    EXPECT_FALSE(Signed<TestEnumClassChar16>);
    EXPECT_FALSE(Signed<TestEnumClassChar32>);
}

TEST(ConceptsTests, FixSizedArithmeticType)
{
    EXPECT_FALSE(FixSizedArithmeticType<char>);
    EXPECT_FALSE(FixSizedArithmeticType<signed char>);
    EXPECT_FALSE(FixSizedArithmeticType<unsigned char>);
    EXPECT_FALSE(FixSizedArithmeticType<short>);
    EXPECT_FALSE(FixSizedArithmeticType<unsigned short>);
    EXPECT_FALSE(FixSizedArithmeticType<int>);
    EXPECT_FALSE(FixSizedArithmeticType<unsigned int>);
    EXPECT_FALSE(FixSizedArithmeticType<long>);
    EXPECT_FALSE(FixSizedArithmeticType<unsigned long>);
    EXPECT_FALSE(FixSizedArithmeticType<long long>);
    EXPECT_FALSE(FixSizedArithmeticType<unsigned long long>);

    EXPECT_TRUE(FixSizedArithmeticType<char8_t>);
    EXPECT_TRUE(FixSizedArithmeticType<char16_t>);
    EXPECT_TRUE(FixSizedArithmeticType<char32_t>);

    EXPECT_TRUE(FixSizedArithmeticType<float>);
    EXPECT_TRUE(FixSizedArithmeticType<double>);
    EXPECT_FALSE(FixSizedArithmeticType<long double>);

    EXPECT_FALSE(FixSizedArithmeticType<Test>);

    EXPECT_FALSE(FixSizedArithmeticType<TestEnumChar>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumCharU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumCharS>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumShort>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumShortU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumInt>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumLong>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumLongU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumLongLong>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumLongLongU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumChar8>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumChar16>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumChar32>);

    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassChar>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassCharU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassCharS>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassShort>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassShortU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassInt>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassLong>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassLongU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassLongLong>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassLongLongU>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassChar8>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassChar16>);
    EXPECT_FALSE(FixSizedArithmeticType<TestEnumClassChar32>);
}

TEST(ConceptsTests, FixSizedEnumType)
{
    EXPECT_FALSE(FixSizedEnumType<char>);
    EXPECT_FALSE(FixSizedEnumType<signed char>);
    EXPECT_FALSE(FixSizedEnumType<unsigned char>);
    EXPECT_FALSE(FixSizedEnumType<short>);
    EXPECT_FALSE(FixSizedEnumType<unsigned short>);
    EXPECT_FALSE(FixSizedEnumType<int>);
    EXPECT_FALSE(FixSizedEnumType<unsigned int>);
    EXPECT_FALSE(FixSizedEnumType<long>);
    EXPECT_FALSE(FixSizedEnumType<unsigned long>);
    EXPECT_FALSE(FixSizedEnumType<long long>);
    EXPECT_FALSE(FixSizedEnumType<unsigned long long>);

    EXPECT_FALSE(FixSizedEnumType<char8_t>);
    EXPECT_FALSE(FixSizedEnumType<char16_t>);
    EXPECT_FALSE(FixSizedEnumType<char32_t>);

    EXPECT_FALSE(FixSizedEnumType<float>);
    EXPECT_FALSE(FixSizedEnumType<double>);
    EXPECT_FALSE(FixSizedEnumType<long double>);

    EXPECT_FALSE(FixSizedEnumType<Test>);

    EXPECT_FALSE(FixSizedEnumType<TestEnumChar>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumCharU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumCharS>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumShort>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumShortU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumInt>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumLong>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumLongU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumLongLong>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumLongLongU>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumChar8>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumChar16>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumChar32>);

    EXPECT_FALSE(FixSizedEnumType<TestEnumClassChar>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassCharU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassCharS>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassShort>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassShortU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassInt>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassLong>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassLongU>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassLongLong>);
    EXPECT_FALSE(FixSizedEnumType<TestEnumClassLongLongU>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumClassChar8>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumClassChar16>);
    EXPECT_TRUE(FixSizedEnumType<TestEnumClassChar32>);
}

TEST(ConceptsTests, FixSizedArithmeticOrEnumType)
{
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<char>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<signed char>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<unsigned char>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<short>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<unsigned short>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<int>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<unsigned int>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<long>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<unsigned long>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<long long>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<unsigned long long>);

    EXPECT_TRUE(FixSizedArithmeticOrEnumType<char8_t>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<char16_t>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<char32_t>);

    EXPECT_TRUE(FixSizedArithmeticOrEnumType<float>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<double>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<long double>);

    EXPECT_FALSE(FixSizedArithmeticOrEnumType<Test>);

    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumChar>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumCharU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumCharS>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumShort>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumShortU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumInt>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumLong>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumLongU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumLongLong>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumLongLongU>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumChar8>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumChar16>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumChar32>);

    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassChar>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassCharU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassCharS>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassShort>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassShortU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassInt>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumIntU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassLong>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassLongU>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassLongLong>);
    EXPECT_FALSE(FixSizedArithmeticOrEnumType<TestEnumClassLongLongU>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumClassChar8>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumClassChar16>);
    EXPECT_TRUE(FixSizedArithmeticOrEnumType<TestEnumClassChar32>);
}

TEST(ConceptsTests, HasDestroyingDeleteOp)
{
    // CustomDeleterStruct has destroying delete op
    EXPECT_TRUE(HasDestroyingDeleteOp<tests_special_types::CustomDeleterStruct>);
    // CustomDeleterStructDesc is descendant of sturct that has destroying delete op
    EXPECT_TRUE(HasDestroyingDeleteOp<tests_special_types::CustomDeleterStructDesc>);
    // VirtDistructorOwner has virtual destructor
    EXPECT_FALSE(HasDestroyingDeleteOp<tests_special_types::VirtDistructorOwner>);
    // VirtDistructorOwnerDesc is descendant of struct that has virtual destructor
    EXPECT_FALSE(HasDestroyingDeleteOp<tests_special_types::VirtDistructorOwnerDesc>);
    // ErrorProne has simple destructor
    EXPECT_FALSE(HasDestroyingDeleteOp<tests_special_types::ErrorProne>);
    // Test has no user-defined destructor
    EXPECT_FALSE(HasDestroyingDeleteOp<Test>);
    EXPECT_FALSE(HasDestroyingDeleteOp<int>);
    EXPECT_FALSE(HasDestroyingDeleteOp<TestEnumChar>);
    EXPECT_FALSE(HasDestroyingDeleteOp<TestEnumClassChar>);
}

} // namespace
