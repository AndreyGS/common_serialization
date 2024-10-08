/**
 * @file common_serialization/common/traits.h
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
#include <cassert>
#include <concepts>
#include <type_traits>
#include <bit>
#endif // #ifdef AGS_CS_NO_STD_LIB

namespace common_serialization
{

struct Dummy {};

/// @brief Get most derived type in CRTP pattern
/// @note If derived type is void then this class is the most derived type.
///     If not - derived type is the most derived type.
/// @tparam X This class type
/// @tparam _T Derived type
template<typename _X, typename _T>
using GetCrtpMainType = std::conditional_t<std::is_same_v<_T, void>, _X, _T>;

template<typename _T>
struct remove_member_pointer
{
    using type = _T;
};

template<typename _C, typename _T>
struct remove_member_pointer<_T _C::*>
{
    using type = _T;
};

template<typename _T>
using remove_member_pointer_t = typename remove_member_pointer<_T>::type;

template<typename _T, std::size_t L>
struct _pointer_level_traits_impl
{
    static const std::size_t value = L;
    using from_ptr_to_const_to_ptr = std::remove_const_t<_T>;
};

template<typename _T, std::size_t L>
    requires std::is_pointer_v<_T>
struct _pointer_level_traits_impl<_T, L> : _pointer_level_traits_impl<std::remove_pointer_t<_T>, L + 1>
{
    using from_ptr_to_const_to_ptr = typename _pointer_level_traits_impl<std::remove_pointer_t<_T>, L + 1>::from_ptr_to_const_to_ptr*;
};

template<typename _T>
    requires std::is_pointer_v<_T>
struct _pointer_level_traits : _pointer_level_traits_impl<std::remove_pointer_t<_T>, 0>
{
    using from_ptr_to_const_to_ptr = typename _pointer_level_traits_impl<std::remove_pointer_t<_T>, 0>::from_ptr_to_const_to_ptr*;
};

/// @brief Erases all consts from pointer
/// @note Works only with pointers
/// @remark const int* const * const * const -> int***
template<typename _T>
    requires std::is_pointer_v<_T>
using from_ptr_to_const_to_ptr_t = typename _pointer_level_traits<_T>::from_ptr_to_const_to_ptr;

template<typename _T>
using normalize_t = std::remove_cv_t<std::remove_reference_t<_T>>;

template<typename _T>
class is_template : public std::false_type
{
};

template<template<typename...> typename _T, typename... _Ts>
class is_template<_T<_Ts...>> : public std::true_type
{
};

template<typename _T>
constexpr bool is_template_v = is_template<_T>::value;

} // namespace common_serialization
