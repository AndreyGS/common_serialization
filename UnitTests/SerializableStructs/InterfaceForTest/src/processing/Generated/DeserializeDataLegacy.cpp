/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/DeserializeDataLegacy.h
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

#include "interface_for_test/processing/Generated/DeserializeDataLegacy.h"

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOne_Version0<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_ti));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimplyAssignable_Version0<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

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

    RUN(deserializeData(ctx, value.m_vt));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimplyAssignableDescendant_Version0<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, static_cast<interface_for_test::SimplyAssignable_Version0<>&>(value)));

    RUN(deserializeData(ctx, value.m_d));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SForAllModesTests_Version0<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_saDs));
    RUN(deserializeData(ctx, value.m_diamond));
    RUN(deserializeData(ctx, value.m_sptCs));
    RUN(deserializeData(ctx, value.m_saaToStS));
    RUN(deserializeData(ctx, value.m_saStS));
    RUN(deserializeData(ctx, value.m_stS));
    RUN(deserializeData(ctx, value.m_mpt));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimplyAssignableAlignedToOne_Version1<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_x));
    RUN(deserializeData(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SForAllModesTests_Version2<>& value)
{
    CSP_DESERIALIZE_NO_CONVERSION_COMMON(ctx, value);

    RUN(deserializeData(ctx, value.m_saS));
    RUN(deserializeData(ctx, value.m_diamond));
    RUN(deserializeData(ctx, value.m_sptCs));
    RUN(deserializeData(ctx, value.m_saaToStS));
    RUN(deserializeData(ctx, value.m_saStS));
    RUN(deserializeData(ctx, value.m_stS));
    RUN(deserializeData(ctx, value.m_mpt));

    RUN(deserializeData(ctx, value.m_i));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
