/**
 * @file UnitTests/SerializableStructs/NotPartOfInterfaces/src/Generated/SerializeData.cpp
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

#include "not_part_of_interfaces/Generated/SerializeData.h"

#define RUN(x)                                                                          \
{                                                                                       \
    if (Status status = (x); !statusSuccess(status))                                    \
        return status;                                                                  \
}

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

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::SimpleAssignableAlignedToOne& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.a, ctx));
    RUN(serializeData(value.s, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::SimpleAssignable& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.q, ctx));
    RUN(serializeData(value.w, ctx));

    return Status::kNoError;
}


template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::DynamicPolymorphic& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_r, ctx));
    RUN(serializeData(value.m_arrR, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::DiamondBase& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_d0, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::DiamondEdge1& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondBase&>(value), ctx));
    RUN(serializeData(value.m_d1, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::DiamondEdge2& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondBase&>(value), ctx));
    RUN(serializeData(value.m_d2, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const not_part_of_interfaces::TwoInts& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.x, ctx));
    RUN(serializeData(value.y, ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef SERIALIZE_NO_CONVERSION_COMMON

#undef RUN
