/**
 * @file cslib/include/common_serialization/csp_base/traits.h
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

#include <common_serialization/csp_base/traits.h>
#include <common_serialization/csp_base/context/DataFlags.h>

namespace common_serialization::csp
{

struct Interface
{
    using simply_assignable_fixed_size_tag = std::true_type;

    Id id{ kNullUuid };

    /// @brief The only field that allowed to change since interface publication
    interface_version_t version{ traits::kInterfaceVersionUndefined };

    context::DataFlags mandatoryDataFlags;
    context::DataFlags forbiddenDataFlags;

    constexpr [[nodiscard]] bool operator<(const Interface& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator==(const Interface& rhs) const noexcept;
};

constexpr bool Interface::operator<(const Interface& rhs) const noexcept
{
    return id < rhs.id && version < rhs.version && mandatoryDataFlags < rhs.mandatoryDataFlags && forbiddenDataFlags < rhs.forbiddenDataFlags;
}

constexpr bool Interface::operator==(const Interface& rhs) const noexcept
{
    return id < rhs.id == version < rhs.version == mandatoryDataFlags < rhs.mandatoryDataFlags == forbiddenDataFlags < rhs.forbiddenDataFlags;
}

} // namespace common_serialization::csp

namespace common_serialization::csp::traits
{

constexpr Interface kUndefinedInterface;

} // namespace common_serialization::csp
