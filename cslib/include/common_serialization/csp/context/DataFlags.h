/**
 * @file cslib/include/common_serialization/csp/context/DataFlags.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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
struct DataFlags
{
    /// @details Indicates that serialization and deserialization processes
    ///     may be made on modules that are built on different compilers 
    ///     or with different compiler options.
    ///     And caution on fields alignment must be made.
    ///     
    ///         To speed up serialization structs marked with simple_assignable_tag 
    ///     and arrays of them would be serialized/deserialized by memcpy
    ///     if their target versions are equal to latest, when this flag not set
    ///     and sizeOfArithmeticTypesMayBeNotEqual also not set.
    uint32_t alignmentMayBeNotEqual                 : 1 = 0;  
    
    /// @details Indicates that serialization and deserialization processes
    ///     may be made on modules that are built on different compilers 
    ///     or with different compiler options.
    ///     And if struct contains any of not strictly sized types
    ///     before any write/read of it value there would be a write/read
    ///     of size that this type have.
    /// 
    ///         The fact that size is strictly defined or not
    ///     is checks by FixSizedArithmeticType and FixSizedEnumType concepts.
    ///     Notice that most frequent used types are not strictly sized,
    ///     including uin32_t, uin64_t and others.
    ///     
    ///         This flag is very dangerous and it should never be used,
    ///     except you are really know what you are doing.
    ///     Instead in your interface structs you should using types that have
    ///     "semanticaly fixed" size (uin32_t, uin64_t and others).
    uint32_t sizeOfArithmeticTypesMayBeNotEqual     : 1 = 0;
    
    /// @details Allow serialization of pointers without help of DataProcessor
    ///     serializeData/deserializeData specialized class functions.
    ///     
    ///         For instance, may be used for containers that holds pointers on objects
    ///     without special handling functions.
    ///     
    ///         In deserialization process every creation of new pointer
    ///     is registers in context stored container, which will allow
    ///     to use and safe delete this pointers in future.
    uint32_t allowUnmanagedPointers                 : 1 = 0;

    /// @brief Checks if processed pointers are not recursively linked.
    ///     Works only in conjunction with allowUnmanagedPointers flag
    ///     which will be set automatically if need.
    uint32_t checkRecursivePointers                 : 1 = 0;

    uint32_t reserved                               :28 = 0;

    /// @brief Default constructor
    constexpr DataFlags() noexcept;

    /// @brief Constructor from uint32_t
    /// @param value 32 bit unsigned integer
    explicit constexpr DataFlags(uint32_t value) noexcept;

    /// @brief Operator= from uint32_t
    /// @param value 32 bit unsigned integer
    /// @return *this
    constexpr DataFlags& operator=(uint32_t value) noexcept;

    /// @brief Operator==
    /// @param rhs Another instance
    /// @return Result of comparison
    [[nodiscard]] constexpr bool operator==(DataFlags rhs) const noexcept;

    /// @brief Cast to uint32_t
    [[nodiscard]] constexpr explicit operator uint32_t() const noexcept;

    /// @brief Cast to bool
    [[nodiscard]] constexpr explicit operator bool() const noexcept;
};

constexpr DataFlags::DataFlags() noexcept 
{
    *this = 0;
}

constexpr DataFlags::DataFlags(uint32_t value) noexcept
{
    operator=(value);
}

constexpr DataFlags& DataFlags::operator=(uint32_t value) noexcept
{
    return *static_cast<DataFlags*>
        (static_cast<void*>(
            &(*static_cast<uint32_t*>(
                static_cast<void*>(this)) = value)));
}

[[nodiscard]] constexpr bool DataFlags::operator==(DataFlags rhs) const noexcept
{
    uint32_t thisValue = static_cast<uint32_t>(*this);
    uint32_t rhsValue = static_cast<uint32_t>(rhs);
    return thisValue == rhsValue;
}

[[nodiscard]] constexpr DataFlags::operator uint32_t() const noexcept
{
    return (*static_cast<const uint32_t*>(static_cast<const void*>(this)) & 0xffffff);
}

[[nodiscard]] constexpr DataFlags::operator bool() const noexcept
{
    return static_cast<uint32_t>(*this) != 0;
}

} // namespace common_serialization::csp::context
