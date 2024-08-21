/**
 * @file common_serialization/csp_base/context/CommonFlags.h
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
class CommonFlags
{
public:
    /// @brief Bitness of system on which serialization is performed
    ///     is 32 bits (default is 64 bits, when not set)
    /// @remark Currently not implemented
    static constexpr uint32_t kBitness32 = 0x1;

    /// @brief Private parts of message has big-endian format
    /// @remark Currently not implemented
    static constexpr uint32_t kBigEndianFormat = 0x2;

    /// @brief kBigEndianFormat value and environment property are different 
    /// @remark Currently not implemented
    static constexpr uint32_t kEndiannessDifference = 0x4;

    static constexpr uint32_t kValidFlagsMask = 0x7;
    static constexpr uint32_t kForbiddenFlagsMask = ~kValidFlagsMask;
    static constexpr uint32_t kNoFlagsMask = 0x0;

    constexpr CommonFlags() noexcept;
    explicit constexpr CommonFlags(uint32_t value) noexcept;
    constexpr CommonFlags operator=(uint32_t value) noexcept;

    constexpr CommonFlags addFlags(uint32_t value) noexcept;
    constexpr CommonFlags removeFlags(uint32_t value) noexcept;

    constexpr [[nodiscard]] bool bitness32() const noexcept;
    constexpr [[nodiscard]] bool bigEndianFormat() const noexcept;
    constexpr [[nodiscard]] bool endiannessDifference() const noexcept;

    constexpr [[nodiscard]] CommonFlags operator|(CommonFlags rhs) const noexcept;
    constexpr [[nodiscard]] CommonFlags operator&(CommonFlags rhs) const noexcept;
    constexpr [[nodiscard]] CommonFlags operator|(uint32_t rhs) const noexcept;
    constexpr [[nodiscard]] CommonFlags operator&(uint32_t rhs) const noexcept;

    constexpr [[nodiscard]] bool operator<(const CommonFlags& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator==(const CommonFlags& rhs) const noexcept;

    explicit constexpr [[nodiscard]] operator uint32_t() const noexcept;
    explicit constexpr [[nodiscard]] operator bool() const noexcept;

private:
    uint32_t m_flags{ 0 };
};

constexpr CommonFlags::CommonFlags() noexcept
{
    m_flags = (helpers::isBitness32() ? kBitness32 : 0) | (helpers::isBigEndianPlatform() ? kBigEndianFormat : 0);
}

constexpr CommonFlags::CommonFlags(uint32_t value) noexcept
{
    operator=(value);
}

constexpr CommonFlags CommonFlags::operator=(uint32_t value) noexcept
{
    m_flags = value & kValidFlagsMask;

    return *this;
}

constexpr CommonFlags CommonFlags::addFlags(uint32_t value) noexcept
{
    m_flags |= value & kValidFlagsMask;
    return *this;
}

constexpr CommonFlags CommonFlags::removeFlags(uint32_t value) noexcept
{
    m_flags &= ~value;
    return *this;
}

constexpr bool CommonFlags::bitness32() const noexcept
{
    return static_cast<bool>(m_flags & kBitness32);
}

constexpr bool CommonFlags::bigEndianFormat() const noexcept
{
    return static_cast<bool>(m_flags & kBigEndianFormat);
}

constexpr bool CommonFlags::endiannessDifference() const noexcept
{
    return static_cast<bool>(m_flags & kEndiannessDifference);
}

constexpr CommonFlags CommonFlags::operator|(CommonFlags rhs) const noexcept
{
    return static_cast<CommonFlags>(m_flags | rhs.m_flags);
}

constexpr CommonFlags CommonFlags::operator&(CommonFlags rhs) const noexcept
{
    return static_cast<CommonFlags>(m_flags & rhs.m_flags);
}

constexpr CommonFlags CommonFlags::operator|(uint32_t rhs) const noexcept
{
    return *this | static_cast<CommonFlags>(rhs);
}

constexpr CommonFlags CommonFlags::operator&(uint32_t rhs) const noexcept
{
    return *this & static_cast<CommonFlags>(rhs);
}

constexpr bool CommonFlags::operator<(const CommonFlags& rhs) const noexcept
{
    return m_flags < rhs.m_flags;
}

constexpr bool CommonFlags::operator==(const CommonFlags& rhs) const noexcept
{
    return m_flags == rhs.m_flags;
}

constexpr CommonFlags::operator uint32_t() const noexcept
{
    return m_flags;
}

constexpr CommonFlags::operator bool() const noexcept
{
    return m_flags != 0;
}

} // namespace common_serialization::csp::context
