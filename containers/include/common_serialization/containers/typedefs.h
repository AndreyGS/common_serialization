/**
 * @file common_serialization/containers/typedefs.h
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

#ifndef CS_CUSTOM_CONTAINERS_TYPEDEFS

#ifdef AGS_CS_NO_STD_LIB
#include TODO
#else // AGS_CS_NO_STD_LIB
#include <map>
#include <unordered_map> 
#include <list>
#endif // #ifdef AGS_CS_NO_STD_LIB


#include <common_serialization/containers/GenericPointerKeeper.h>
#include <common_serialization/containers/UniquePtr.h>
#include <common_serialization/containers/Vector.h>
#include <common_serialization/containers/Walker.h>

namespace common_serialization
{

using GenericPointerKeeperT = GenericPointerKeeper;

template<typename _T, typename... _Ts>
using UniquePtrT = UniquePtr<_T, _Ts...>;

template<typename _T, typename... _Ts>
using VectorT = Vector<_T, _Ts...>;

template<typename _T>
using RawVectorT = VectorT<_T, RawStratAllocationManagerT<_T>>;

using BinVectorT = RawVectorT<uint8_t>;

template<typename _T, typename... _Ts>
using WalkerT = Walker<_T, _Ts...>;

template<typename _T>
using RawWalkerT = Walker<_T, RawStratAllocationManagerT<_T>>;

using BinWalkerT = RawWalkerT<uint8_t>;

// Next 3 typedefs are temporary solutions before embedded added

template<typename _K, typename _V, typename... _Ts>
using MapT = std::map<_K, _V, _Ts...>;

template<typename _K, typename _V, typename... _Ts>
using HashMapT = std::unordered_map<_K, _V, _Ts...>;

template<typename _K, typename _V, typename... _Ts>
using HashMultiMapT = std::unordered_multimap<_K, _V, _Ts...>;

template<typename _T, typename... Ts>
using ListT = std::list<_T, Ts...>;

} // namespace common_serialization

#endif // #ifndef CS_CUSTOM_CONTAINERS_TYPEDEFS
