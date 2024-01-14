/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/Generated/SerializeDataLegacy.cpp
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

#include "interface_for_test/Generated/SerializeDataLegacy.h"

#define RUN(x)                                                                          \
{                                                                                       \
    if (Status status = (x); !statusSuccess(status))                                    \
        return status;                                                                  \
}

#define SERIALIZE_NO_CONVERSION_COMMON(value, ctx)                                      \
{                                                                                       \
    if constexpr (                                                                      \
           SimplyAssignableType<decltype(value)>                                        \
        || SimplyAssignableAlignedToOneType<decltype(value)>)                           \
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
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableAlignedToOne_Version0<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_ti, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignable_Version0<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

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

    RUN(serializeData(value.m_vt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableDescendant_Version0<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(static_cast<const interface_for_test::SimpleAssignable_Version0<>&>(value), ctx));

    RUN(serializeData(value.m_d, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SForAllModesTests_Version0<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_saDs, ctx));
    RUN(serializeData(value.m_diamond, ctx));
    RUN(serializeData(value.m_sptCs, ctx));
    RUN(serializeData(value.m_saaToStS, ctx));
    RUN(serializeData(value.m_saStS, ctx));
    RUN(serializeData(value.m_stS, ctx));
    RUN(serializeData(value.m_mpt, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SimpleAssignableAlignedToOne_Version1<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_x, ctx));
    RUN(serializeData(value.m_y, ctx));

    return Status::kNoError;
}

template<>
Status DataProcessor::serializeData(const interface_for_test::SForAllModesTests_Version2<>& value, context::SData<>& ctx)
{
    SERIALIZE_NO_CONVERSION_COMMON(value, ctx);

    RUN(serializeData(value.m_saS, ctx));
    RUN(serializeData(value.m_diamond, ctx));
    RUN(serializeData(value.m_sptCs, ctx));
    RUN(serializeData(value.m_saaToStS, ctx));
    RUN(serializeData(value.m_saStS, ctx));
    RUN(serializeData(value.m_stS, ctx));
    RUN(serializeData(value.m_mpt, ctx));

    RUN(serializeData(value.m_i, ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing

#undef SERIALIZE_NO_CONVERSION_COMMON

#undef RUN
