/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/SerializeData.cpp
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

#include "interface_for_test/processing/Generated/SerializeData.h"

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableAlignedToOne<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_x, ctx));
    CS_RUN(serializeData(value.m_y, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignable<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_i, ctx));
    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_et, ctx));
    CS_RUN(serializeData(value.m_et2, ctx));
    CS_RUN(serializeData(value.m_saaToS, ctx));
    CS_RUN(serializeData(value.m_saaToNS, ctx));
    CS_RUN(serializeData(value.m_saNS, ctx));
    
    CS_RUN(serializeData(value.m_arrI32, ctx));
    CS_RUN(serializeData(value.m_arrEtS, ctx));
    CS_RUN(serializeData(value.m_arrEtNS, ctx));
    CS_RUN(serializeData(value.m_arrSaaTos, ctx));
    CS_RUN(serializeData(value.m_arrSaaToNS, ctx));
    CS_RUN(serializeData(value.m_arrSaNS, ctx));

    CS_RUN(serializeData(value.m_vx, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableDescendant<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(static_cast<const interface_for_test::SimplyAssignable<>&>(value), ctx));

    CS_RUN(serializeData(value.m_d, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::AlwaysSimplyAssignable<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_x, ctx));
    CS_RUN(serializeData(value.m_y, ctx));

    return Status::kNoError;
}


template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableFixedSize<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_x, ctx));
    CS_RUN(serializeData(value.m_asa, ctx));
    CS_RUN(serializeData(value.m_arrAsa, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::DynamicPolymorphic<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_o, ctx));
    CS_RUN(serializeData(value.m_dpNS, ctx));

    CS_RUN(serializeData(value.m_arrO, ctx));
    CS_RUN(serializeData(value.m_arrDpNS, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::Diamond<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondEdge1&>(value), ctx));
    CS_RUN(serializeData(static_cast<const not_part_of_interfaces::DiamondEdge2&>(value), ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SpecialProcessingType<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_vec, ctx));
    CS_RUN(serializeData(value.m_saaToNS, ctx));
    CS_RUN(serializeData(value.m_saNS, ctx));
    CS_RUN(serializeData(value.m_pVec, ctx));
    CS_RUN(serializeData(value.m_pInt, ctx));
    CS_RUN(serializeData(value.m_ppInt, ctx));
    CS_RUN(serializeData(value.m_nullptrInt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableAlignedToOneSimilarType1<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableAlignedToOneSimilarType2<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableSimilarType1<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimplyAssignableSimilarType2<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimilarType1<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));
    CS_RUN(serializeData(value.m_arrL, ctx));
    CS_RUN(serializeData(value.m_sasTs, ctx));
    CS_RUN(serializeData(value.m_saaToSts, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimilarType2<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_j, ctx));
    CS_RUN(serializeData(value.m_k, ctx));
    CS_RUN(serializeData(value.m_arrL, ctx));
    CS_RUN(serializeData(value.m_sasTs, ctx));
    CS_RUN(serializeData(value.m_saaToSts, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::ManyPointersType<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_vec, ctx));
    CS_RUN(serializeData(value.m_vecRecursive, ctx));
    CS_RUN(serializeData(value.m_pVec, ctx));
    CS_RUN(serializeData(value.m_rtSpec1, ctx));
    CS_RUN(serializeData(value.m_rtSpec2, ctx));
    CS_RUN(serializeData(value.m_pInt, ctx));
    CS_RUN(serializeData(value.m_intArr, ctx));
    CS_RUN(serializeData(value.m_ppInt, ctx));
    CS_RUN(serializeData(value.m_nullptrInt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::DForAllModesTests<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_saDs, ctx));
    CS_RUN(serializeData(value.m_diamond, ctx));
    CS_RUN(serializeData(value.m_sptCs, ctx));
    CS_RUN(serializeData(value.m_saaToStS, ctx));
    CS_RUN(serializeData(value.m_saStS, ctx));
    CS_RUN(serializeData(value.m_stS, ctx));
    CS_RUN(serializeData(value.m_mpt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::ContainSimplyAssignableWithoutSerializationFunctions<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serializeData(value.m_sawsf, ctx));
    CS_RUN(serializeData(value.m_npfSawsf, ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
