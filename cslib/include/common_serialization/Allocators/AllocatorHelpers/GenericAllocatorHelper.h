/**
 * @file cslib/include/common_serialization/Allocators/AllocatorHelpers/GenericAllocatorHelper.h
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

#include "common_serialization/Allocators/AllocatorHelpers/IAllocatorHelper.h"
#include "common_serialization/Helpers.h"

namespace common_serialization
{

/// @brief Generic stateless class for allocation helping
/// @tparam T Type of objects that allocator would allocate and construct
/// @tparam Allocator Class that implement IAllocator interface
/// @tparam MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocatorHelper it must be Dummy.
template<typename T, IAllocator Allocator, typename MostDerivedClass = Dummy>
class GenericAllocatorHelper : public IAllocatorHelper<T, Allocator, GetCrtpMainType<GenericAllocatorHelper<T, Allocator>, MostDerivedClass>>
{
public:
    /// @brief Real most derived class
    using instance_type = GetCrtpMainType<GenericAllocatorHelper<T, Allocator>, MostDerivedClass>;

    /// @brief IAllocatorHelper interface
    using interface_type = IAllocatorHelper<T, Allocator, instance_type>::interface_type;
    using value_type = typename interface_type::value_type;
    using size_type = typename interface_type::size_type;
    using difference_type = typename interface_type::difference_type;
    using constructor_allocator = typename interface_type::constructor_allocator;

    constexpr GenericAllocatorHelper() : IAllocatorHelper<T, Allocator, instance_type>() { }

protected:
    friend interface_type;

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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
template<typename... Args>
[[nodiscard]] constexpr T* GenericAllocatorHelper<T, Allocator, MostDerivedClass>::allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
{
    size_type allocatedN = 0;

    T* p = this->allocate(requestedN, &allocatedN);

    if (p)
    {
        T* pNError = nullptr;
        if (Status status = this->constructN(p, &pNError, allocatedN, std::forward<Args>(args)...); !statusSuccess(status))
        {
            this->destroyAndDeallocateImpl(p, pNError - p);
            allocatedN = 0;
            p = nullptr;
        }
    }

    if (pAllocatedN)
        *pAllocatedN = allocatedN;

    return p;
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
[[nodiscard]] constexpr T* GenericAllocatorHelper<T, Allocator, MostDerivedClass>::allocateImpl(size_type requestedN, size_type* pAllocatedN) const
{
    T* p = this->getAllocator().allocate(requestedN);

    if (pAllocatedN)
        *pAllocatedN = p ? requestedN : 0;

    return p;
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
[[nodiscard]] constexpr T* GenericAllocatorHelper<T, Allocator, MostDerivedClass>::allocateStrictImpl(size_type n) const
{
    return this->getAllocator().allocate(n);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
template<typename... Args>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::constructImpl(T* p, Args&&... args) const
{
    T* pNError = nullptr;
    return this->constructNImpl(p, &pNError, 1, std::forward<Args>(args)...);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
template<typename... Args>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::constructNImpl(T* p, T** nError, size_type n, Args&&... args) const
{
    if (p)
        for (size_type i = 0, lastElement = n - 1; i < n; ++i)
        {
            Status status = Status::kNoError;

            if (i == lastElement)
                status = this->getAllocator().construct(p++, std::forward<Args>(args)...);
            else
                status = this->getAllocator().construct(p++, args...);

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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::copyDirtyImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::copyDirtyNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::moveImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr Status GenericAllocatorHelper<T, Allocator, MostDerivedClass>::moveNoOverlapImpl(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
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

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr void GenericAllocatorHelper<T, Allocator, MostDerivedClass>::destroyAndDeallocateImpl(T* p, size_type n) const noexcept
{
    if constexpr (constructor_allocator::value)
        this->destroyN(p, n);
    this->deallocate(p);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr void GenericAllocatorHelper<T, Allocator, MostDerivedClass>::deallocateImpl(T* p) const noexcept
{
    this->getAllocator().deallocate(p);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr void GenericAllocatorHelper<T, Allocator, MostDerivedClass>::destroyImpl(T* p) const noexcept
{
    if constexpr (constructor_allocator::value)
        if (p)
            this->getAllocator().destroy(p);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr void GenericAllocatorHelper<T, Allocator, MostDerivedClass>::destroyNImpl(T* p, size_type n) const noexcept
{   
    if constexpr (constructor_allocator::value)
        if (p)
            for (size_type i = 0; i < n; ++i)
                this->getAllocator().destroy(p + i);
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr GenericAllocatorHelper<T, Allocator, MostDerivedClass>::size_type GenericAllocatorHelper<T, Allocator, MostDerivedClass>::max_size_impl() const noexcept
{
    return this->getAllocator().max_size();
}

template<typename T>
using RawGenericAllocatorHelper = GenericAllocatorHelper<T, RawNoexceptAllocator<T>>;

template<typename T>
using RawKeeperGenericAllocatorHelper = GenericAllocatorHelper<T, RawKeeperAllocator<T>>;

template<typename T>
using ConstructorGenericAllocatorHelper = GenericAllocatorHelper<T, ConstructorNoexceptAllocator<T>>;

} // namespace common_serialization
