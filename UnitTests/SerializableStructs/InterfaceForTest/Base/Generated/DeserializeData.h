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

#define DESERIALIZE_COMMON(ctx, value)                                                  \
{                                                                                       \
    if (const context::Flags flags = ctx.getFlags(); flags.interfaceVersionsNotMatch)   \
    {                                                                                   \
        Status status = convertFromOldStructIfNeed((ctx), (value));                     \
                                                                                        \
        if (status == Status::kNoFurtherProcessingRequired)                             \
            return Status::kNoError;                                                    \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
    }                                                                                   \
    else if constexpr (serialization_concepts::SimpleAssignableType<std::remove_reference_t<decltype(value)>>    \
        || serialization_concepts::SimpleAssignableAlignedToOneType<std::remove_reference_t<decltype(value)>>)   \
    {                                                                                   \
        Status status = deserializeDataSimpleAssignable((ctx), (value));                \
        if (status == Status::kNoFurtherProcessingRequired)                             \
            return Status::kNoError;                                                    \
        else if (!statusSuccess(status) && status != Status::kErrorNotSupportedSerializationSettingsForStruct)   \
            return status;                                                              \
                                                                                        \
        /* if we get Status::kErrorNotSupportedSerializationSettingsForStruct, */       \
        /* than we should deserialize it field-by-field */                              \
   }                                                                                    \
}

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSerializable<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_x));
    RUN(deserializeData(ctx, value.m_y));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx
    , special_types::SimpleAssignableSerializable<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_i));
    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_et));
    RUN(deserializeData(ctx, value.m_et2));
    RUN(deserializeData(ctx, value.m_saaToS));
    RUN(deserializeData(ctx, value.m_saaToNS));
    RUN(deserializeData(ctx, value.m_saNS));

    RUN(deserializeData(ctx, value.m_arrI32));
    RUN(deserializeData(ctx, value.m_arrEtS));
    RUN(deserializeData(ctx, value.m_arrEtNS));
    RUN(deserializeData(ctx, value.m_arrSaaTos));
    RUN(deserializeData(ctx, value.m_arrSaaToNS));
    RUN(deserializeData(ctx, value.m_arrSaNS));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DynamicPolymorphicNotSerializable& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_r));
    RUN(deserializeData(ctx, value.m_arrR));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DynamicPolymorphicSerializable<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_o));
    RUN(deserializeData(ctx, value.m_dpNS));

    RUN(deserializeData(ctx, value.m_arrO));
    RUN(deserializeData(ctx, value.m_arrDpNS));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DiamondBaseNotSerializable& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_d0));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DiamondEdge1NotSerializable& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeData(ctx, value.m_d1));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DiamondEdge2NotSerializable& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeData(ctx, value.m_d2));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::DiamondSerializable<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<special_types::DiamondEdge1NotSerializable&>(value)));
    RUN(deserializeData(ctx, static_cast<special_types::DiamondEdge2NotSerializable&>(value)));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx, special_types::SpecialProcessingTypeContainSerializable<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_vec));
    RUN(deserializeData(ctx, value.m_saaToNS));
    RUN(deserializeData(ctx, value.m_saNS));
    RUN(deserializeData(ctx, value.m_pVec));
    RUN(deserializeData(ctx, value.m_pInt));
    RUN(deserializeData(ctx, value.m_ppInt));

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN
