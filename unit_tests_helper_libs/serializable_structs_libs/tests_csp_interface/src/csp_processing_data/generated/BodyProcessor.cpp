/**
 * @file common_serialization/tests_csp_interface/src/csp_processing_data/generated/BodyProcessor.cpp
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

#include <common_serialization/csp_base/processing/data/VersionConverter.h>
#include <common_serialization/tests_csp_interface/tests_csp_interface.h>

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOne<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_x, ctx));
    CS_RUN(serialize(value.m_y, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOne<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_y));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignable<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_i, ctx));
    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_et, ctx));
    CS_RUN(serialize(value.m_et2, ctx));
    CS_RUN(serialize(value.m_saaToS, ctx));
    CS_RUN(serialize(value.m_saaToNS, ctx));
    CS_RUN(serialize(value.m_saNS, ctx));
    
    CS_RUN(serialize(value.m_arrI32, ctx));
    CS_RUN(serialize(value.m_arrEtS, ctx));
    CS_RUN(serialize(value.m_arrEtNS, ctx));
    CS_RUN(serialize(value.m_arrSaaTos, ctx));
    CS_RUN(serialize(value.m_arrSaaToNS, ctx));
    CS_RUN(serialize(value.m_arrSaNS, ctx));

    CS_RUN(serialize(value.m_vx, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignable<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_i));
    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_et));
    CS_RUN(deserialize(ctx, value.m_et2));
    CS_RUN(deserialize(ctx, value.m_saaToS));
    CS_RUN(deserialize(ctx, value.m_saaToNS));
    CS_RUN(deserialize(ctx, value.m_saNS));

    CS_RUN(deserialize(ctx, value.m_arrI32));
    CS_RUN(deserialize(ctx, value.m_arrEtS));
    CS_RUN(deserialize(ctx, value.m_arrEtNS));
    CS_RUN(deserialize(ctx, value.m_arrSaaTos));
    CS_RUN(deserialize(ctx, value.m_arrSaaToNS));
    CS_RUN(deserialize(ctx, value.m_arrSaNS));

    CS_RUN(deserialize(ctx, value.m_vx));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableDescendant<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(static_cast<const tests_csp_interface::SimplyAssignable<>&>(value), ctx));

    CS_RUN(serialize(value.m_d, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableDescendant<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<tests_csp_interface::SimplyAssignable<>&>(value)));

    CS_RUN(deserialize(ctx, value.m_d));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::AlwaysSimplyAssignable<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_x, ctx));
    CS_RUN(serialize(value.m_y, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::AlwaysSimplyAssignable<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_y));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableFixedSize<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_x, ctx));
    CS_RUN(serialize(value.m_asa, ctx));
    CS_RUN(serialize(value.m_arrAsa, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableFixedSize<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_asa));
    CS_RUN(deserialize(ctx, value.m_arrAsa));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::DynamicPolymorphic<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_o, ctx));
    CS_RUN(serialize(value.m_dpNS, ctx));

    CS_RUN(serialize(value.m_arrO, ctx));
    CS_RUN(serialize(value.m_arrDpNS, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::DynamicPolymorphic<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_o));
    CS_RUN(deserialize(ctx, value.m_dpNS));

    CS_RUN(deserialize(ctx, value.m_arrO));
    CS_RUN(deserialize(ctx, value.m_arrDpNS));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::Diamond<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(static_cast<const tests_restricted_structs::DiamondEdge1&>(value), ctx));
    CS_RUN(serialize(static_cast<const tests_restricted_structs::DiamondEdge2&>(value), ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::Diamond<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<tests_restricted_structs::DiamondEdge1&>(value)));
    CS_RUN(deserialize(ctx, static_cast<tests_restricted_structs::DiamondEdge2&>(value)));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SpecialProcessingType<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_vec, ctx));
    CS_RUN(serialize(value.m_saaToNS, ctx));
    CS_RUN(serialize(value.m_saNS, ctx));
    CS_RUN(serialize(value.m_pVec, ctx));
    CS_RUN(serialize(value.m_pInt, ctx));
    CS_RUN(serialize(value.m_ppInt, ctx));
    CS_RUN(serialize(value.m_nullptrInt, ctx));

    CS_RUN(serialize(value.m_c, ctx));
    CS_RUN(serialize(value.m_sh, ctx));
    CS_RUN(serialize(value.m_m, ctx));
    CS_RUN(serialize(value.m_tEnum, ctx));
    CS_RUN(serialize(value.m_ll, ctx));
    CS_RUN(serialize(value.m_float, ctx));
    CS_RUN(serialize(value.m_double, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SpecialProcessingType<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_vec));
    CS_RUN(deserialize(ctx, value.m_saaToNS));
    CS_RUN(deserialize(ctx, value.m_saNS));
    CS_RUN(deserialize(ctx, value.m_pVec));
    CS_RUN(deserialize(ctx, value.m_pInt));
    CS_RUN(deserialize(ctx, value.m_ppInt));
    CS_RUN(deserialize(ctx, value.m_nullptrInt));

    CS_RUN(deserialize(ctx, value.m_c));
    CS_RUN(deserialize(ctx, value.m_sh));
    CS_RUN(deserialize(ctx, value.m_m));
    CS_RUN(deserialize(ctx, value.m_tEnum));
    CS_RUN(deserialize(ctx, value.m_ll));
    CS_RUN(deserialize(ctx, value.m_float));
    CS_RUN(deserialize(ctx, value.m_double));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOneSimilarType1<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOneSimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOneSimilarType2<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOneSimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableSimilarType1<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableSimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableSimilarType2<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableSimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimilarType1<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));
    CS_RUN(serialize(value.m_arrL, ctx));
    CS_RUN(serialize(value.m_sasTs, ctx));
    CS_RUN(serialize(value.m_saaToSts, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));
    CS_RUN(deserialize(ctx, value.m_arrL));
    CS_RUN(deserialize(ctx, value.m_sasTs));
    CS_RUN(deserialize(ctx, value.m_saaToSts));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimilarType2<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_j, ctx));
    CS_RUN(serialize(value.m_k, ctx));
    CS_RUN(serialize(value.m_arrL, ctx));
    CS_RUN(serialize(value.m_sasTs, ctx));
    CS_RUN(serialize(value.m_saaToSts, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));
    CS_RUN(deserialize(ctx, value.m_arrL));
    CS_RUN(deserialize(ctx, value.m_sasTs));
    CS_RUN(deserialize(ctx, value.m_saaToSts));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ManyPointersType<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_vec, ctx));
    CS_RUN(serialize(value.m_vecRecursive, ctx));
    CS_RUN(serialize(value.m_pVec, ctx));
    CS_RUN(serialize(value.m_rtSpec1, ctx));
    CS_RUN(serialize(value.m_rtSpec2, ctx));
    CS_RUN(serialize(value.m_pInt, ctx));
    CS_RUN(serialize(value.m_intArr, ctx));
    CS_RUN(serialize(value.m_ppInt, ctx));
    CS_RUN(serialize(value.m_nullptrInt, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ManyPointersType<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_vec));
    CS_RUN(deserialize(ctx, value.m_vecRecursive));
    CS_RUN(deserialize(ctx, value.m_pVec));
    CS_RUN(deserialize(ctx, value.m_rtSpec1));
    CS_RUN(deserialize(ctx, value.m_rtSpec2));
    CS_RUN(deserialize(ctx, value.m_pInt));
    CS_RUN(deserialize(ctx, value.m_intArr));
    CS_RUN(deserialize(ctx, value.m_ppInt));
    CS_RUN(deserialize(ctx, value.m_nullptrInt));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::DForAllModesTests<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_saDs, ctx));
    CS_RUN(serialize(value.m_diamond, ctx));
    CS_RUN(serialize(value.m_sptCs, ctx));
    CS_RUN(serialize(value.m_saaToStS, ctx));
    CS_RUN(serialize(value.m_saStS, ctx));
    CS_RUN(serialize(value.m_stS, ctx));
    CS_RUN(serialize(value.m_mpt, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::DForAllModesTests<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_saDs));
    CS_RUN(deserialize(ctx, value.m_diamond));
    CS_RUN(deserialize(ctx, value.m_sptCs));
    CS_RUN(deserialize(ctx, value.m_saaToStS));
    CS_RUN(deserialize(ctx, value.m_saStS));
    CS_RUN(deserialize(ctx, value.m_stS));
    CS_RUN(deserialize(ctx, value.m_mpt));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainSimplyAssignableWithoutSerializationFunctions<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_sawsf, ctx));
    CS_RUN(serialize(value.m_npfSawsf, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainSimplyAssignableWithoutSerializationFunctions<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_sawsf));
    CS_RUN(deserialize(ctx, value.m_npfSawsf));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainBigStructs1<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_big1, ctx));
    CS_RUN(serialize(value.m_big2, ctx));
    CS_RUN(serialize(value.m_big3, ctx));
    CS_RUN(serialize(value.m_big4, ctx));
    CS_RUN(serialize(value.m_big5, ctx));
    CS_RUN(serialize(value.m_big6, ctx));
    CS_RUN(serialize(value.m_big7, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainBigStructs1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_big1));
    CS_RUN(deserialize(ctx, value.m_big2));
    CS_RUN(deserialize(ctx, value.m_big3));
    CS_RUN(deserialize(ctx, value.m_big4));
    CS_RUN(deserialize(ctx, value.m_big5));
    CS_RUN(deserialize(ctx, value.m_big6));
    CS_RUN(deserialize(ctx, value.m_big7));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainBigStructs2ForAllModes<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_big, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainBigStructs2ForAllModes<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_big));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
