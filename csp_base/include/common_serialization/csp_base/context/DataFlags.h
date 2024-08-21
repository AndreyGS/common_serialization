/**
 * @file common_serialization/csp_base/context/DataFlags.h
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
class DataFlags
{
public:
    /// @details Alignment of structs that take a part in Data Body in 
    ///     modules that serialize and deserialize Message, may be not equal.
    ///     
    /// @note To speed up serialization structs marked with special tags
    ///     and arrays of them would be serialized/deserialized by memcpy
    ///     if their target versions are equal to latest, when this flag not set
    ///     and kSizeOfIntegersMayBeNotEqual also not set.
    static constexpr uint32_t kAlignmentMayBeNotEqual = 0x1;
    
    /// @details Indicates that serialization and deserialization processes
    ///     may be made on modules that are built on different compilers 
    ///     or with different compiler options.
    ///     And if struct contains any of not strictly sized types
    ///     before any write/read of it value there would be a write/read
    ///     of size that this type have.
    /// 
    /// @note  The fact that size is strictly defined or not
    ///     is checked by FixSizedArithmeticType and FixSizedEnumType concepts.
    ///     Notice that most frequent used types are not strictly sized,
    ///     including uin32_t, uin64_t and others.
    ///         This flag has no influence on size_t processing (if it happens
    ///     through serializeSizeT or deserializeSizeT BodyProcessor functions).
    ///     
    /// @remark To avoid unnecessary overhead you should design your interfaces
    ///     with integers that are strictly sized. But if you must do this,
    ///     always set kSizeOfIntegersMayBeNotEqual flag.
    static constexpr uint32_t kSizeOfIntegersMayBeNotEqual = 0x2;
    
    /// @details Allow serialization of pointers without help of BodyProcessor
    ///     serialize/deserialize specialized class functions.
    ///     
    /// @note May be used for containers that holds pointers on objects
    ///     without special handling functions.
    ///     
    /// @remark In deserialization process every creation of new pointer
    ///     is registers in context stored container, which will allow
    ///     to use and safe delete this pointers in future.
    static constexpr uint32_t kAllowUnmanagedPointers = 0x4;

    /// @brief Allows processed pointers be recursively linked.
    ///     Works only in conjunction with allowUnmanagedPointers flag
    ///     which will be set automatically if need.
    static constexpr uint32_t kCheckRecursivePointers = 0x8;

    /// @brief Turns of all optimizations of "simply assignable" tags.
    ///     Must be used when dealing with CSP implementations that does
    ///     not support such. 
    static constexpr uint32_t kSimplyAssignableTagsOptimizationsAreTurnedOff = 0x10;

    static constexpr uint32_t kValidFlagsMask = 0x1f;
    static constexpr uint32_t kForbiddenFlagsMask = ~kValidFlagsMask;
    static constexpr uint32_t kNoFlagsMask = 0x0;

    constexpr DataFlags() = default;
    explicit constexpr DataFlags(uint32_t value) noexcept;
    constexpr DataFlags& operator=(uint32_t value) noexcept;

    constexpr void addFlags(uint32_t value) noexcept;
    constexpr void removeFlags(uint32_t value) noexcept;

    constexpr [[nodiscard]] bool alignmentMayBeNotEqual() const noexcept;
    constexpr [[nodiscard]] bool sizeOfIntegersMayBeNotEqual() const noexcept;
    constexpr [[nodiscard]] bool allowUnmanagedPointers() const noexcept;
    constexpr [[nodiscard]] bool checkRecursivePointers() const noexcept;
    constexpr [[nodiscard]] bool simplyAssignableTagsOptimizationsAreTurnedOff() const noexcept;

    constexpr [[nodiscard]] DataFlags operator|(DataFlags rhs) const noexcept;
    constexpr [[nodiscard]] DataFlags operator&(DataFlags rhs) const noexcept;
    constexpr [[nodiscard]] DataFlags operator|(uint32_t rhs) const noexcept;
    constexpr [[nodiscard]] DataFlags operator&(uint32_t rhs) const noexcept;

    constexpr [[nodiscard]] bool operator<(const DataFlags& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator==(const DataFlags& rhs) const noexcept;

    explicit constexpr [[nodiscard]] operator uint32_t() const noexcept;
    explicit constexpr [[nodiscard]] operator bool() const noexcept;

private:
    uint32_t m_flags{ 0 };
};

constexpr DataFlags::DataFlags(uint32_t value) noexcept
{
    operator=(value);
}

constexpr DataFlags& DataFlags::operator=(uint32_t value) noexcept
{
    m_flags = value & kValidFlagsMask;

    return *this;
}

constexpr void DataFlags::addFlags(uint32_t value) noexcept
{
    m_flags |= value & kValidFlagsMask;
}

constexpr void DataFlags::removeFlags(uint32_t value) noexcept
{
    m_flags &= ~value;
}

constexpr bool DataFlags::alignmentMayBeNotEqual() const noexcept
{
    return static_cast<bool>(m_flags & kAlignmentMayBeNotEqual);
}

constexpr bool DataFlags::sizeOfIntegersMayBeNotEqual() const noexcept
{
    return static_cast<bool>(m_flags & kSizeOfIntegersMayBeNotEqual);
}

constexpr bool DataFlags::allowUnmanagedPointers() const noexcept
{
    return static_cast<bool>(m_flags & kAllowUnmanagedPointers);
}

constexpr bool DataFlags::checkRecursivePointers() const noexcept
{
    return static_cast<bool>(m_flags & kCheckRecursivePointers);
}

constexpr bool DataFlags::simplyAssignableTagsOptimizationsAreTurnedOff() const noexcept
{
    return static_cast<bool>(m_flags & kSimplyAssignableTagsOptimizationsAreTurnedOff);
}

constexpr DataFlags DataFlags::operator|(DataFlags rhs) const noexcept
{
    return static_cast<DataFlags>(m_flags | rhs.m_flags);
}

constexpr DataFlags DataFlags::operator&(DataFlags rhs) const noexcept
{
    return static_cast<DataFlags>(m_flags & rhs.m_flags);
}

constexpr DataFlags DataFlags::operator|(uint32_t rhs) const noexcept
{
    return *this | static_cast<DataFlags>(rhs);
}

constexpr DataFlags DataFlags::operator&(uint32_t rhs) const noexcept
{
    return *this & static_cast<DataFlags>(rhs);
}

constexpr bool DataFlags::operator<(const DataFlags& rhs) const noexcept
{
    return m_flags < rhs.m_flags;
}

constexpr bool DataFlags::operator==(const DataFlags& rhs) const noexcept
{
    return m_flags == rhs.m_flags;
}

constexpr DataFlags::operator uint32_t() const noexcept
{
    return m_flags;
}

constexpr DataFlags::operator bool() const noexcept
{
    return m_flags != 0;
}

} // namespace common_serialization::csp::context
