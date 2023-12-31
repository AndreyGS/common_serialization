/**
 * @file UnitTests/SerializableStructs/NotPartOfInterfaces/src/Generated/DeserializeData.cpp
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

#include "not_part_of_interfaces/Generated/DeserializeData.h"

#define RUN(x)                                                                          \
{                                                                                       \
    if (Status status = (x); !statusSuccess(status))                                    \
        return status;                                                                  \
}

#define DESERIALIZE_NO_CONVERSION_COMMON(ctx, value)                                    \
{                                                                                       \
    if constexpr (                                                                      \
           SimpleAssignableType<decltype(value)>                                        \
        || SimpleAssignableAlignedToOneType<decltype(value)>)                           \
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
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::SimpleAssignableAlignedToOne& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.a));
    RUN(deserializeData(ctx, value.s));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::SimpleAssignable& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.q));
    RUN(deserializeData(ctx, value.w));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::DynamicPolymorphic& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_r));
    RUN(deserializeData(ctx, value.m_arrR));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::DiamondBase& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_d0));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::DiamondEdge1& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<not_part_of_interfaces::DiamondBase&>(value)));
    RUN(deserializeData(ctx, value.m_d1));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::DiamondEdge2& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<not_part_of_interfaces::DiamondBase&>(value)));
    RUN(deserializeData(ctx, value.m_d2));

    return Status::kNoError;
}


template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, not_part_of_interfaces::TwoInts& value)
{
    DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.x));
    RUN(deserializeData(ctx, value.y));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef DESERIALIZE_NO_CONVERSION_COMMON

#undef RUN