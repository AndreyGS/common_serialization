/**
 * @file ContextDataFlags.h
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

struct DataFlags
{
    uint32_t alignmentMayBeNotEqual                 : 1 = 0;    // to speed up serialization without this flag
                                                                // structs marked with simple_assignable_tag and their vectors
                                                                // would be serialized and deserialized by memcpy
    uint32_t sizeOfArithmeticTypesMayBeNotEqual     : 1 = 0;    // this flag is very dangerous and it should never be used,
                                                                // except you are really know what you are doing
    uint32_t allowUnmanagedPointers                 : 1 = 0;    // Allow serialization of pointers without help of DataProcessor
                                                                // serializeData/deserializeData specialized class functions.
                                                                // For instance, may be used for containers that holds pointers on objects
                                                                // without special handling functions.
                                                                // In deserialization process every creation of new pointer
                                                                // is registers in context stored container, which will allow
                                                                // to safe delete this pointers in future.
                                                                // Note that when context is destroyed all accumulated pointers are destroyed too
                                                                // so if you need them, you shall clear container that contains them before context destruction.
                                                                // Also note, that if container on which points context field is destroyed before context itself
                                                                // you will get dangling pointers, so be careful with that.
    uint32_t checkRecursivePointers                 : 1 = 0;    // works only in conjunction with allowUnmanagedPointers which sets automatically
    uint32_t reserved                               :28 = 0;

    constexpr DataFlags() noexcept;
    explicit constexpr DataFlags(uint32_t value) noexcept;

    constexpr DataFlags& operator=(uint32_t value) noexcept;

    [[nodiscard]] constexpr explicit operator uint32_t() const noexcept;
    [[nodiscard]] constexpr explicit operator bool() const noexcept;
};

constexpr DataFlags::DataFlags() noexcept {}

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

[[nodiscard]] constexpr DataFlags::operator uint32_t() const noexcept
{
    return (*static_cast<const uint32_t*>(static_cast<const void*>(this)) & 0xffffff);
}

[[nodiscard]] constexpr DataFlags::operator bool() const noexcept
{
    return static_cast<uint32_t>(*this) != 0;
}

} // namespace context

} // namespace csp

} // namespace common_serialization
