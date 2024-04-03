/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/DeserializeLegacy.h
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

#include "interface_for_test/processing/Generated/DeserializeLegacy.h"

namespace common_serialization::csp::processing
{

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOne_Version0<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_ti));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignable_Version0<>& value)
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

    CS_RUN(deserialize(ctx, value.m_vt));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableDescendant_Version0<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<interface_for_test::SimplyAssignable_Version0<>&>(value)));

    CS_RUN(deserialize(ctx, value.m_d));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::AlwaysSimplyAssignable_Version0<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_xx));
    CS_RUN(deserialize(ctx, value.m_yy));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableFixedSize_Version1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_xx));
    CS_RUN(deserialize(ctx, value.m_asa));
    CS_RUN(deserialize(ctx, value.m_arrAsa));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SForAllModesTests_Version0<>& value)
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
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOne_Version1<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_x));
    CS_RUN(deserialize(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status BodyProcessor::deserialize(context::DData<>& ctx, interface_for_test::SForAllModesTests_Version2<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_saS));
    CS_RUN(deserialize(ctx, value.m_diamond));
    CS_RUN(deserialize(ctx, value.m_sptCs));
    CS_RUN(deserialize(ctx, value.m_saaToStS));
    CS_RUN(deserialize(ctx, value.m_saStS));
    CS_RUN(deserialize(ctx, value.m_stS));
    CS_RUN(deserialize(ctx, value.m_mpt));

    CS_RUN(deserialize(ctx, value.m_i));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
