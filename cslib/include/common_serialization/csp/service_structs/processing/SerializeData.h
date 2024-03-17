
/**
 * @file cslib/include/common_serialization/csp/service_structs/processing/SerializeData.h
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

#include "common_serialization/csp/processing/DataTemplates.h"

namespace common_serialization::csp::processing
{

template<>
constexpr Status DataProcessor::serializeData(const service_structs::OutGetInterface<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    RUN(serializeData(value.properties, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const service_structs::GetInterface<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    RUN(serializeData(value.id, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const service_structs::CspPartySettings<>& value, context::SData<>& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    assert(value.protocolVersions.size() < traits::kProtocolVersionUndefined);

    RUN(serializeData(static_cast<protocol_version_t>(value.protocolVersions.size()), ctx));
    RUN(serializeData(value.protocolVersions.data(), static_cast<protocol_version_t>(value.protocolVersions.size()), ctx));

    RUN(serializeData(static_cast<uint32_t>(value.mandatoryCommonFlags), ctx));
    RUN(serializeData(static_cast<uint32_t>(value.forbiddenCommonFlags), ctx));

    RUN(serializeData(value.interfaces.size(), ctx));
    RUN(serializeData(value.interfaces.data(), value.interfaces.size(), ctx));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
