/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/Deserialize.cpp
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

#include "interface_for_test/Interface.h"

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOne<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignable<>& value)
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

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableDescendant<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<interface_for_test::SimplyAssignable<>&>(value)));

    CS_RUN(deserialize(ctx, value.m_d));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::AlwaysSimplyAssignable<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableFixedSize<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_asa));
    CS_RUN(deserialize(ctx, value.m_arrAsa));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::DynamicPolymorphic<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_o));
    CS_RUN(deserialize(ctx, value.m_dpNS));

    CS_RUN(deserialize(ctx, value.m_arrO));
    CS_RUN(deserialize(ctx, value.m_arrDpNS));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::Diamond<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<not_part_of_interfaces::DiamondEdge1&>(value)));
    CS_RUN(deserialize(ctx, static_cast<not_part_of_interfaces::DiamondEdge2&>(value)));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SpecialProcessingType<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_vec));
    CS_RUN(deserialize(ctx, value.m_saaToNS));
    CS_RUN(deserialize(ctx, value.m_saNS));
    CS_RUN(deserialize(ctx, value.m_pVec));
    CS_RUN(deserialize(ctx, value.m_pInt));
    CS_RUN(deserialize(ctx, value.m_ppInt));
    CS_RUN(deserialize(ctx, value.m_nullptrInt));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOneSimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOneSimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableSimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableSimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimilarType1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));
    CS_RUN(deserialize(ctx, value.m_arrL));
    CS_RUN(deserialize(ctx, value.m_sasTs));
    CS_RUN(deserialize(ctx, value.m_saaToSts));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimilarType2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_j));
    CS_RUN(deserialize(ctx, value.m_k));
    CS_RUN(deserialize(ctx, value.m_arrL));
    CS_RUN(deserialize(ctx, value.m_sasTs));
    CS_RUN(deserialize(ctx, value.m_saaToSts));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::ManyPointersType<>& value)
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

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::DForAllModesTests<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_saDs));
    CS_RUN(deserialize(ctx, value.m_diamond));
    CS_RUN(deserialize(ctx, value.m_sptCs));
    CS_RUN(deserialize(ctx, value.m_saaToStS));
    CS_RUN(deserialize(ctx, value.m_saStS));
    CS_RUN(deserialize(ctx, value.m_stS));
    CS_RUN(deserialize(ctx, value.m_mpt));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::ContainSimplyAssignableWithoutSerializationFunctions<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_sawsf));
    CS_RUN(deserialize(ctx, value.m_npfSawsf));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing::data
