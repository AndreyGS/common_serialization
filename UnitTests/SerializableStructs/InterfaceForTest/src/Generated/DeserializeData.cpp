/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/Generated/DeserializeData.cpp
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

#include "interface_for_test/Generated/DeserializeData.h"

#define RUN(x)                                                                          \
{                                                                                       \
    if (Status status = (x); !statusSuccess(status))                                    \
        return status;                                                                  \
}

#define DESERIALIZE_COMMON(ctx, value)                                                  \
{                                                                                       \
    if (                                                                                \
           IsISerializableBased<decltype(value)>                                        \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = convertFromOldStructIfNeed((ctx), (value));                     \
                                                                                        \
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
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOne<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_x));
    RUN(deserializeData(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignable<>& value)
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

    RUN(deserializeData(ctx, value.m_vx));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableDescendant<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<interface_for_test::SimpleAssignable<>&>(value)));

    RUN(deserializeData(ctx, value.m_d));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::DynamicPolymorphic<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_o));
    RUN(deserializeData(ctx, value.m_dpNS));

    RUN(deserializeData(ctx, value.m_arrO));
    RUN(deserializeData(ctx, value.m_arrDpNS));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::Diamond<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<not_part_of_interfaces::DiamondEdge1&>(value)));
    RUN(deserializeData(ctx, static_cast<not_part_of_interfaces::DiamondEdge2&>(value)));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SpecialProcessingType<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_vec));
    RUN(deserializeData(ctx, value.m_saaToNS));
    RUN(deserializeData(ctx, value.m_saNS));
    RUN(deserializeData(ctx, value.m_pVec));
    RUN(deserializeData(ctx, value.m_pInt));
    RUN(deserializeData(ctx, value.m_ppInt));
    RUN(deserializeData(ctx, value.m_nullptrInt));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOneSimilarType1<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOneSimilarType2<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableSimilarType1<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableSimilarType2<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimilarType1<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));
    RUN(deserializeData(ctx, value.m_arrL));
    RUN(deserializeData(ctx, value.m_sasTs));
    RUN(deserializeData(ctx, value.m_saaToSts));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimilarType2<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_j));
    RUN(deserializeData(ctx, value.m_k));
    RUN(deserializeData(ctx, value.m_arrL));
    RUN(deserializeData(ctx, value.m_sasTs));
    RUN(deserializeData(ctx, value.m_saaToSts));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::RecursiveTestSpecial1& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.pAny));
   
    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::RecursiveTestSpecial2& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.pI));
    RUN(deserializeData(ctx, value.pNext));
    RUN(deserializeData(ctx, value.pAny));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::ManyPointersType<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_vec));
    RUN(deserializeData(ctx, value.m_vecRecursive));
    RUN(deserializeData(ctx, value.m_pVec));
    RUN(deserializeData(ctx, value.m_rtSpec1));
    RUN(deserializeData(ctx, value.m_rtSpec2));
    RUN(deserializeData(ctx, value.m_pInt));
    RUN(deserializeData(ctx, value.m_intArr));
    RUN(deserializeData(ctx, value.m_ppInt));
    RUN(deserializeData(ctx, value.m_nullptrInt));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::DForAllModesTests<>& value)
{
    DESERIALIZE_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_saDs));
    RUN(deserializeData(ctx, value.m_diamond));
    RUN(deserializeData(ctx, value.m_sptCs));
    RUN(deserializeData(ctx, value.m_saaToStS));
    RUN(deserializeData(ctx, value.m_saStS));
    RUN(deserializeData(ctx, value.m_stS));
    RUN(deserializeData(ctx, value.m_mpt));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef DESERIALIZE_COMMON

#undef RUN
