/**
 * @file InterfaceAllocatorHelper.h
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

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
class InterfaceAllocatorHelper
{
protected:
    InterfaceAllocatorHelper() { }

public:
    using value_type = T;
    using allocator = Allocator;

    template<typename... Args>
    [[nodiscard]] constexpr inline T* allocateAndConstruct(size_t requestedN, size_t* allocatedN, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateAndConstructImpl(requestedN, allocatedN, std::forward<Args>(args)...);
    }

    [[nodiscard]] constexpr inline T* allocate(size_t requestedN, size_t* allocatedN) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateImpl(requestedN, allocatedN);
    }

    [[nodiscard]] constexpr inline T* allocateStrict(size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateStrictImpl(n);
    }

    template<typename... Args>
    constexpr inline void construct(T* p, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructImpl(p, std::forward<Args>(args)...);
    }

    template<typename... Args>
    constexpr inline void construct_n(T* p, size_t n, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructNImpl(p, n, std::forward<Args>(args)...);
    }

    constexpr inline void copy(T* dest, const T* src, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyImpl(dest, src, n);
    }

    constexpr inline void copyNoOverlap(T* dest, const T* src, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyNoOverlapImpl(dest, src, n);
    }

    constexpr inline void move(T* dest, T* src, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveImpl(dest, src, n);
    }

    constexpr inline void moveNoOverlap(T* dest, T* src, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveNoOverlapImpl(dest, src, n);
    }

    constexpr inline void destroyAndDeallocate(T* p, size_t n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyAndDeallocateImpl(p, n);
    }

    constexpr inline void deallocate(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->deallocateImpl(p);
    }

    constexpr inline void destroy(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyImpl(p);
    }

    constexpr inline void destroyN(T* p, size_t n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyNImpl(p, n);
    }

    constexpr inline size_t max_size() const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->max_size_impl();
    }
};

} // namespace common_serialization
