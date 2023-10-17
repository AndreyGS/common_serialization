/**
 * @file DeserializeData.h
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

#include "../SpecialTypesSerializable.h"

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

#define DESERIALIZE_FIELD(ctx, field)                                                                                           \
{                                                                                                                               \
    if (Status status = deserializeData((ctx), *const_cast<std::remove_cv_t<decltype(&field)>>(&field)); !statusSuccess(status))\
        return status;                                                                                                          \
}

#define CONVERT_FROM_OLD_IF_NEED(ctx, value)                                    \
{                                                                               \
    Status status = convertFromOldStructIfNeed((ctx), (value));                 \
                                                                                \
    if (status == Status::kNoFurtherProcessingRequired)                         \
        return Status::kNoError;                                                \
    else if (!statusSuccess(status))                                            \
        return status;                                                          \
}

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSerializable<>& value)
{
    CONVERT_FROM_OLD_IF_NEED(ctx, value);

    DESERIALIZE_FIELD(ctx, value.m_x);
    DESERIALIZE_FIELD(ctx, value.m_y);

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DynamicPolymorphicNotSerializable& value)
{
    RUN(DataProcessor::deserializeData(ctx, value.m_r));
    RUN(DataProcessor::deserializeData(ctx, value.m_arrR));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DynamicPolymorphicSerializable<>& value)
{
    RUN(DataProcessor::deserializeData(ctx, value.m_o));
    RUN(DataProcessor::deserializeData(ctx, value.m_dpNS));

    RUN(DataProcessor::deserializeData(ctx, value.m_arrO));
    RUN(DataProcessor::deserializeData(ctx, value.m_arrDpNS));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DiamondBaseNotSerializable& value)
{
    RUN(DataProcessor::deserializeData(ctx, value.m_d0));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DiamondEdge1NotSerializable& value)
{
    RUN(DataProcessor::deserializeData(ctx, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(DataProcessor::deserializeData(ctx, value.m_d1));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DiamondEdge2NotSerializable& value)
{
    RUN(DataProcessor::deserializeData(ctx, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(DataProcessor::deserializeData(ctx, value.m_d2));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::DiamondSerializable<>& value)
{
    RUN(DataProcessor::deserializeData(ctx, static_cast<special_types::DiamondEdge1NotSerializable&>(value)));
    RUN(DataProcessor::deserializeData(ctx, static_cast<special_types::DiamondEdge2NotSerializable&>(value)));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, const void*>>& ctx, special_types::SpecialProcessingTypeContainSerializable<>& value)
{
    RUN(DataProcessor::deserializeData(ctx, value.m_vec));

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN
