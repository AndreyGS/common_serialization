/**
 * @file cslib/include/common_serialization/csp/Macros.h
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

#define CSP_SERIALIZE_COMMON(value, ctx)                                                    \
{                                                                                           \
    if (                                                                                    \
           IsISerializableBased<decltype(value)>                                            \
        && ctx.isInterfaceVersionsNotMatch()                                                \
    )                                                                                       \
    {                                                                                       \
        Status status = convertToOldStructIfNeed((value), (ctx));                           \
        if (status == Status::kNoFurtherProcessingRequired)                                 \
            return Status::kNoError;                                                        \
        else if (!statusSuccess(status))                                                    \
            return status;                                                                  \
    }                                                                                       \
                                                                                            \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                     \
    {                                                                                       \
        if (!ctx.simplyAssignableTagsOptimizationsAreTurnedOff())                           \
        {                                                                                   \
            Status status = serializeDataSimplyAssignable((value), (ctx));                  \
            if (status == Status::kNoFurtherProcessingRequired)                             \
                return Status::kNoError;                                                    \
            else if (                                                                       \
                       !statusSuccess(status)                                               \
                    && status != Status::kErrorNotSupportedSerializationSettingsForStruct   \
            )                                                                               \
                return status;                                                              \
                                                                                            \
            /* if we get Status::kErrorNotSupportedSerializationSettingsForStruct, */       \
            /* than we should serialize it field-by-field */                                \
        }                                                                                   \
    }                                                                                       \
}

#define CSP_DESERIALIZE_COMMON(ctx, value)                                                  \
{                                                                                           \
    if (                                                                                    \
           IsISerializableBased<decltype(value)>                                            \
        && ctx.isInterfaceVersionsNotMatch()                                                \
    )                                                                                       \
    {                                                                                       \
        Status status = convertFromOldStructIfNeed((ctx), (value));                         \
                                                                                            \
        if (status == Status::kNoFurtherProcessingRequired)                                 \
            return Status::kNoError;                                                        \
        else if (!statusSuccess(status))                                                    \
            return status;                                                                  \
    }                                                                                       \
                                                                                            \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                     \
    {                                                                                       \
        if (!ctx.simplyAssignableTagsOptimizationsAreTurnedOff())                           \
        {                                                                                   \
            Status status = deserializeDataSimplyAssignable((ctx), (value));                \
            if (status == Status::kNoFurtherProcessingRequired)                             \
                return Status::kNoError;                                                    \
            else if (                                                                       \
                       !statusSuccess(status)                                               \
                    && status != Status::kErrorNotSupportedSerializationSettingsForStruct   \
            )                                                                               \
                return status;                                                              \
                                                                                            \
            /* if we get Status::kErrorNotSupportedSerializationSettingsForStruct, */       \
            /* than we should deserialize it field-by-field */                              \
        }                                                                                   \
   }                                                                                        \
}

#define CSP_SERIALIZE_NO_CONVERSION_COMMON(value, ctx)                                      \
{                                                                                           \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                     \
    {                                                                                       \
        if (!ctx.simplyAssignableTagsOptimizationsAreTurnedOff())                           \
        {                                                                                   \
            Status status = serializeDataSimplyAssignable((value), (ctx));                  \
            if (status == Status::kNoFurtherProcessingRequired)                             \
                return Status::kNoError;                                                    \
            else if (                                                                       \
                        !statusSuccess(status)                                              \
                    && status != Status::kErrorNotSupportedSerializationSettingsForStruct   \
            )                                                                               \
                return status;                                                              \
                                                                                            \
            /* if we get Status::kErrorNotSupportedSerializationSettingsForStruct, */       \
            /* than we should serialize it field-by-field */                                \
        }                                                                                   \
    }                                                                                       \
}

#define CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value)                                    \
{                                                                                           \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                     \
    {                                                                                       \
        if (!ctx.simplyAssignableTagsOptimizationsAreTurnedOff())                           \
        {                                                                                   \
            Status status = deserializeDataSimplyAssignable((ctx), (value));                \
            if (status == Status::kNoFurtherProcessingRequired)                             \
                return Status::kNoError;                                                    \
            else if (                                                                       \
                       !statusSuccess(status)                                               \
                    && status != Status::kErrorNotSupportedSerializationSettingsForStruct   \
            )                                                                               \
                return status;                                                              \
                                                                                            \
            /* if we get Status::kErrorNotSupportedSerializationSettingsForStruct, */       \
            /* than we should deserialize it field-by-field */                              \
        }                                                                                   \
   }                                                                                        \
}
