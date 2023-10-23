/**
 * @file CsStatus.h
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

enum class Status : int_fast32_t
{
    // No error
    kNoError                                        =        0,

    // Info messages
    kNoFurtherProcessingRequired                    =        1,

    // Errors
    kErrorNoMemory                                  =       -1,
    kErrorOverflow                                  =       -2,
    kErrorInvalidArgument                           =       -3,
    kErrorNotSupportedProtocolVersion               =       -4,
    kErrorNotSupportedInterfaceVersion              =       -5,
    kErrorInvalidHash                               =       -6,
    kErrorMismatchOfProtocolVersions                =       -7,
    kErrorMismatchOfInterfaceVersions               =       -8,
    kErrorMismatchOfStructNameHash                  =       -9,
    kErrorNoSuchHandler                             =      -10,
    KErrorInternal                                  =      -11,
    kErrorNotSupportedSerializationSettingsForStruct=      -12,
    kErrorInvalidType                               =      -13
};

constexpr bool statusSuccess(Status status)
{
    return static_cast<int_fast32_t>(status) >= 0;
}

} // namespace common_serialization
