/**
 * @file AllocatorConcepts.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

template<typename T>
concept BasicAllocator = requires(T a)
{
    typename T::size_type;
    typename T::difference_type;
    T();
    { a.allocate(0) } -> std::same_as<void*>;
    { a.deallocate(nullptr) } -> std::same_as<void>;
    { a.max_size() } -> std::same_as<size_t>;
};

template<typename T>
concept ConstructorAllocator = BasicAllocator<T> && requires(T a)
{
    typename T::value_type;
    typename T::pointer;
    { a.construct(nullptr, 1) } -> std::same_as<void>;
    { a.destroy(nullptr) } -> std::same_as<void>;
};

template<typename T>
concept RawAllocator = BasicAllocator<T> && !ConstructorAllocator<T>;

} // namespace common_serialization
