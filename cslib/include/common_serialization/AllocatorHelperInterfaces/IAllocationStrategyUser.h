/**
 * @file cslib/include/common_serialization/AllocatorHelperInterfaces/IAllocationStrategyUser.h
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

#include "common_serialization/AllocatorInterfaces/allocator_interface.h"

namespace common_serialization
{

enum class AllocationStrategy
{
    StrictByDataSize,
    DoubleOfDataSize
};

/// @brief Interface of user of allocation strategy (CRTP)
/// @tparam _User Most derived class (instance type)
template<typename _User>
class IAllocationStrategyUser
{
public:
    /// @brief Get current allocation strategy
    /// @return Current allocation strategy
    [[nodiscard]] CS_ALWAYS_INLINE constexpr AllocationStrategy getAllocationStrategy() const noexcept
    {
        return static_cast<const _User*>(this)->getAllocationStrategyImpl();
    }

    /// @brief Set allocation strategy
    /// @param allocationStrategy Allocation strategy
    CS_ALWAYS_INLINE constexpr void setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept
    {
        static_cast<_User*>(this)->setAllocationStrategyImpl(allocationStrategy);
    }
};

template<typename _User>
concept IAllocationStrategyUserImpl = std::is_base_of_v<IAllocationStrategyUser<normalize_t<_User>>, normalize_t<_User>>;

} // namespace common_serialization
