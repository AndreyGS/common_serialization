/**
 * @file SerializationFlags.h
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

struct SerializationFlags
{
    uint32_t alignmentMayBeNotEqual                 : 1 = 0;
    uint32_t sizeOfArithmeticTypesMayBeNotEqual     : 1 = 0;    // this flag is very dangerous and it should never be used,
                                                                // except you are really know what you are doing
    uint32_t interfaceVersionsNotMatch              : 1 = 0;
    uint32_t checkOfCyclicReferences                : 1 = 0;
    uint32_t reserved                               :20 = 0;
    uint32_t doNotUse                               : 8 = 0;    // this bit-field shall have only 24 significant bits
                                                                // 8 bits are using for serialization protocol version

    [[nodiscard]] constexpr explicit operator uint32_t() const noexcept
    {
        return (*static_cast<const uint32_t*>(static_cast<const void*>(this)) & 0xffffff);
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept
    {
        return static_cast<uint32_t>(*this) != 0;
    }
};

}