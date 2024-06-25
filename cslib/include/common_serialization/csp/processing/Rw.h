/**
 * @file cslib/include/common_serialization/csp/processing/Rw.h
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

#include "common_serialization/Helpers.h"
#include "common_serialization/csp/context/Common.h"

namespace common_serialization::csp::processing
{

template<typename T>
CS_ALWAYS_INLINE constexpr Status writePrimitive(const T& value, context::SCommon& ctx)
{
    if constexpr (IsEndiannessReversable<T>)
    {
        if (ctx.isEndiannessNotMatch())
        {
            if constexpr (std::is_same_v<std::remove_cv_t<T>, long double>)
                return Status::ErrorNotSupportedSerializationSettingsForStruct;

            const auto reversedValue = helpers::reverseEndianess(value);

            return ctx.getBinaryData().pushBackN(reinterpret_cast<const uint8_t*>(&reversedValue), sizeof(T));
        }
        else
            return ctx.getBinaryData().pushBackN(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
    }
    else
        return ctx.getBinaryData().pushBackN(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
}

template<typename T>
CS_ALWAYS_INLINE constexpr Status writeRawData(const T* p, csp_size_t n, context::SCommon& ctx)
{
    assert(p && n > 0 || n == 0);
    const csp_size_t bytesSize = sizeof(T) * n;
    assert((n == bytesSize / sizeof(T)));

    return ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize);
}

template<typename T>
CS_ALWAYS_INLINE constexpr Status readPrimitive(context::DCommon& ctx, T& value)
{
    csp_size_t sizeRead{ 0 };
    Status status = Status::NoError;

    if constexpr (IsEndiannessReversable<T> && std::is_same_v<std::remove_cv_t<T>, long double>)
        if (ctx.isEndiannessNotMatch())
            return Status::ErrorNotSupportedSerializationSettingsForStruct;

    CS_RUN(ctx.getBinaryData().read(reinterpret_cast<uint8_t*>(&const_cast<std::remove_const_t<T>&>(value)), sizeof(T), &sizeRead));
    if (sizeRead != sizeof(T))
        return Status::ErrorOverflow;

    if constexpr (IsEndiannessReversable<T>)
        if (ctx.isEndiannessNotMatch())
            (const_cast<std::remove_const_t<T>&>(value)) = helpers::reverseEndianess(value);
    
    return Status::NoError;
}

template<typename T>
CS_ALWAYS_INLINE constexpr Status readRawData(context::DCommon& ctx, csp_size_t n, T* p)
{
    assert(p && n > 0 || n == 0);
    const csp_size_t bytesSize = sizeof(T) * n;
    assert((n == bytesSize / sizeof(T)));
    csp_size_t readSize = 0;

    CS_RUN(ctx.getBinaryData().read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize, &readSize));

    return readSize == bytesSize ? Status::NoError : Status::ErrorOverflow;
}

} // namespace common_serialization::csp::processing
