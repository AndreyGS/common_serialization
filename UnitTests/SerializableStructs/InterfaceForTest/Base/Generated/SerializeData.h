/**
 * @file SerializeData.h
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

#define SERIALIZE_COMMON(value, ctx)                                                    \
{                                                                                       \
    if (                                                                                \
           serialization_concepts::IsISerializableBased<decltype(value)>                \
        && ctx.getFlags().interfaceVersionsNotMatch                                     \
    )                                                                                   \
    {                                                                                   \
        Status status = convertToOldStructIfNeed((value), (ctx));                       \
        if (status == Status::kNoFurtherProcessingRequired)                             \
            return Status::kNoError;                                                    \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
    }                                                                                   \
    else if constexpr (serialization_concepts::SimpleAssignableType<decltype(value)>    \
        || serialization_concepts::SimpleAssignableAlignedToOneType<decltype(value)>)   \
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

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneNotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.a, ctx));
    RUN(serializeData(value.s, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSerializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_x, ctx));
    RUN(serializeData(value.m_y, ctx));

    return Status::kNoError;
}


template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableNotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.q, ctx));
    RUN(serializeData(value.w, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableSerializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_i, ctx));
    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_et, ctx));
    RUN(serializeData(value.m_et2, ctx));
    RUN(serializeData(value.m_saaToS, ctx));
    RUN(serializeData(value.m_saaToNS, ctx));
    RUN(serializeData(value.m_saNS, ctx));
    
    RUN(serializeData(value.m_arrI32, ctx));
    RUN(serializeData(value.m_arrEtS, ctx));
    RUN(serializeData(value.m_arrEtNS, ctx));
    RUN(serializeData(value.m_arrSaaTos, ctx));
    RUN(serializeData(value.m_arrSaaToNS, ctx));
    RUN(serializeData(value.m_arrSaNS, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DynamicPolymorphicNotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_r, ctx));
    RUN(serializeData(value.m_arrR, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DynamicPolymorphicSerializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_o, ctx));
    RUN(serializeData(value.m_dpNS, ctx));

    RUN(serializeData(value.m_arrO, ctx));
    RUN(serializeData(value.m_arrDpNS, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondBaseNotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_d0, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondEdge1NotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(static_cast<const special_types::DiamondBaseNotSerializable&>(value), ctx));
    RUN(serializeData(value.m_d1, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondEdge2NotSerializable& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(static_cast<const special_types::DiamondBaseNotSerializable&>(value), ctx));
    RUN(serializeData(value.m_d2, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondSerializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(static_cast<const special_types::DiamondEdge1NotSerializable&>(value), ctx));
    RUN(serializeData(static_cast<const special_types::DiamondEdge2NotSerializable&>(value), ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SpecialProcessingTypeContainSerializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_vec, ctx));
    RUN(serializeData(value.m_saaToNS, ctx));
    RUN(serializeData(value.m_saNS, ctx));
    RUN(serializeData(value.m_pVec, ctx));
    RUN(serializeData(value.m_pInt, ctx));
    RUN(serializeData(value.m_ppInt, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSimilarType1Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSimilarType2Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableSimilarType1Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableSimilarType2Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimilarType1Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimilarType2Serializable<>& value
    , context::SData<Vector<uint8_t>, std::unordered_map<const void*, uint64_t>>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef SERIALIZE_COMMON

#undef RUN
