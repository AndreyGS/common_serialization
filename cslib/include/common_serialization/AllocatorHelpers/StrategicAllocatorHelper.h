/**
 * @file cslib/include/common_serialization/AllocatorHelpers/StrategicAllocatorHelper.h
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

#include "common_serialization/AllocatorHelpers/GenericAllocatorHelper.h"

namespace common_serialization
{

enum class AllocationStrategy
{
    strictByDataSize,
    doubleOfDataSize
};

/// @brief Stateful allocator helper which allocates storage
///     using allocation strategy
/// @tparam _T Type of objects that allocator would allocate and construct
/// @tparam Allocator Class that implement IAllocator interface
/// @tparam MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocatorHelper it must be Dummy.
template<typename _T, IAllocator _Allocator, typename _MostDerivedClass = Dummy>
class StrategicAllocatorHelper : public GenericAllocatorHelper<_T, _Allocator, GetCrtpMainType<StrategicAllocatorHelper<_T, _Allocator>, _MostDerivedClass>>
{
public:
    using value_type = _T;
    using allocator_type = _Allocator;
    using constructor_allocator = typename allocator_type::constructor_allocator;

    /// @brief Real most derived class
    using instance_type = GetCrtpMainType<StrategicAllocatorHelper<value_type, allocator_type>, _MostDerivedClass>;

    /// @brief IAllocatorHelper interface
    using interface_type = typename IAllocatorHelper<value_type, allocator_type, instance_type>::interface_type;
    using size_type = typename interface_type::size_type;
    using difference_type = typename interface_type::difference_type;

    explicit constexpr StrategicAllocatorHelper(AllocationStrategy allocationStrategy = AllocationStrategy::doubleOfDataSize) noexcept;

    /// @brief Get current allocation strategy
    /// @return Current allocation strategy
    [[nodiscard]] constexpr AllocationStrategy getAllocationStrategy() const noexcept;

    /// @brief Set allocation strategy
    /// @param allocationStrategy Allocation strategy
    constexpr void setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept;

protected:
    friend interface_type;

    [[nodiscard]] constexpr value_type* allocateImpl(size_type n, size_type* pAllocatedN) const;

private:
    AllocationStrategy m_allocation_strategy{ AllocationStrategy::doubleOfDataSize };
};

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr StrategicAllocatorHelper<_T, _Allocator, _MostDerivedClass>::StrategicAllocatorHelper(AllocationStrategy allocationStrategy) noexcept
    : GenericAllocatorHelper<value_type, allocator_type, instance_type>(), m_allocation_strategy(allocationStrategy)
{ }


template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
[[nodiscard]] constexpr AllocationStrategy StrategicAllocatorHelper<_T, _Allocator, _MostDerivedClass>::getAllocationStrategy() const noexcept
{
    return m_allocation_strategy;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
constexpr void StrategicAllocatorHelper<_T, _Allocator, _MostDerivedClass>::setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept
{
    m_allocation_strategy = allocationStrategy;
}

template<typename _T, IAllocator _Allocator, typename _MostDerivedClass>
[[nodiscard]] constexpr _T* StrategicAllocatorHelper<_T, _Allocator, _MostDerivedClass>::allocateImpl(size_type requestedN, size_type* pAllocatedN) const
{
    value_type* p = nullptr;

    if (m_allocation_strategy == AllocationStrategy::doubleOfDataSize)
    {
        *pAllocatedN = requestedN << 1;
        p = this->allocateStrict(*pAllocatedN);
    }

    if (!p)
    {
        *pAllocatedN = requestedN;
        p = this->allocateStrict(*pAllocatedN);
    }

    if (!p)
        *pAllocatedN = 0;

    return p;
}

} // namespace common_serialization
