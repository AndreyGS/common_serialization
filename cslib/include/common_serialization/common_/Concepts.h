/**
 * @file cslib/include/common_serialization/Concepts.h
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

namespace common_serialization
{

/// @brief Test for possibility to init object of type _T
///     by another instance of the same type.
/// @remark Using to figure out can initialization of object be
///     replaced from copy-ctor to init function.
///     Primary for avoid exceptions and consequently 
///     init function should not throw.
template<typename _T>
concept Initable = requires(_T t)
{
    { t.init(*(new _T)) } -> std::same_as<Status>;
};

/// @brief Test for possibility to init object of type _T
///     by another instance of type SpecClass.
template<typename _T, typename SpecClass>
concept InitableBySpecialClass = requires(_T t)
{
    { t.init(*(new SpecClass)) } -> std::same_as<Status>;
};

template<typename _T>
concept IsNotPointer = !(std::is_pointer_v<_T> || std::is_member_pointer_v<_T> || std::is_function_v<_T> || std::is_member_function_pointer_v<_T>);

template<typename _T>
concept IsEndiannessReversable = (std::is_arithmetic_v<_T> || std::is_enum_v<_T>) && sizeof(_T) > 1  && sizeof(_T) <= 8;

template<typename _T>
concept IsSigned
        =      std::is_arithmetic_v<_T>
        && (   std::is_same_v<_T, signed char> || std::is_same_v<_T, short> || std::is_same_v<_T, int> || std::is_same_v<_T, long>
            || std::is_same_v<_T, long long> || std::is_same_v<_T, float> || std::is_same_v<_T, double> || std::is_same_v<_T, long double>);

template<typename _T>
concept HasDestroyingDeleteOp = requires (_T t) { _T::operator delete(&t, std::destroying_delete_t{}); };

} // namespace common_serialization
