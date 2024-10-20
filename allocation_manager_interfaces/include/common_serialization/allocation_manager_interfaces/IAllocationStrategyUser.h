/**
 * @file common_serialization/allocation_manager_interfaces/IAllocationStrategyUser.h
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

#include <common_serialization/common/common.h>

namespace common_serialization
{

enum class AllocationStrategy
{
    StrictByDataSize,
    DoubleOfDataSize
};

/// @brief Interface of user of allocation strategy (CRTP)
/// @tparam User Most derived class (instance type)
template<typename User>
class IAllocationStrategyUser
{
public:
    /// @brief Get current allocation strategy
    /// @return Current allocation strategy
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] AllocationStrategy getAllocationStrategy() const noexcept
    {
        return static_cast<const User*>(this)->getAllocationStrategyImpl();
    }

    /// @brief Set allocation strategy
    /// @param allocationStrategy Allocation strategy
    AGS_CS_ALWAYS_INLINE constexpr void setAllocationStrategy(AllocationStrategy allocationStrategy) noexcept
    {
        static_cast<User*>(this)->setAllocationStrategyImpl(allocationStrategy);
    }
};

template<typename User>
concept IAllocationStrategyUserImpl = std::is_base_of_v<IAllocationStrategyUser<normalize_t<User>>, normalize_t<User>>;

} // namespace common_serialization
