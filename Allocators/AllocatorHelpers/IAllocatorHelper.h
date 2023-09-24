/**
 * @file IAllocatorHelper.h
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

namespace common_serialization
{

template<typename T, IAllocator Allocator, typename AllocatorHelper>
class IAllocatorHelper
{
public:
    using value_type = typename Allocator::value_type;
    using pointer = typename Allocator::pointer;
    using size_type = typename Allocator::size_type;
    using difference_type = typename Allocator::difference_type;
    using constructor_allocator = typename Allocator::constructor_allocator;
    using allocator = Allocator;

    template<typename... Args>
    [[nodiscard]] constexpr T* allocateAndConstruct(size_t requestedN, size_t* allocatedN, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateAndConstructImpl(requestedN, allocatedN, std::forward<Args>(args)...);
    }

    [[nodiscard]] constexpr T* allocate(size_t requestedN, size_t* allocatedN) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateImpl(requestedN, allocatedN);
    }

    [[nodiscard]] constexpr T* allocateStrict(size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateStrictImpl(n);
    }

    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructImpl(p, std::forward<Args>(args)...);
    }

    template<typename... Args>
    constexpr Status constructN(T* p, T* pNError, size_t n, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructNImpl(p, pNError, n, std::forward<Args>(args)...);
    }

    // copy using copy constructor
    constexpr Status copy(T* pDest, const T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyImpl(pDest, pDest, pSrc, n);
    }

    // copy using copy constructor when dest and src not overlapping
    constexpr Status copyNoOverlap(T* pDest, const T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    constexpr Status copyDirty(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    // copy using copy constructor when dest and src not overlapping
    constexpr Status copyDirtyNoOverlap(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    constexpr Status move(T* pDest, T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveImpl(pDest, pDest, pSrc, n);
    }

    constexpr Status moveNoOverlap(T* pDest, T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    constexpr Status moveDirty(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    constexpr Status moveDirtyNoOverlap(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_t n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    constexpr void destroyAndDeallocate(T* p, size_t n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyAndDeallocateImpl(p, n);
    }

    constexpr void deallocate(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->deallocateImpl(p);
    }

    constexpr void destroy(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyImpl(p);
    }

    constexpr void destroyN(T* p, size_t n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyNImpl(p, n);
    }

    constexpr size_t max_size() const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->max_size_impl();
    }

    constexpr Allocator& getAllocator() noexcept
    {
        return m_allocator;
    }

    constexpr const Allocator& getAllocator() const noexcept
    {
        return m_allocator;
    }

protected:
    constexpr IAllocatorHelper() { }

private:
    Allocator m_allocator;
};

} // namespace common_serialization
