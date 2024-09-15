/**
 * @file common_serialization/allocation_managers/GenericAllocationManager.h
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

#include <common_serialization/allocation_managers/allocation_managers_config.h>
#include <common_serialization/allocation_manager_interfaces/IAllocationManager.h>

namespace common_serialization
{

/// @brief Generic stateless class for allocation helping
/// @tparam _Allocator Class that implement IAllocatorImpl interface
/// @tparam _MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocationManager it must be void.
template<IAllocatorImpl _Allocator, typename _MostDerivedClass = void>
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
    constexpr [[nodiscard]] pointer allocateAndConstructImpl(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
    {
        size_type allocatedN = 0;

        pointer p = this->allocate(requestedN, &allocatedN);

        if (p)
        {
            if (Status status = this->constructN(p, allocatedN, std::forward<Args>(args)...); !statusSuccess(status))
            {
                this->deallocate(p);
                allocatedN = 0;
                p = nullptr;
            }
        }

        if (pAllocatedN)
            *pAllocatedN = allocatedN;

        return p;
    }

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateImpl(size_type requestedN, size_type* pAllocatedN) const
    {
        pointer p = this->getAllocator().allocate(requestedN);

        if (pAllocatedN)
            *pAllocatedN = p ? requestedN : 0;

        return p;
    }

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateStrictImpl(size_type n) const
    {
        return this->getAllocator().allocate(n);
    }

    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, Args&&... args) const
    {
        return this->getAllocator().construct(p++, std::forward<Args>(args)...);
    }

    template<typename... Args>
    constexpr Status constructNImpl(pointer p, size_type n, Args&&... args) const
    {
        if (p)
        {
            pointer pStart = p;
            for (size_type i = 0, lastElement = n - 1; i < n; ++i)
            {
                Status status = Status::NoError;

                if (i == lastElement)
                    status = this->getAllocator().construct(p++, std::forward<Args>(args)...);
                else
                    status = this->getAllocator().construct(p++, args...);

                if (!statusSuccess(status))
                {
                    this->destroyN(pStart, p - pStart - 1);
                    return status;
                }
            }

            return Status::NoError;
        }
        else
            return Status::ErrorInvalidArgument;
    }

    constexpr Status copyToRawNoOverlapImpl(pointer pDest, const_pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if constexpr (constructor_allocator::value)
        {
            pointer pStart = pDest;

            for (size_type i = 0; i < n; ++i)
            {
                Status status = this->getAllocator().construct(pDest++, *pSrc++);

                if (!statusSuccess(status))
                {
                    this->destroyN(pStart, pDest - pStart - 1);
                    return status;
                }
            }
        }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));
        
        return Status::NoError;
    }

    constexpr Status copyToDirtyImpl(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

        if (helpers::areRegionsOverlap(pDest, pSrc, n))
        {
            if constexpr (constructor_allocator::value)
            {
                if (pDest < pSrc)
                    return this->copyToDirtyNoOverlap(pDest, pDirtyMemoryFinish, pSrc, n);
                else
                {
                    size_type lastElementOffset = n - 1;

                    for (size_type i = lastElementOffset; i != static_cast<size_type>(-1); --i)
                    {
                        if (&pDest[i] < pDirtyMemoryFinish)
                            this->getAllocator().destroy(&pDest[i]);

                        Status status = this->getAllocator().construct(&pDest[i], pSrc[i]);

                        if (!statusSuccess(status))
                        {
                            this->destroyN(&pDest[i] + 1, &pDest[lastElementOffset] - &pDest[i]);
                            return status;
                        }
                    }
                }
            }
            else
                memmove(pDest, pSrc, n * sizeof(value_type));
        }
        else
            return this->copyToDirtyNoOverlap(pDest, pDirtyMemoryFinish, pSrc, n);

        return Status::NoError;
    }

    constexpr Status copyToDirtyNoOverlapImpl(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if constexpr (constructor_allocator::value)
        {
            pointer pStart = pDest;
            for (size_type i = 0; i < n; ++i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                Status status = this->getAllocator().construct(pDest++, *pSrc++);

                if (!statusSuccess(status))
                {
                    this->destroyN(pStart, pDest - pStart - 1);
                    return status;
                }
            }
        }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));

        return Status::NoError;
    }

    constexpr Status moveToRawNoOverlapImpl(pointer pDest, pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if constexpr (constructor_allocator::value)
            for (size_type i = 0; i < n; ++i)
            {
                Status status = this->getAllocator().construct(pDest++, std::move(*pSrc++));

                if (!statusSuccess(status))
                {
                    for (pointer pDestBegin = pDest - i - 1; pDestBegin != pDest - 1;)
                        this->getAllocator().destroy(pDestBegin++);

                    return status;
                }
            }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));

        return Status::NoError;
    }

    constexpr Status moveToDirtyImpl(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if (pDest == pSrc)
            return Status::NoError;

        if (helpers::areRegionsOverlap(pDest, pSrc, n))
        {
            if constexpr (constructor_allocator::value)
            {
                if (pDest < pSrc)
                    return this->moveToDirtyNoOverlap(pDest, pDirtyMemoryFinish, pSrc, n);
                else
                {
                    size_type lastElementOffset = n - 1;

                    for (size_type i = lastElementOffset; i != static_cast<size_type>(-1); --i)
                    {
                        if (&pDest[i] < pDirtyMemoryFinish)
                            this->getAllocator().destroy(&pDest[i]);

                        Status status = this->getAllocator().construct(&pDest[i], std::move(pSrc[i]));

                        if (!statusSuccess(status))
                        {
                            this->destroyN(&pDest[i] + 1, &pDest[lastElementOffset] - &pDest[i]);
                            return status;
                        }
                    }
                }
            }
            else
                memmove(pDest, pSrc, n * sizeof(value_type));
        }
        else
            return this->moveToDirtyNoOverlap(pDest, pDirtyMemoryFinish, pSrc, n);

        return Status::NoError;
    }

    constexpr Status moveToDirtyNoOverlapImpl(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        assert(!n || pDest && pSrc);

        if constexpr (constructor_allocator::value)
        {
            pointer pStart = pDest;
            for (size_type i = 0; i < n; ++i)
            {
                if (pDest < pDirtyMemoryFinish)
                    this->getAllocator().destroy(pDest);

                Status status = this->getAllocator().construct(pDest++, std::move(*pSrc++));

                if (!statusSuccess(status))
                {
                    this->destroyN(pStart, pDest - pStart - 1);
                    return status;
                }
            }
        }
        else
            memcpy(pDest, pSrc, n * sizeof(value_type));

        return Status::NoError;
    }

    AGS_CS_ALWAYS_INLINE constexpr void destroyAndDeallocateImpl(pointer p, size_type n) const noexcept
    {
        if constexpr (constructor_allocator::value)
            this->destroyN(p, n);
        this->deallocate(p);
    }

    AGS_CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p) const noexcept
    {
        this->getAllocator().deallocate(p);
    }

    AGS_CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept
    {
        if constexpr (constructor_allocator::value)
            if (p)
                this->getAllocator().destroy(p);
    }

    AGS_CS_ALWAYS_INLINE constexpr void destroyNImpl(pointer p, size_type n) const noexcept
    {
        if constexpr (constructor_allocator::value)
            if (p)
                for (size_type i = 0; i < n; ++i)
                    this->getAllocator().destroy(p + i);
    }

    AGS_CS_ALWAYS_INLINE constexpr size_type max_size_impl() const noexcept
    {
        return this->getAllocator().max_size();
    }
};

} // namespace common_serialization
