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

#ifdef CS_ALWAYS_INLINE
#undef CS_ALWAYS_INLINE
#endif // #ifdef CS_ALWAYS_INLINE
#if defined(_MSC_VER) && !defined(__clang__)
#define CS_ALWAYS_INLINE __forceinline
#else
#define CS_ALWAYS_INLINE __attribute__((always_inline)) inline
#endif // #if defined(_MSC_VER) && !defined(__clang__)

// header for fixed size integers for non-std environments is not ready yet
#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <cstdint>
#endif // USER_MODE

#ifdef CS_RUN
#undef CS_RUN
#endif // CS_RUN
#define CS_RUN(x)                                                                   \
{                                                                                   \
    if (Status status = (x); !statusSuccess(status))                                \
        return status;                                                              \
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
enum class Status : int32_t
{
    // No error
    NoError                                         =        0,
                                                    
    // Info messages                                
    NoFurtherProcessingRequired                     =        1,
                                                    
    // Errors                                       
    ErrorNoMemory                                   =       -1,
    ErrorOverflow                                   =       -2,
    ErrorInvalidArgument                            =       -3,
    ErrorNotSupportedProtocolVersion                =       -4,
    ErrorNotSupportedInterfaceVersion               =       -5,
    ErrorInvalidHash                                =       -6,
    ErrorMismatchOfProtocolVersions                 =       -7,
    ErrorMismatchOfInterfaceVersions                =       -8,
    ErrorMismatchOfStructId                         =       -9,
    ErrorNoSuchHandler                              =      -10,
    ErrorInternal                                   =      -11,
    ErrorNotSupportedSerializationSettingsForStruct =      -12,
    ErrorInvalidType                                =      -13,
    ErrorDataCorrupted                              =      -14,
    ErrorNotCompatibleCommonFlagsSettings           =      -15,
    ErrorNotCompatibleDataFlagsSettings             =      -16,
    ErrorMoreEntires                                =      -17,
    ErrorNotInited                                  =      -18,
    ErrorNoSupportedInterfaces                      =      -19,
    ErrorNotSupportedInterface                      =      -20,
    ErrorTypeSizeIsTooBig                           =      -21,
    ErrorValueOverflow                              =      -22,
    ErrorNotAvailible                               =      -23,
    ErrorAlreadyInited                              =      -24
};

[[nodiscard]] CS_ALWAYS_INLINE constexpr bool statusSuccess(Status status)
{
    return static_cast<int32_t>(status) >= 0;
}

} // namespace common_serialization
