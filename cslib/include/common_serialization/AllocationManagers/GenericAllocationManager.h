/**
 * @file cslib/include/common_serialization/AllocationManagers/GenericAllocationManager.h
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

#include "common_serialization/Common/Helpers.h"
#include "common_serialization/AllocationManagerInterfaces/IAllocationManager.h"

namespace common_serialization
{

/// @brief Generic stateless class for allocation helping
/// @tparam _Allocator Class that implement IAllocatorImpl interface
/// @tparam _MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocationManager it must be Dummy.
template<IAllocatorImpl _Allocator, typename _MostDerivedClass = Dummy>
class GenericAllocationManager : public IAllocationManager<_Allocator, GetCrtpMainType<GenericAllocationManager<_Allocator>, _MostDerivedClass>>
{
public:
    using allocator_type = _Allocator;
    using value_type = typename allocator_type::value_type;
    using pointer = typename allocator_type::pointer;
    using const_pointer = const value_type*;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using constructor_allocator = typename allocator_type::constructor_allocator;

    /// @brief Real most derived class
    using instance_type = GetCrtpMainType<GenericAllocationManager<allocator_type>, _MostDerivedClass>;

    /// @brief IAllocationManager interface
    using allocator_helper_interface_type = IAllocationManager<_Allocator, instance_type>;

protected:
    friend allocator_helper_interface_type;

    template<typename... Args>
    [[nodiscard]] constexpr pointer allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
    {
        size_type allocatedN = 0;

        pointer p = this->allocate(requestedN, &allocatedN);

        if (p)
        {
            pointer pNError = nullptr;
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

    [[nodiscard]] CS_ALWAYS_INLINE constexpr pointer allocateImpl(size_type requestedN, size_type* pAllocatedN) const
    {
        pointer p = this->getAllocator().allocate(requestedN);

        if (pAllocatedN)
            *pAllocatedN = p ? requestedN : 0;

        return p;
    }

    [[nodiscard]] CS_ALWAYS_INLINE constexpr pointer allocateStrictImpl(size_type n) const
    {
        return this->getAllocator().allocate(n);
    }

    template<typename... Args>
    CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, Args&&... args) const
    {
        return this->getAllocator().construct(p++, std::forward<Args>(args)...);
    }

    template<typename... Args>
    constexpr Status constructNImpl(pointer p, pointer* pNError, size_type n, Args&&... args) const
    {
        if (p)
            for (size_type i = 0, lastElement = n - 1; i < n; ++i)
            {
                Status status = Status::NoError;

                if (i == lastElement)
                    status = this->getAllocator().construct(p++, std::forward<Args>(args)...);
                else
                    status = this->getAllocator().construct(p++, args...);

                if (!statusSuccess(status))
                {
                    *pNError = p + i;
                    return status;
                }
            }
        else
            return Status::ErrorInvalidArgument;

        return Status::NoError;
    }

    constexpr Status copyDirtyImpl(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

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
                memmove(pDest, pSrc, n * sizeof(value_type));
        }
        else
            return copyDirtyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

        return Status::NoError;
    }

    constexpr Status copyDirtyNoOverlapImpl(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

        if constexpr (constructor_allocator::value)
            for (size_type i = 0; i < n; ++i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                CS_RUN(this->getAllocator().construct(pDest++, *pSrc++));
            }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));

        return Status::NoError;
    }

    constexpr Status moveImpl(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

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
                    /*
                    if (Status status = this->getAllocator().construct(pDest--, std::move(*pSrc--)); !statusSuccess(status))
                    {
                        ++pDest;
                        for (pointer pDestDone = pDest - i; pDestDone != pDest;)
                            this->getAllocator().destroy(pDestDone++);
                    }*/
                }
            }
            else
                memmove(pDest, pSrc, n * sizeof(value_type));
        }
        else
            return moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);

        return Status::NoError;
    }

    constexpr Status moveNoOverlapImpl(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

        if constexpr (constructor_allocator::value)
            for (size_type i = 0; i < n; ++i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                if (Status status = this->getAllocator().construct(pDest++, std::move(*pSrc++)); !statusSuccess(status))
                {
                    // need to apply logic for overlapping case!
                    --pDest;
                    for (pointer pDestDone = pDest - i; pDestDone != pDest;)
                        this->getAllocator().destroy(pDestDone++);
                
                    while (++pDest < pDirtyMemoryFinish)
                        this->getAllocator().destroy(pDest);

                    for (value_type* pSrcBegin = pSrc - i - 1, *pSrcEnd = pSrc - i + n; pSrcBegin != pSrcEnd;)
                        this->getAllocator().destroy(pSrcBegin++);

                    return status;
                }

            }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));

        return Status::NoError;
    }

    CS_ALWAYS_INLINE constexpr void destroyAndDeallocateImpl(pointer p, size_type n) const noexcept
    {
        if constexpr (constructor_allocator::value)
            this->destroyN(p, n);
        this->deallocate(p);
    }

    CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p) const noexcept
    {
        this->getAllocator().deallocate(p);
    }

    CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept
    {
        if constexpr (constructor_allocator::value)
            if (p)
                this->getAllocator().destroy(p);
    }

    CS_ALWAYS_INLINE constexpr void destroyNImpl(pointer p, size_type n) const noexcept
    {
        if constexpr (constructor_allocator::value)
            if (p)
                for (size_type i = 0; i < n; ++i)
                    this->getAllocator().destroy(p + i);
    }

    CS_ALWAYS_INLINE constexpr size_type max_size_impl() const noexcept
    {
        return this->getAllocator().max_size();
    }
};

} // namespace common_serialization
