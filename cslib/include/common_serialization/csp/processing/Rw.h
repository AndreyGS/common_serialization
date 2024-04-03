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

template<typename T, ISerializationCapableContainer S>
CS_ALWAYS_INLINE constexpr Status writePrimitive(const T& value, context::Common<S>& ctx)
{
    if constexpr (IsEndiannessReversable<T>)
    {
        if (ctx.isEndiannessNotMatch())
            return ctx.getBinaryData().pushBackArithmeticValue(helpers::reverseEndianess(value));
        else
            return ctx.getBinaryData().pushBackArithmeticValue(value);
    }
    else
        return ctx.getBinaryData().pushBackArithmeticValue(value);
}

template<typename T, ISerializationCapableContainer S>
CS_ALWAYS_INLINE constexpr Status writeRawData(const T* p, typename S::size_type n, context::Common<S>& ctx)
{
    const typename S::size_type bytesSize = sizeof(T) * n;
    assert((n == bytesSize / sizeof(T)));

    return ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize);
}

template<typename T, IDeserializationCapableContainer D>
CS_ALWAYS_INLINE constexpr Status readPrimitive(context::Common<D>& ctx, T& value)
{
    if constexpr (IsEndiannessReversable<T>)
    {
        if (ctx.isEndiannessNotMatch())
        {
            CS_RUN(ctx.getBinaryData().readArithmeticValue(const_cast<std::remove_const_t<T>&>(value)));
            value = helpers::reverseEndianess(value);
            return Status::kNoError;
        }
        else
            return ctx.getBinaryData().readArithmeticValue(value);
    }
    else
        return ctx.getBinaryData().readArithmeticValue(value);
}

template<typename T, IDeserializationCapableContainer D>
CS_ALWAYS_INLINE constexpr Status readRawData(context::Common<D>& ctx, typename D::size_type n, T* p)
{
    const typename D::size_type bytesSize = sizeof(T) * n;
    assert((n == bytesSize / sizeof(T)));
    typename D::size_type readSize = 0;

    CS_RUN(ctx.getBinaryData().read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize, &readSize));

    return readSize == bytesSize ? Status::kNoError : Status::kErrorOverflow;
}

}
