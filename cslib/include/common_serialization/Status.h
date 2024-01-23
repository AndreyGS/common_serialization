/**
 * @file cslib/include/common_serialization/Status.h
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

#ifdef RUN
#undef RUN
#endif // RUN
#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

#ifdef SET_NEW_ERROR
#undef SET_NEW_ERROR
#endif // SET_NEW_ERROR
#define SET_NEW_ERROR(x)                                                            \
{                                                                                   \
    if (Status newStatus = (x); !statusSuccess(newStatus) && statusSuccess(status)) \
        status = newStatus;                                                         \
}

namespace common_serialization
{

/// @brief Status messages that are using in all procedures 
///     of common_serialization namespace and are part of CSP
/// @note Negative numbers are errors, others are success
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
    kErrorMismatchOfStructId                        =       -9,
    kErrorNoSuchHandler                             =      -10,
    kErrorInternal                                  =      -11,
    kErrorNotSupportedSerializationSettingsForStruct=      -12,
    kErrorInvalidType                               =      -13,
    kErrorDataCorrupted                             =      -14,
    kErrorNotCompatibleFlagsSettings                =      -15,
    kErrorMoreEntires                               =      -16,
    kErrorNotSupportedInOutInterfaceVersion         =      -17,
    kErrorNotInited                                 =      -18
};

constexpr bool statusSuccess(Status status)
{
    return static_cast<int_fast32_t>(status) >= 0;
}

} // namespace common_serialization
