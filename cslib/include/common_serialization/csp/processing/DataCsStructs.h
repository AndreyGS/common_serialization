/**
 * @file cslib/include/common_serialization/csp/processing/DataCsStructs.h
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

#include "common_serialization/csp/traits.h"
#include "common_serialization/csp/processing/DataProcessor.h"


#define SERIALIZE_NO_CONVERSION_COMMON(value, ctx)                                      \
{                                                                                       \
    if constexpr (                                                                      \
           SimplyAssignableType<decltype(value)>                                        \
        || SimplyAssignableAlignedToOneType<decltype(value)>)                           \
    {                                                                                   \
        Status status = serializeDataSimpleAssignable((value), (ctx));                  \
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
}

#define DESERIALIZE_NO_CONVERSION_COMMON(ctx, value)                                    \
{                                                                                       \
    if constexpr (                                                                      \
           SimplyAssignableType<decltype(value)>                                        \
        || SimplyAssignableAlignedToOneType<decltype(value)>)                           \
    {                                                                                   \
        Status status = deserializeDataSimpleAssignable((ctx), (value));                \
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
   }                                                                                    \
}

namespace common_serialization::csp::processing
{

template<>
constexpr Status DataProcessor::serializeData(const Id& value, context::SData<>& ctx)
{
    RUN(serializeData(value.leftPart, ctx));
    RUN(serializeData(value.rightPart, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, Id& value)
{
    RUN(deserializeData(ctx, value.leftPart));
    RUN(deserializeData(ctx, value.rightPart));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const context::DataFlags& value, context::SData<>& ctx)
{
    RUN(serializeData(value.leftPart, ctx));
    RUN(serializeData(value.rightPart, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, context::DataFlags& value)
{
    RUN(deserializeData(ctx, value.leftPart));
    RUN(deserializeData(ctx, value.rightPart));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const traits::Interface& value, context::SData<>& ctx)
{
    RUN(serializeData(value.id, ctx));
    RUN(serializeData(value.version, ctx));
    RUN(serializeData(value.mandatoryDataFlags, ctx));
    RUN(serializeData(value.forbiddenDataFlags, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, traits::Interface& value)
{
    RUN(deserializeData(ctx, value.id));
    RUN(deserializeData(ctx, value.version));
    RUN(deserializeData(ctx, value.mandatoryDataFlags));
    RUN(deserializeData(ctx, value.forbiddenDataFlags));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
