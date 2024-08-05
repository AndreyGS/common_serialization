/**
 * @file cslib/include/common_serialization/AllocatorInterfaces/IAllocatorTraits.h
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

#include <common_serialization/Common/common.h>

namespace common_serialization
{

template<typename _T, typename _ConstructorAllocator>
struct IAllocatorTraits
{
    static_assert(std::is_trivially_copyable_v<_T> || _ConstructorAllocator::value);

    using value_type = _T;
    using pointer = value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = _ConstructorAllocator;
};

template<typename _T>
concept IAllocatorTraitsImpl = std::is_base_of_v<IAllocatorTraits<typename _T::value_type, typename _T::constructor_allocator>, normalize_t<_T>>;

template<typename _T>
using RawAllocatorTraits = IAllocatorTraits<_T, std::false_type>;

template<typename _T>
using ConstructorAllocatorTraits = IAllocatorTraits<_T, std::true_type>;

} // namespace common_serialization
