/**
 * @file GenericAllocatorHelper.h
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

#include "../Allocators/ConstructorNoexceptAllocator.h"
#include "IAllocatorHelper.h"
#include "../CsHelpers.h"

namespace common_serialization
{

template<typename T, IAllocator Allocator, typename AllocatorHelper>
class GenericAllocatorHelperImpl : public IAllocatorHelper<T, Allocator, AllocatorHelper>
{
protected:
    using size_type = typename IAllocatorHelper<T, Allocator, AllocatorHelper>::size_type;
    using constructor_allocator = typename IAllocatorHelper<T, Allocator, AllocatorHelper>::constructor_allocator;

    friend IAllocatorHelper<T, Allocator, AllocatorHelper>;

    constexpr GenericAllocatorHelperImpl() : IAllocatorHelper<T, Allocator, AllocatorHelper>() { }

    template<typename... Args>
    [[nodiscard]] constexpr T* allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const;
    [[nodiscard]] constexpr T* allocateImpl(size_type n, size_type* pAllocatedN) const;
    [[nodiscard]] constexpr T* allocateStrictImpl(size_type n) const;

    template<typename... Args>
    constexpr Status constructImpl(T* p, Args&&... args) const;
    template<typename... Args>
    constexpr Status constructNImpl(T* p, T** pNError, size_type n, Args&&... args) const;

    constexpr Status copyDirtyImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const;
    constexpr Status copyDirtyNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const;

    constexpr Status moveImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const;
    constexpr Status moveNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const;

    constexpr void destroyAndDeallocateImpl(T* p, size_type n) const noexcept;
    constexpr void deallocateImpl(T* p) const noexcept;
    constexpr void destroyImpl(T* p) const noexcept;
    constexpr void destroyNImpl(T* p, size_type n) const noexcept;

    constexpr size_type max_size_impl() const noexcept;
};

template<typename T, IAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
{
    T* p = this->allocate(requestedN * sizeof(T), pAllocatedN);

    if constexpr (constructor_allocator::value)
        if (p)
        {
            T* pNError = nullptr;
            if (Status status = this->constructN(p, &pNError, requestedN, std::forward<Args>(args)...); !statusSuccess(status))
            {
                this->destroyN(p, pNError - p);
                p = nullptr;
            }
        }

    return p;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateImpl(size_type requestedN, size_type* pAllocatedN) const
{
    T* p = this->getAllocator().allocate(requestedN);

    if (pAllocatedN)
        *pAllocatedN = p ? requestedN : 0;

    return p;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateStrictImpl(size_type n) const
{
    return this->getAllocator().allocate(n);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::constructImpl(T* p, Args&&... args) const
{
    T* pNError = nullptr;
    return this->constructNImpl(p, &pNError, 1, std::forward<Args>(args)...);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::constructNImpl(T* p, T** nError, size_type n, Args&&... args) const
{
    if (p)
        for (size_type i = 0; i < n; ++i)
        {
            Status status = this->getAllocator().construct(p++, std::forward<Args>(args)...);
            if (!statusSuccess(status))
            {
                *nError = p + i;
                return status;
            }
        }
    else
        return Status::kErrorInvalidArgument;

    return Status::kNoError;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyDirtyImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if (helpers::areRegionsOverlap(pDest, pSrc, n) && pDest > pSrc)
    {
        if constexpr (constructor_allocator::value)
        {
            size_type lastElementOffset = n - 1;
            pDest += lastElementOffset;
            pSrc += lastElementOffset;

            for (size_type i = lastElementOffset; i != static_cast<size_type>(-1); --i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                RUN(this->getAllocator().construct(pDest--, *pSrc--));
            }
        }
        else
            memmove(pDest, pSrc, n * sizeof(T));
    }
    else
        return copyDirtyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

    return Status::kNoError;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyDirtyNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if constexpr (constructor_allocator::value)
        for (size_type i = 0; i < n; ++i)
        {
            if (pDest < pDirtyMemoryFinish)
                this->getAllocator().destroy(pDest);

            RUN(this->getAllocator().construct(pDest++, *pSrc++));
        }
    else
        memcpy(pDest, pSrc, n * sizeof(T));

    return Status::kNoError;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::moveImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if (helpers::areRegionsOverlap(pDest, pSrc, n) && pDest > pSrc)
    {
        if constexpr (constructor_allocator::value)
        {
            size_type lastElementOffset = n - 1;
            pDest += lastElementOffset;
            pSrc += lastElementOffset;

            for (size_type i = lastElementOffset; i != static_cast<size_type>(-1); --i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                RUN(this->getAllocator().construct(pDest++, std::move(*pSrc)));
                (pSrc++)->~T(); // as a precaution if T is not moveable
            }
        }
        else
            memmove(pDest, pSrc, n * sizeof(T));
    }
    else
        return moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

    return Status::kNoError;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr Status GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::moveNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if constexpr (constructor_allocator::value)
        for (size_type i = 0; i < n; ++i)
        {
            if (pDest < pDirtyMemoryFinish)
                this->getAllocator().destroy(pDest);

            RUN(this->getAllocator().construct(pDest++, std::move(*pSrc)));
            (pSrc++)->~T(); // as a precaution if T is not moveable
        }
    else
        memcpy(pDest, pSrc, n * sizeof(T));

    return Status::kNoError;
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyAndDeallocateImpl(T* p, size_type n) const noexcept
{
    if constexpr (constructor_allocator::value)
        this->destroyN(p, n);
    this->deallocate(p);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::deallocateImpl(T* p) const noexcept
{
    this->getAllocator().deallocate(p);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyImpl(T* p) const noexcept
{
    if constexpr (constructor_allocator::value)
        if (p)
            this->getAllocator().destroy(p);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyNImpl(T* p, size_type n) const noexcept
{   
    if constexpr (constructor_allocator::value)
        if (p)
            for (size_type i = 0; i < n; ++i)
                this->getAllocator().destroy(p + i);
}

template<typename T, IAllocator Allocator, typename AllocatorHelper>
constexpr IAllocatorHelper<T, Allocator, AllocatorHelper>::size_type GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::max_size_impl() const noexcept
{
    return this->getAllocator().max_size();
}

template<typename T, IAllocator Allocator>
class GenericAllocatorHelper : public GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>
{
public:
    using value_type = typename IAllocatorHelper<T, Allocator, GenericAllocatorHelper<T, Allocator>>::value_type;
    using size_type = typename IAllocatorHelper<T, Allocator, GenericAllocatorHelper<T, Allocator>>::size_type;
    using difference_type = typename IAllocatorHelper<T, Allocator, GenericAllocatorHelper<T, Allocator>>::difference_type;
    using constructor_allocator = typename IAllocatorHelper<T, Allocator, GenericAllocatorHelper<T, Allocator>>::constructor_allocator;

    constexpr GenericAllocatorHelper() noexcept
        : GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>()
    { }
};

} // namespace common_serialization
