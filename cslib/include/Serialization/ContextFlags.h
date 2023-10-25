/**
 * @file ContextFlags.h
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

namespace common_serialization
{

namespace csp
{
    
namespace context
{

struct Flags
{
    uint32_t protocolVersionsNotMatch               : 1 = 0;
    uint32_t interfaceVersionsNotMatch              : 1 = 0;    // when this flag is set, it's also indicates that
                                                                // serialized data would include version for every serialized struct
                                                                // to allow it properly deserialized
    uint32_t alignmentMayBeNotEqual                 : 1 = 0;    // to speed up serialization without this flag
                                                                // structs marked with simple_assignable_tag and their vectors
                                                                // would be serialized and deserialized by memcpy
    uint32_t sizeOfArithmeticTypesMayBeNotEqual     : 1 = 0;    // this flag is very dangerous and it should never be used,
                                                                // except you are really know what you are doing
    uint32_t extendedPointersProcessing             : 1 = 0;    // currently makes only recursion resolution for pointers
    uint32_t reserved                               :27 = 0;

    constexpr Flags() noexcept;
    explicit constexpr Flags(uint32_t value) noexcept;

    constexpr Flags& operator=(uint32_t value) noexcept;

    [[nodiscard]] constexpr explicit operator uint32_t() const noexcept;
    [[nodiscard]] constexpr explicit operator bool() const noexcept;
};

constexpr Flags::Flags() noexcept {}

constexpr Flags::Flags(uint32_t value) noexcept
{
    operator=(value);
}

constexpr Flags& Flags::operator=(uint32_t value) noexcept
{
    return *static_cast<Flags*>
        (static_cast<void*>(
            &(*static_cast<uint32_t*>(
                static_cast<void*>(this)) = value)));
}

[[nodiscard]] constexpr Flags::operator uint32_t() const noexcept
{
    return (*static_cast<const uint32_t*>(static_cast<const void*>(this)) & 0xffffff);
}

[[nodiscard]] constexpr Flags::operator bool() const noexcept
{
    return static_cast<uint32_t>(*this) != 0;
}

} // namespace context

} // namespace csp

} // namespace common_serialization
