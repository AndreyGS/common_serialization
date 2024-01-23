/**
 * @file cslib/include/common_serialization/csp/context/CommonFlags.h
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

namespace common_serialization::csp::context
{

/// @brief Flags that are using in Data type of message in CSP
struct CommonFlags
{
    /// @brief Bitness of system on which serialization is performed
    ///     is 32 bits (default is 64 bits, when not set)
    uint16_t bitness32                              : 1 = 0;

    /// @brief Private parts of message has big-endian format
    /// @remark Currently not implemented
    uint16_t bigEndianFormat                        : 1 = 0;

    /// @brief Private parts of message aware of endianess difference
    /// @remark Currently not implemented
    uint16_t possibleEndianDifference               : 1 = 0;

    uint32_t reserved                               :13 = 0;

    constexpr CommonFlags() noexcept { }
    constexpr CommonFlags(bool isBitness32, bool isBigEndianFormat) noexcept : bitness32(isBitness32), bigEndianFormat(isBigEndianFormat) { }
    explicit constexpr CommonFlags(uint16_t value) noexcept;
    constexpr CommonFlags& operator=(uint16_t value) noexcept;
    [[nodiscard]] constexpr CommonFlags operator&(CommonFlags rhs) const noexcept;
    [[nodiscard]] constexpr bool operator==(CommonFlags rhs) const noexcept;
    [[nodiscard]] constexpr explicit operator uint16_t() const noexcept;
    [[nodiscard]] constexpr explicit operator bool() const noexcept;
};

constexpr CommonFlags::CommonFlags(uint16_t value) noexcept
{
    operator=(value);
}

constexpr CommonFlags& CommonFlags::operator=(uint16_t value) noexcept
{
    return *static_cast<CommonFlags*>
        (static_cast<void*>(
            &(*static_cast<uint16_t*>(
                static_cast<void*>(this)) = value)));
}

[[nodiscard]] constexpr CommonFlags CommonFlags::operator&(CommonFlags rhs) const noexcept
{
    return CommonFlags(static_cast<uint16_t>(*this) & static_cast<uint16_t>(rhs));
}

[[nodiscard]] constexpr bool CommonFlags::operator==(CommonFlags rhs) const noexcept
{
    uint16_t thisValue = static_cast<uint16_t>(*this);
    uint16_t rhsValue = static_cast<uint16_t>(rhs);
    return thisValue == rhsValue;
}

[[nodiscard]] constexpr CommonFlags::operator uint16_t() const noexcept
{
    return *static_cast<const uint16_t*>(static_cast<const void*>(this));
}

[[nodiscard]] constexpr CommonFlags::operator bool() const noexcept
{
    return static_cast<uint16_t>(*this) != 0;
}

} // namespace common_serialization::csp::context
