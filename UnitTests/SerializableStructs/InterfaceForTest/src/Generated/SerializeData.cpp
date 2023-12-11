/**
 * @file UnitTests/SerializableStructs/interface_for_test/src/Generated/SerializeData.cpp
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

#include "interface_for_test/Generated/SerializeData.h"

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
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableAlignedToOne<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_x, ctx));
    RUN(serializeData(value.m_y, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignable<>& value, context::SData<>& ctx)
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

    RUN(serializeData(value.m_vx, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableDescendant<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(static_cast<const interface_for_test::SimpleAssignable<>&>(value), ctx));

    RUN(serializeData(value.m_d, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::DynamicPolymorphic<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_o, ctx));
    RUN(serializeData(value.m_dpNS, ctx));

    RUN(serializeData(value.m_arrO, ctx));
    RUN(serializeData(value.m_arrDpNS, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::Diamond<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondEdge1&>(value), ctx));
    RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondEdge2&>(value), ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SpecialProcessingType<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_vec, ctx));
    RUN(serializeData(value.m_saaToNS, ctx));
    RUN(serializeData(value.m_saNS, ctx));
    RUN(serializeData(value.m_pVec, ctx));
    RUN(serializeData(value.m_pInt, ctx));
    RUN(serializeData(value.m_ppInt, ctx));
    RUN(serializeData(value.m_nullptrInt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableAlignedToOneSimilarType1<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableAlignedToOneSimilarType2<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableSimilarType1<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableSimilarType2<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimilarType1<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));
    RUN(serializeData(value.m_arrL, ctx));
    RUN(serializeData(value.m_sasTs, ctx));
    RUN(serializeData(value.m_saaToSts, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimilarType2<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_j, ctx));
    RUN(serializeData(value.m_k, ctx));
    RUN(serializeData(value.m_arrL, ctx));
    RUN(serializeData(value.m_sasTs, ctx));
    RUN(serializeData(value.m_saaToSts, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::RecursiveTestSpecial1& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.pAny, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::RecursiveTestSpecial2& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.pI, ctx));
    RUN(serializeData(value.pNext, ctx));
    RUN(serializeData(value.pAny, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::ManyPointersType<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_vec, ctx));
    RUN(serializeData(value.m_vecRecursive, ctx));
    RUN(serializeData(value.m_pVec, ctx));
    RUN(serializeData(value.m_rtSpec1, ctx));
    RUN(serializeData(value.m_rtSpec2, ctx));
    RUN(serializeData(value.m_pInt, ctx));
    RUN(serializeData(value.m_intArr, ctx));
    RUN(serializeData(value.m_ppInt, ctx));
    RUN(serializeData(value.m_nullptrInt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::DForAllModesTests<>& value, context::SData<>& ctx)
{
    SERIALIZE_COMMON(value, ctx);

    RUN(serializeData(value.m_saDs, ctx));
    RUN(serializeData(value.m_diamond, ctx));
    RUN(serializeData(value.m_sptCs, ctx));
    RUN(serializeData(value.m_saaToStS, ctx));
    RUN(serializeData(value.m_saStS, ctx));
    RUN(serializeData(value.m_stS, ctx));
    RUN(serializeData(value.m_mpt, ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef SERIALIZE_COMMON

#undef RUN
