/**
 * @file UnitTests/SerializableStructs/AnotherYetInterface/src/Generated/SerializeData.cpp
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

#include "another_yet_interface/Generated/SerializeData.h"

#define RUN(x)                                                                          \
{                                                                                       \
    if (Status status = (x); !statusSuccess(status))                                    \
        return status;                                                                  \
}

#define SERIALIZE_COMMON(value, ctx)                                                    \
{                                                                                       \
    if (                                                                                \
           IsISerializableBased<decltype(value)>                                        \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = convertToOldStructIfNeed((value), (ctx));                       \
        if (status == Status::kNoFurtherProcessingRequired)                             \
            return Status::kNoError;                                                    \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
    }                                                                                   \
                                                                                        \
    if constexpr (                                                                      \
           SimpleAssignableType<decltype(value)>                                        \
        || SimpleAssignableAlignedToOneType<decltype(value)>)                           \
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

namespace common_serialization::csp::processing
{


template<>
Status DataProcessor::serializeData(const another_yet_interface::SimpleStruct<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_i, ctx));


    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef SERIALIZE_COMMON

#undef RUN
