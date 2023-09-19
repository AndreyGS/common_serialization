/**
 * @file SerializationFlags.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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
    unsigned alignmentMayBeNotEqual                 : 1 = 0;
    unsigned sizeOfArithmeticTypesMayBeNotEqual     : 1 = 0;
    unsigned interfaceVersionsNotMatch              : 1 = 0;
    unsigned checkOfCyclicReferences                : 1 = 0;
    unsigned reserved                               :20 = 0;
    unsigned doNotUse                               : 8 = 0;    // this bit-field shall have only 24 significant bits
                                                                // 8 bits are using for serialization protocol version

    [[nodiscard]] constexpr operator bool() const noexcept
    {
        return *static_cast<const unsigned*>(static_cast<const void*>(this)) != 0;
    }
};

}