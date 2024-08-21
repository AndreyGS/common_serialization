/**
 * @file common_serialization/allocation_managers/StrategicAllocationManager.h
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

#include <common_serialization/allocation_managers/GenericAllocationManager.h>
#include <common_serialization/allocation_manager_interfaces/IAllocationStrategyUser.h>

namespace common_serialization
{

/// @brief Stateful allocator helper which allocates storage
///     using allocation strategy
/// @tparam _Allocator Class that implement IAllocatorImpl interface
/// @tparam _MostDerivedClass Instance type. But if type of current instance 
///     is GenericAllocationManager it must be Dummy.
template<IAllocatorImpl _Allocator, typename _MostDerivedClass = Dummy>
class StrategicAllocationManager 
    : public GenericAllocationManager<_Allocator, GetCrtpMainType<StrategicAllocationManager<_Allocator>, _MostDerivedClass>>
    , public IAllocationStrategyUser<GetCrtpMainType<StrategicAllocationManager<_Allocator>, _MostDerivedClass>>
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
    using instance_type = GetCrtpMainType<StrategicAllocationManager<allocator_type>, _MostDerivedClass>;

    /// @brief IAllocationManager interface
    using allocator_helper_interface_type = IAllocationManager<allocator_type, instance_type>;

    using allocation_strategy_user_interface_type = IAllocationStrategyUser<instance_type>;

    explicit constexpr StrategicAllocationManager(AllocationStrategy allocationStrategy = AllocationStrategy::DoubleOfDataSize) noexcept
        : GenericAllocationManager<allocator_type, instance_type>(), m_allocation_strategy(allocationStrategy)
    { }

protected:
    friend allocator_helper_interface_type;
    friend allocation_strategy_user_interface_type;

    constexpr [[nodiscard]] pointer allocateImpl(size_type requestedN, size_type* pAllocatedN) const
    {
        value_type* p = nullptr;

        if (m_allocation_strategy == AllocationStrategy::DoubleOfDataSize)
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

    CS_ALWAYS_INLINE constexpr [[nodiscard]] AllocationStrategy getAllocationStrategyImpl() const noexcept
    {
        return m_allocation_strategy;
    }

    CS_ALWAYS_INLINE constexpr void setAllocationStrategyImpl(AllocationStrategy allocationStrategy) noexcept
    {
        m_allocation_strategy = allocationStrategy;
    }

private:
    AllocationStrategy m_allocation_strategy{ AllocationStrategy::DoubleOfDataSize };
};

} // namespace common_serialization
