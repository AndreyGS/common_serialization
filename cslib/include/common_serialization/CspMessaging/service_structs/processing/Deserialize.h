/**
 * @file cslib/include/common_serialization/CspMessaging/service_structs/processing/Deserialize.h
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

namespace common_serialization::csp::processing::data
{

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, messaging::service_structs::OutGetInterface<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.properties));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, messaging::service_structs::GetInterface<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.id));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, messaging::service_structs::CspPartySettings<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    // Here needs special handling to be compliant with CSP

    protocol_version_t cspVersionsSize{ 0 };
    CS_RUN(deserialize(ctx, cspVersionsSize));
    value.m_protocolVersions.setSize(cspVersionsSize);
    CS_RUN(deserialize(ctx, cspVersionsSize, value.m_protocolVersions.data()));

    uint32_t mandatoryCommonFlags{ 0 };
    CS_RUN(deserialize(ctx, mandatoryCommonFlags));
    value.m_mandatoryCommonFlags = mandatoryCommonFlags;

    uint32_t forbiddenCommonFlags{ 0 };;
    CS_RUN(deserialize(ctx, forbiddenCommonFlags));
    value.m_forbiddenCommonFlags = forbiddenCommonFlags;

    CS_RUN(deserialize(ctx, value.m_interfaces));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
