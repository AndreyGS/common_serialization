/**
 * @file common_serialization/common/concepts.h
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

#pragma once

#ifdef AGS_CS_NO_STD_LIB
#include <common_serialization/common/std_equivalents.h>
#else // AGS_CS_NO_STD_LIB
#include <concepts>
#include <type_traits>
#include <new>
#endif // #ifdef AGS_CS_NO_STD_LIB

#include <common_serialization/common/traits.h>
#include <common_serialization/common/Status.h>

namespace common_serialization
{

/// @brief Test for possibility to init object of type T
///     by another instance of the same type.
/// @remark Using to figure out can initialization of object be
///     replaced from copy-ctor to init function.
///     Primary for avoid exceptions and consequently 
///     init function should not throw.
template<typename T>
concept Initable = requires(normalize_t<T>& lhs, T&& rhs)
{
    { lhs.init(std::forward<T>(rhs)) } -> std::same_as<Status>;
};

/// @brief Test for possibility to init object of type T
///     by special arguments
/// @remark Using to figure out can initialization of object be
///     replaced from copy-ctor to init function.
///     Primary for avoid exceptions and consequently 
///     init function should not throw.
template<typename T, typename... Args>
concept InitableBySpecialArgs = requires(T t, Args&&... args)
{
    { t.init(std::forward<Args>(args)...) } -> std::same_as<Status>;
};

template<typename T>
concept NotPointer = !(std::is_pointer_v<T> || std::is_member_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>);

template<typename T>
concept HasLessOperator = requires (T t1, T t2) { { t1 < t2 } -> std::same_as<bool>; };

template<typename T>
concept HasEqualityOperator = requires (T t1, T t2) { { t1 == t2 } -> std::same_as<bool>; };

template<typename T>
concept EndiannessReversable = (std::is_arithmetic_v<T> || std::is_enum_v<T>) && sizeof(T) > 1  && sizeof(T) <= 8;

template<typename T>
concept Signed
        =  std::is_arithmetic_v<T>
                &&    (std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_same_v<T, long>
                    || std::is_same_v<T, long long> || std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>)
        || std::is_enum_v<T>
                &&    (std::is_same_v<std::underlying_type_t<T>, char> || std::is_same_v<std::underlying_type_t<T>, signed char> 
                    || std::is_same_v<std::underlying_type_t<T>, short> || std::is_same_v<std::underlying_type_t<T>, int>
                    || std::is_same_v<std::underlying_type_t<T>, long> || std::is_same_v<std::underlying_type_t<T>, long long>);

template<typename T>
concept FixSizedArithmeticType
    =  std::is_same_v<std::remove_cv_t<T>, char8_t> || std::is_same_v<std::remove_cv_t<T>, char16_t> || std::is_same_v<std::remove_cv_t<T>, char32_t>
    || std::is_same_v<T, float> || std::is_same_v<T, double>;

template<typename T>
concept FixSizedEnumType = std::is_enum_v<T> && FixSizedArithmeticType<std::underlying_type_t<T>>;

template<typename T>
concept FixSizedArithmeticOrEnumType = FixSizedArithmeticType<T> || FixSizedEnumType<T>;

template<typename T>
concept HasDestroyingDeleteOp = requires (T t) { T::operator delete(&t, std::destroying_delete_t{}); };

} // namespace common_serialization
