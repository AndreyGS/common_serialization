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

/// @brief Test for possibility to init object of type _T
///     by another instance of the same type.
/// @remark Using to figure out can initialization of object be
///     replaced from copy-ctor to init function.
///     Primary for avoid exceptions and consequently 
///     init function should not throw.
template<typename _T>
concept Initable = requires(normalize_t<_T>& lhs, _T&& rhs)
{
    { lhs.init(std::forward<_T>(rhs)) } -> std::same_as<Status>;
};

/// @brief Test for possibility to init object of type _T
///     by special arguments
/// @remark Using to figure out can initialization of object be
///     replaced from copy-ctor to init function.
///     Primary for avoid exceptions and consequently 
///     init function should not throw.
template<typename _T, typename... _Args>
concept InitableBySpecialArgs = requires(_T t, _Args&&... args)
{
    { t.init(std::forward<_Args>(args)...) } -> std::same_as<Status>;
};

template<typename _T>
concept NotPointer = !(std::is_pointer_v<_T> || std::is_member_pointer_v<_T> || std::is_function_v<_T> || std::is_member_function_pointer_v<_T>);

template<typename _T>
concept HasLessOperator = requires (_T t1, _T t2) { { t1 < t2 } -> std::same_as<bool>; };

template<typename _T>
concept HasEqualityOperator = requires (_T t1, _T t2) { { t1 == t2 } -> std::same_as<bool>; };

template<typename _T>
concept EndiannessReversable = (std::is_arithmetic_v<_T> || std::is_enum_v<_T>) && sizeof(_T) > 1  && sizeof(_T) <= 8;

template<typename _T>
concept Signed
        =  std::is_arithmetic_v<_T>
                &&    (std::is_same_v<_T, char> || std::is_same_v<_T, signed char> || std::is_same_v<_T, short> || std::is_same_v<_T, int> || std::is_same_v<_T, long>
                    || std::is_same_v<_T, long long> || std::is_same_v<_T, float> || std::is_same_v<_T, double> || std::is_same_v<_T, long double>)
        || std::is_enum_v<_T>
                &&    (std::is_same_v<std::underlying_type_t<_T>, char> || std::is_same_v<std::underlying_type_t<_T>, signed char> 
                    || std::is_same_v<std::underlying_type_t<_T>, short> || std::is_same_v<std::underlying_type_t<_T>, int>
                    || std::is_same_v<std::underlying_type_t<_T>, long> || std::is_same_v<std::underlying_type_t<_T>, long long>);

template<typename _T>
concept FixSizedArithmeticType
    =  std::is_same_v<std::remove_cv_t<_T>, char8_t> || std::is_same_v<std::remove_cv_t<_T>, char16_t> || std::is_same_v<std::remove_cv_t<_T>, char32_t>
    || std::is_same_v<_T, float> || std::is_same_v<_T, double>;

template<typename _T>
concept FixSizedEnumType = std::is_enum_v<_T> && FixSizedArithmeticType<std::underlying_type_t<_T>>;

template<typename _T>
concept FixSizedArithmeticOrEnumType = FixSizedArithmeticType<_T> || FixSizedEnumType<_T>;

template<typename _T>
concept HasDestroyingDeleteOp = requires (_T t) { _T::operator delete(&t, std::destroying_delete_t{}); };

} // namespace common_serialization
