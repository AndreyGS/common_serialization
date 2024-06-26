/**
 * @file cslib/include/common_serialization/Allocators/Concepts.h
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

/// @brief Interface concept of allocators using in common_serialization
/// @note common_serialization Allocators in many (or most) scenarios
///     should be used with conjuction of Allocator Helpers
///     as low-level tool
template<typename _T>
concept IAllocator = (std::is_same_v<std::true_type, typename _T::constructor_allocator> || std::is_same_v<std::false_type, typename _T::constructor_allocator>) && requires(_T a)
{
    typename _T::value_type;
    typename _T::pointer;
    typename _T::size_type;
    typename _T::difference_type;
    typename _T::constructor_allocator;
    _T();
    { a.allocate(0) } -> std::same_as<typename _T::pointer>;
    { a.deallocate(nullptr) } -> std::same_as<void>;
    { a.deallocate(nullptr, 1) } -> std::same_as<void>;

    { a.construct(nullptr) } -> std::same_as<Status>;
    { a.destroy(nullptr) } -> std::same_as<void>;

    { a.max_size() } -> std::same_as<typename _T::size_type>;
};

/// @brief Concept of Constructor Allocator
/// @details Allocators in common_serialization have two types:
///     Raw Allocators and Constructor Allocators. Despite the naming
///     both of them can construct objects, but Raw Allocators can only be
///     used with trivially copyable structs and they do not use destructors 
///     on destroying objects. Allocator Helpers that are using Raw Allocators 
///     may use memcpy and memmove procedures in many scenarios instead 
///     of calling ctors and dtors.
template<typename _T>
concept IConstructorAllocator = IAllocator<_T> && std::is_same_v<std::true_type, typename _T::constructor_allocator>;

} // namespace common_serialization
