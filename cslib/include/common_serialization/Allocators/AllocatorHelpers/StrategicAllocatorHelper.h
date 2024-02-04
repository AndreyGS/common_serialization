/**
 * @file cslib/include/common_serialization/Allocators/AllocatorHelpers/StrategicAllocatorHelper.h
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

#include "common_serialization/Allocators/AllocatorHelpers/GenericAllocatorHelper.h"

namespace common_serialization
{

enum class AllocationStrategy
{
    strictByDataSize,
    doubleOfDataSize
};

/// @brief Stateful allocator helper which allocates storage
///     using allocation strategy
/// @tparam T Type of objects that allocator would allocate and construct
/// @tparam Allocator Class that implement IAllocator interface
/// @tparam MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocatorHelper it must be Dummy.
template<typename T, IAllocator Allocator, typename MostDerivedClass = Dummy>
class StrategicAllocatorHelper : public GenericAllocatorHelper<T, Allocator, GetCrtpMainType<StrategicAllocatorHelper<T, Allocator>, MostDerivedClass>>
{
public:
    /// @brief Real most derived class
    using instance_type = GetCrtpMainType<StrategicAllocatorHelper<T, Allocator>, MostDerivedClass>;

    /// @brief IAllocatorHelper interface
    using interface_type = IAllocatorHelper<T, Allocator, instance_type>::interface_type;
    using value_type = typename interface_type::value_type;
    using size_type = typename interface_type::size_type;
    using difference_type = typename interface_type::difference_type;
    using constructor_allocator = typename interface_type::constructor_allocator;

    constexpr StrategicAllocatorHelper(AllocationStrategy allocationStrategy = AllocationStrategy::doubleOfDataSize) noexcept;

    /// @brief Get current allocation strategy
    /// @return Current allocation strategy
    [[nodiscard]] constexpr AllocationStrategy getAllocationStrategy() const noexcept;

    /// @brief Set allocation strategy
    /// @param allocationStrategy Allocation strategy
    constexpr void setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept;

protected:
    friend interface_type;

    [[nodiscard]] constexpr T* allocateImpl(size_type n, size_type* pAllocatedN) const;

private:
    AllocationStrategy m_allocation_strategy;
};

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr StrategicAllocatorHelper<T, Allocator, MostDerivedClass>::StrategicAllocatorHelper(AllocationStrategy allocationStrategy) noexcept
    : GenericAllocatorHelper<T, Allocator, instance_type>(), m_allocation_strategy(allocationStrategy)
{ }


template<typename T, IAllocator Allocator, typename MostDerivedClass>
[[nodiscard]] constexpr AllocationStrategy StrategicAllocatorHelper<T, Allocator, MostDerivedClass>::getAllocationStrategy() const noexcept
{
    return m_allocation_strategy;
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
constexpr void StrategicAllocatorHelper<T, Allocator, MostDerivedClass>::setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept
{
    m_allocation_strategy = allocationStrategy;
}

template<typename T, IAllocator Allocator, typename MostDerivedClass>
[[nodiscard]] constexpr T* StrategicAllocatorHelper<T, Allocator, MostDerivedClass>::allocateImpl(size_type requestedN, size_type* pAllocatedN) const
{
    T* p = nullptr;

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

template<typename T>
using RawStrategicAllocatorHelper = StrategicAllocatorHelper<T, RawNoexceptAllocator<T>>;

} // namespace common_serialization
