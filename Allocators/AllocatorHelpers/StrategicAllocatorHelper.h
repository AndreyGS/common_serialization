/**
 * @file StrategicAllocatorHelper.h
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

#include "GenericAllocatorHelper.h"

namespace common_serialization
{

enum class AllocationStrategy
{
    strictByDataSize,
    doubleOfDataSize
};

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
class StrategicAllocatorHelperImpl : public GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>
{
public:
    [[nodiscard]] constexpr AllocationStrategy getAllocationStrategy() const noexcept;
    constexpr void setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept;

protected:
    friend InterfaceAllocatorHelper<T, Allocator, AllocatorHelper>;

    constexpr StrategicAllocatorHelperImpl(AllocationStrategy allocationStrategy = AllocationStrategy::doubleOfDataSize) noexcept;

    [[nodiscard]] constexpr T* allocateImpl(size_t n, size_t* allocatedN) const;

private:
    AllocationStrategy m_allocation_strategy;
};

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr StrategicAllocatorHelperImpl<T, Allocator, AllocatorHelper>::StrategicAllocatorHelperImpl(AllocationStrategy allocationStrategy) noexcept
    : GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>(), m_allocation_strategy(allocationStrategy)
{ }


template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr AllocationStrategy StrategicAllocatorHelperImpl<T, Allocator, AllocatorHelper>::getAllocationStrategy() const noexcept
{
    return m_allocation_strategy;
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void StrategicAllocatorHelperImpl<T, Allocator, AllocatorHelper>::setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept
{
    m_allocation_strategy = allocationStrategy;
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr T* StrategicAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateImpl(size_t requestedN, size_t* allocatedN) const
{
    T* p = nullptr;

    if (m_allocation_strategy == AllocationStrategy::doubleOfDataSize)
    {
        *allocatedN = requestedN << 1;
        p = this->allocateStrict(*allocatedN);
    }

    if (!p)
    {
        *allocatedN = requestedN;
        p = this->allocateStrict(*allocatedN);
    }

    if (!p)
        *allocatedN = 0;

    return p;
}

template<typename T, BasicAllocator Allocator = ConstructorNoexceptAllocator<T, RawHeapAllocator>>
class StrategicAllocatorHelper : public StrategicAllocatorHelperImpl<T, Allocator, StrategicAllocatorHelper<T, Allocator>>
{
public:
    constexpr StrategicAllocatorHelper(AllocationStrategy allocationStrategy = AllocationStrategy::doubleOfDataSize) noexcept
        : StrategicAllocatorHelperImpl<T, Allocator, StrategicAllocatorHelper<T, Allocator>>(allocationStrategy)
    { }
};

} // namespace common_serialization
