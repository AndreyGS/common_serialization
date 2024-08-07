/**
 * @file cslib/include/common_serialization/CspBase/Macros.h
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


#define CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx)                                 \
{                                                                                       \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                 \
    {                                                                                   \
        Status status = serializeSimplyAssignable(value, ctx);                          \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (                                                                       \
                    !statusSuccess(status)                                              \
                && status != Status::ErrorNotSupportedSerializationSettingsForStruct    \
        )                                                                               \
            return status;                                                              \
                                                                                        \
        /* if we get Status::ErrorNotSupportedSerializationSettingsForStruct, */        \
        /* than we should serialize it field-by-field */                                \
    }                                                                                   \
}

#define CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)                               \
{                                                                                       \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                 \
    {                                                                                   \
        Status status = deserializeSimplyAssignable(ctx, value);                        \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (                                                                       \
                    !statusSuccess(status)                                              \
                && status != Status::ErrorNotSupportedSerializationSettingsForStruct    \
        )                                                                               \
            return status;                                                              \
                                                                                        \
        /* if we get Status::ErrorNotSupportedSerializationSettingsForStruct, */        \
        /* than we should deserialize it field-by-field */                              \
   }                                                                                    \
}

#define CSP_SERIALIZE_COMMON(value, ctx)                                                \
{                                                                                       \
    if (                                                                                \
           ISerializableImpl<decltype(value)>                                           \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = VersionConverter::toOldStructIfNeed(value, ctx);                \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
        else                                                                            \
            CS_RUN(ContextProcessor::testDataFlagsCompatibility                         \
                    <normalize_t<decltype(value)>>(ctx.getDataFlags()));                \
    }                                                                                   \
                                                                                        \
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx)                                     \
}                                                                                       

#define CSP_DESERIALIZE_COMMON(ctx, value)                                              \
{                                                                                       \
    if (                                                                                \
           ISerializableImpl<decltype(value)>                                           \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = VersionConverter::fromOldStructIfNeed(ctx, value);              \
                                                                                        \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
        else                                                                            \
            CS_RUN(ContextProcessor::testDataFlagsCompatibility                         \
                    <normalize_t<decltype(value)>>(ctx.getDataFlags()));                \
    }                                                                                   \
                                                                                        \
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)                                   \
}

