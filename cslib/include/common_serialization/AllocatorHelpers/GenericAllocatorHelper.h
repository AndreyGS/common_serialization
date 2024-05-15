/**
 * @file cslib/include/common_serialization/AllocatorHelpers/GenericAllocatorHelper.h
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

#include "common_serialization/Helpers.h"
#include "common_serialization/AllocatorHelpers/IAllocatorHelper.h"

namespace common_serialization
{

/// @brief Generic stateless class for allocation helping
/// @tparam _T Type of objects that allocator would allocate and construct
/// @tparam _Allocator Class that implement IAllocator interface
/// @tparam _MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocatorHelper it must be Dummy.
template<typename _T, IAllocator _Allocator, typename _MostDerivedClass = Dummy>
class GenericAllocatorHelper : public IAllocatorHelper<_T, _Allocator, GetCrtpMainType<GenericAllocatorHelper<_T, _Allocator>, _MostDerivedClass>>
{
public:
    using value_type = _T;
    using allocator_type = _Allocator;
    using constructor_allocator = typename allocator_type::constructor_allocator;

    /// @brief Real most derived class
    using instance_type = GetCrtpMainType<GenericAllocatorHelper<value_type, allocator_type>, _MostDerivedClass>;

    /// @brief IAllocatorHelper interface
    using interface_type = typename IAllocatorHelper<value_type, allocator_type, instance_type>::interface_type;
    using size_type = typename interface_type::size_type;
    using difference_type = typename interface_type::difference_type;

protected:
    friend interface_type;

    template<typename... Args>
    [[nodiscard]] constexpr value_type* allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const;
    [[nodiscard]] constexpr value_type* allocateImpl(size_type n, size_type* pAllocatedN) const;
    [[nodiscard]] constexpr value_type* allocateStrictImpl(size_type n) const;

    template<typename... Args>
    constexpr Status constructImpl(value_type* p, Args&&... args) const;
    template<typename... Args>
    constexpr Status constructNImpl(value_type* p, value_type** pNError, size_type n, Args&&... args) const;

    constexpr Status copyDirtyImpl(value_type* pDest, value_type* pDirtyMemoryFinish, const value_type* pSrc, size_type n) const;
    constexpr Status copyDirtyNoOverlapImpl(value_type* pDest, value_type* pDirtyMemoryFinish, const value_type* pSrc, size_type n) const;

    constexpr Status moveImpl(value_type* pDest, value_type* pDirtyMemoryFinish, value_type* pSrc, size_type n) const;
    constexpr Status moveNoOverlapImpl(value_type* pDest, value_type* pDirtyMemoryFinish, value_type* pSrc, size_type n) const;

    constexpr void destroyAndDeallocateImpl(value_type* p, size_type n) const noexcept;
    constexpr void deallocateImpl(value_type* p) const noexcept;
    constexpr void destroyImpl(value_type* p) const noexcept;
    constexpr void destroyNImpl(value_type* p, size_type n) const noexcept;

    constexpr size_type max_size_impl() const noexcept;
};

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
template<typename... Args>
[[nodiscard]] constexpr _T* GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
{
    size_type allocatedN = 0;

    value_type* p = this->allocate(requestedN, &allocatedN);

    if (p)
    {
        value_type* pNError = nullptr;
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

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
[[nodiscard]] constexpr _T* GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::allocateImpl(size_type requestedN, size_type* pAllocatedN) const
{
    value_type* p = this->getAllocator().allocate(requestedN);

    if (pAllocatedN)
        *pAllocatedN = p ? requestedN : 0;

    return p;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
[[nodiscard]] constexpr _T* GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::allocateStrictImpl(size_type n) const
{
    return this->getAllocator().allocate(n);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
template<typename... Args>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::constructImpl(value_type* p, Args&&... args) const
{
    value_type* pNError = nullptr;
    return this->constructNImpl(p, &pNError, 1, std::forward<Args>(args)...);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
template<typename... Args>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::constructNImpl(value_type* p, value_type** nError, size_type n, Args&&... args) const
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

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::copyDirtyImpl(value_type* pDest, value_type* pDirtyMemoryFinish, const value_type* pSrc, size_type n) const
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

                CS_RUN(this->getAllocator().construct(pDest--, *pSrc--));
            }
        }
        else
            memmove(pDest, pSrc, n * sizeof(_T));
    }
    else
        return copyDirtyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

    return Status::kNoError;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::copyDirtyNoOverlapImpl(value_type* pDest, value_type* pDirtyMemoryFinish, const value_type* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if constexpr (constructor_allocator::value)
        for (size_type i = 0; i < n; ++i)
        {
            if (pDest < pDirtyMemoryFinish)
                this->getAllocator().destroy(pDest);

            CS_RUN(this->getAllocator().construct(pDest++, *pSrc++));
        }
    else
        memcpy(pDest, pSrc, n * sizeof(_T));

    return Status::kNoError;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::moveImpl(value_type* pDest, value_type* pDirtyMemoryFinish, value_type* pSrc, size_type n) const
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

                CS_RUN(this->getAllocator().construct(pDest++, std::move(*pSrc)));
                (pSrc++)->~_T(); // as a precaution if _T is not moveable
            }
        }
        else
            memmove(pDest, pSrc, n * sizeof(_T));
    }
    else
        return moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

    return Status::kNoError;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr Status GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::moveNoOverlapImpl(value_type* pDest, value_type* pDirtyMemoryFinish, value_type* pSrc, size_type n) const
{
    assert(!n || pDest && pSrc);

    if (pDest == pSrc)
        return Status::kNoError;

    if constexpr (constructor_allocator::value)
        for (size_type i = 0; i < n; ++i)
        {
            if (pDest < pDirtyMemoryFinish)
                this->getAllocator().destroy(pDest);

            CS_RUN(this->getAllocator().construct(pDest++, std::move(*pSrc++)));
        }
    else
        memcpy(pDest, pSrc, n * sizeof(value_type));

    return Status::kNoError;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr void GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::destroyAndDeallocateImpl(value_type* p, size_type n) const noexcept
{
    if constexpr (constructor_allocator::value)
        this->destroyN(p, n);
    this->deallocate(p);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr void GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::deallocateImpl(value_type* p) const noexcept
{
    this->getAllocator().deallocate(p);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr void GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::destroyImpl(value_type* p) const noexcept
{
    if constexpr (constructor_allocator::value)
        if (p)
            this->getAllocator().destroy(p);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr void GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::destroyNImpl(value_type* p, size_type n) const noexcept
{   
    if constexpr (constructor_allocator::value)
        if (p)
            for (size_type i = 0; i < n; ++i)
                this->getAllocator().destroy(p + i);
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr typename GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::size_type GenericAllocatorHelper<_T, _Allocator, _MostDerivedClass>::max_size_impl() const noexcept
{
    return this->getAllocator().max_size();
}

} // namespace common_serialization
