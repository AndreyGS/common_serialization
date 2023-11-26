/**
 * @file cslib/include/common_serialization/Helpers.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

#include "common_serialization/Status.h"

#define NO_SERIALIZATION_NEED

namespace common_serialization
{

struct Dummy {};

template<typename X, typename T>
using GetCrtpMainType = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, X, T>;

template<typename T>
struct remove_member_pointer
{
    using type = T;
};

template<typename C, typename T>
struct remove_member_pointer<T C::*>
{
    using type = T;
};

template<typename T>
using remove_member_pointer_t = typename remove_member_pointer<T>::type;

template<typename T, std::size_t L>
struct pointer_level_traits_impl
{
    static const std::size_t value = L;
    using from_ptr_to_const_to_ptr = std::remove_const_t<T>;
};

template<typename T, std::size_t L>
    requires std::is_pointer_v<T>
struct pointer_level_traits_impl<T, L> : pointer_level_traits_impl<std::remove_pointer_t<T>, L + 1>
{
    using from_ptr_to_const_to_ptr = typename pointer_level_traits_impl<std::remove_pointer_t<T>, L + 1>::from_ptr_to_const_to_ptr*;
};

template<typename T>
    requires std::is_pointer_v<T>
struct pointer_level_traits : pointer_level_traits_impl<std::remove_pointer_t<T>, 0>
{
    using from_ptr_to_const_to_ptr = typename pointer_level_traits_impl<std::remove_pointer_t<T>, 0>::from_ptr_to_const_to_ptr*;
};

// erases all consts from ptr
// const int* const * const * const -> int***
template<typename T>
    requires std::is_pointer_v<T>
using from_ptr_to_const_to_ptr_t = typename pointer_level_traits<T>::from_ptr_to_const_to_ptr;

template<typename T>
concept IsNotPointer = !(std::is_pointer_v<T> || std::is_member_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>);

namespace helpers
{

template<typename T>
constexpr inline bool areRegionsOverlap(const T* objs1, const T* objs2, size_t n)
{
    return objs1 >= objs2 && objs1 < objs2 + n || objs1 <= objs2 && objs1 + n > objs2;
}

template<typename T, size_t N>
size_t countof(T(&arr)[N])
{
    return N;
}

} // namespace helpers

} // namespace common_serialization