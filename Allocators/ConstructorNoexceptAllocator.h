/**
 * @file ConstructorNoexceptAllocator.h
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

#include "RawHeapAllocator.h"

namespace common_serialization
{

namespace memory_management
{

template<typename T, typename... Args>
constexpr inline void placement_new(T* p, Args&&... args) noexcept;

template<typename T>
constexpr inline void destroy(T* p) noexcept;

} // namespace memory_management

template<typename T, RawAllocator Allocator>
class ConstructorNoexceptAllocator : public Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = typename Allocator::size_type;
    using difference_type = typename Allocator::difference_type;

    static constexpr size_type max_size_v = Allocator::max_size_v / sizeof(T);

    template<typename... Args>
    constexpr void construct(T* p, Args&&... args) const noexcept;

    constexpr void destroy(T* p) const noexcept;

    constexpr size_type max_size() const noexcept;
};

template<typename T, RawAllocator Allocator>
template<typename... Args>
constexpr void ConstructorNoexceptAllocator<T, Allocator>::construct(T* p, Args&&... args) const noexcept
{
    new ((void*)p) T(std::forward<Args>(args)...);
}

template<typename T, RawAllocator Allocator>
constexpr void ConstructorNoexceptAllocator<T, Allocator>::destroy(T* p) const noexcept
{
    p->~T();
}

template<typename T, RawAllocator Allocator>
constexpr ConstructorNoexceptAllocator<T, Allocator>::size_type ConstructorNoexceptAllocator<T, Allocator>::max_size() const noexcept
{
    return max_size_v;
}

} // namespace common_serialization
