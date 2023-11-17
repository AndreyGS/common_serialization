/**
 * @file DeserializeDataLegacy.h
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

#include "Generated/DeserializeDataLegacy.h"

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSerializable_Version0<>& value)
{
    RUN(deserializeData(ctx, value.m_ti));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableSerializable_Version0<>& value)
{
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
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableDescendantSerializable_Version0<>& value)
{
    RUN(deserializeData(ctx, static_cast<special_types::SimpleAssignableSerializable_Version0<>&>(value)));

    RUN(deserializeData(ctx, value.m_d));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::ForAllFlagsTests1_Version0<>& value)
{
    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::ForAllFlagsTests2_Version0<>& value)
{
    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSerializable_Version1<>& value)
{
    RUN(deserializeData(ctx, value.m_x));
    RUN(deserializeData(ctx, value.m_y));

    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::ForAllFlagsTests1_Version2<>& value)
{
    return Status::kNoError;
}

template<>
Status DataProcessor::deserializeDataLegacy(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::ForAllFlagsTests2_Version2<>& value)
{
    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN
