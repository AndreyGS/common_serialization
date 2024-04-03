/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/SerializeLegacy.cpp
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

#include "interface_for_test/processing/Generated/SerializeLegacy.h"

namespace common_serialization::csp::processing
{

template<>
Status BodyProcessor::serialize(const interface_for_test::SimplyAssignableAlignedToOne_Version0<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_ti, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SimplyAssignable_Version0<>& value, context::SData<>& ctx)
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

    CS_RUN(serialize(value.m_vt, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SimplyAssignableDescendant_Version0<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(static_cast<const interface_for_test::SimplyAssignable_Version0<>&>(value), ctx));

    CS_RUN(serialize(value.m_d, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::AlwaysSimplyAssignable_Version0<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_xx, ctx));
    CS_RUN(serialize(value.m_yy, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SimplyAssignableFixedSize_Version1<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_xx, ctx));
    CS_RUN(serialize(value.m_asa, ctx));
    CS_RUN(serialize(value.m_arrAsa, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SForAllModesTests_Version0<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_saDs, ctx));
    CS_RUN(serialize(value.m_diamond, ctx));
    CS_RUN(serialize(value.m_sptCs, ctx));
    CS_RUN(serialize(value.m_saaToStS, ctx));
    CS_RUN(serialize(value.m_saStS, ctx));
    CS_RUN(serialize(value.m_stS, ctx));
    CS_RUN(serialize(value.m_mpt, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SimplyAssignableAlignedToOne_Version1<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_x, ctx));
    CS_RUN(serialize(value.m_y, ctx));

    return Status::kNoError;
}

template<>
Status BodyProcessor::serialize(const interface_for_test::SForAllModesTests_Version2<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_saS, ctx));
    CS_RUN(serialize(value.m_diamond, ctx));
    CS_RUN(serialize(value.m_sptCs, ctx));
    CS_RUN(serialize(value.m_saaToStS, ctx));
    CS_RUN(serialize(value.m_saStS, ctx));
    CS_RUN(serialize(value.m_stS, ctx));
    CS_RUN(serialize(value.m_mpt, ctx));

    CS_RUN(serialize(value.m_i, ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
