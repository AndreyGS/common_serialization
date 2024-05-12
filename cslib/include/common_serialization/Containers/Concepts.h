/**
 * @file cslib/include/common_serialization/Containers/Concepts.h
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

#include "common_serialization/Containers/GenericPointerKeeper.h"

namespace common_serialization
{

class GenericPointerKeeper;

template<template<typename...> typename _Bc>
concept IBasicContainer
    =  requires(_Bc<int> b)
         {
             typename _Bc<int>::value_type;
             typename _Bc<int>::size_type;

             { b.begin() };
             { b.end() };

             { b.clear() };
             { b.size() } -> std::same_as<typename _Bc<int>::size_type>;
             
             { b.pushBack(typename _Bc<int>::value_type{}) } -> std::same_as<Status>;
         } && std::is_constructible_v<_Bc<int>, _Bc<int>>;

/// @brief Interface of container that holds
///     GenericPointerKeeper objects
template<typename _Gkc>
concept IGenericPointersKeeperContainer
    =  requires(_Gkc e)
         {
             typename _Gkc::value_type;
             typename _Gkc::constructor_allocator;

             { e.clear() };
             { e.begin() };
             { e.end() };
             { e.erase(0, 1) };
             { e.data() } -> std::same_as<typename _Gkc::value_type*>;
             { e.size() } -> std::same_as<typename _Gkc::size_type>;
             { e.capacity() } -> std::same_as<typename _Gkc::size_type>;

             { e.reserve(1) } -> std::same_as<Status>;
             { e.pushBack(*(new GenericPointerKeeper)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename _Gkc::value_type, GenericPointerKeeper> && std::is_same_v<typename _Gkc::constructor_allocator, std::true_type>;

template<typename _T>
concept HasDestroyingDeleteOp = requires (_T t) { _T::operator delete(&t, std::destroying_delete_t{}); };

template<typename _T1, typename _D1, typename _T2, typename _D2>
concept SmartPtrArrConvertible =std::is_convertible_v<_T2*, _T1*>
                            && (std::is_reference_v<_D1> && std::is_same_v<_D2, _D1> || !std::is_reference_v<_D1> && std::is_convertible_v<_D2, _D1>)
                            && (std::is_same_v<_T2, _T1> || std::has_virtual_destructor_v<_T1>);

template<typename _T1, typename _D1, typename _T2, typename _D2>
concept SmartPtrConvertible =   std::is_convertible_v<_T2*, _T1*>
                            && (std::is_reference_v<_D1> && std::is_same_v<_D2, _D1> || !std::is_reference_v<_D1> && std::is_convertible_v<_D2, _D1>)
                            && (std::is_same_v<_T2, _T1> || std::has_virtual_destructor_v<_T1> || HasDestroyingDeleteOp<_T1>);

} // namespace common_serialization
