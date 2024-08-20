/**
 * @file common_serialization/csp_base/processing/rw.h
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

#include <common_serialization/common/helpers.h>
#include <common_serialization/csp_base/context/Common.h>

namespace common_serialization::csp::processing
{

template<typename _T>
CS_ALWAYS_INLINE constexpr Status writePrimitive(_T value, context::SCommon& ctx)
{
    if constexpr (EndiannessReversable<_T>)
        if (ctx.endiannessNotMatch())
        {
            if constexpr (std::is_same_v<std::remove_cv_t<_T>, long double>)
                return Status::ErrorNotSupportedSerializationSettingsForStruct;

            value = helpers::reverseEndianess(value);
        }

    return ctx.getBinaryData().pushBackArithmeticValue(value);
}

template<typename _T>
CS_ALWAYS_INLINE constexpr Status writeRawData(const _T* p, csp_size_t n, context::SCommon& ctx)
{
    assert(p && n > 0 || n == 0);
    const csp_size_t bytesSize = sizeof(_T) * n;
    assert(n == bytesSize / sizeof(_T));

    return ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize);
}

template<typename _T>
CS_ALWAYS_INLINE constexpr Status readPrimitive(context::DCommon& ctx, _T& value)
{
    csp_size_t sizeRead{ 0 };
    Status status = Status::NoError;

    if constexpr (std::is_same_v<std::remove_cv_t<_T>, long double>)
        if (ctx.endiannessNotMatch())
            return Status::ErrorNotSupportedSerializationSettingsForStruct;

    CS_RUN(ctx.getBinaryData().readArithmeticValue(const_cast<std::remove_const_t<_T>&>(value)));

    if constexpr (EndiannessReversable<_T>)
        if (ctx.endiannessNotMatch())
            (const_cast<std::remove_const_t<_T>&>(value)) = helpers::reverseEndianess(value);
    
    return Status::NoError;
}

template<typename _T>
CS_ALWAYS_INLINE constexpr Status readRawData(context::DCommon& ctx, csp_size_t n, _T* p)
{
    assert(p && n > 0 || n == 0);
    const csp_size_t bytesSize = sizeof(_T) * n;
    assert(n == bytesSize / sizeof(_T));
    csp_size_t readSize = 0;

    CS_RUN(ctx.getBinaryData().read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize, &readSize));

    return readSize == bytesSize ? Status::NoError : Status::ErrorOverflow;
}

} // namespace common_serialization::csp::processing
