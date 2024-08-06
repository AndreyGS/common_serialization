/**
 * @file cslib/include/common_serialization/csp/processing/status/Helpers.h
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

#include <common_serialization/csp/processing/common/ContextProcessor.h>
#include <common_serialization/csp/processing/status/BodyProcessor.h>
#include <common_serialization/csp/processing/status/ContextProcessor.h>

namespace common_serialization::csp::processing::status
{

class Helpers
{
public:
    static constexpr Status serializeFullContext(context::SCommon& ctx, Status statusOut, bool noChecks = false);
    static CS_ALWAYS_INLINE Status serializeFullContext(BinVectorT& output, protocol_version_t protocolVersion, context::CommonFlags commonFlags, Status statusOut);

    static constexpr Status serializeErrorNotSupportedProtocolVersion(BinVectorT& output, const RawVectorT<protocol_version_t>& supportedProtocolVersions, context::CommonFlags commonFlags);
    static constexpr Status serializeErrorNotSupportedInterfaceVersion(protocol_version_t protocolVersion, context::CommonFlags commonFlags
        , interface_version_t minimumInterfaceVersion, const Id& outputTypeId, BinVectorT& output);
};

constexpr Status Helpers::serializeFullContext(context::SCommon& ctx, Status statusOut, bool noChecks)
{
    if (!noChecks)
        CS_RUN(common::ContextProcessor::serialize(ctx))
    else
        CS_RUN(common::ContextProcessor::serializeNoChecks(ctx));
        
    CS_RUN(status::ContextProcessor::serialize(ctx, statusOut));

    return Status::NoError;
}

CS_ALWAYS_INLINE Status Helpers::serializeFullContext(BinVectorT& output, protocol_version_t protocolVersion, context::CommonFlags commonFlags, Status statusOut)
{
    context::SCommon ctx{ output, protocolVersion, context::Message::Status, commonFlags };
    return serializeFullContext(ctx, statusOut);
}

constexpr Status Helpers::serializeErrorNotSupportedProtocolVersion(BinVectorT& output, const RawVectorT<protocol_version_t>& supportedProtocolVersions, context::CommonFlags commonFlags)
{
    // For unsupported protocol version always using kProtocolVersionUndefined in response context
    context::SCommon ctx{ output, traits::kProtocolVersionUndefined, context::Message::Status, commonFlags };
    CS_RUN(serializeFullContext(ctx, Status::ErrorNotSupportedProtocolVersion, true));
    return status::BodyProcessor::serializeErrorNotSupportedProtocolVersion(ctx, supportedProtocolVersions);
}

constexpr Status Helpers::serializeErrorNotSupportedInterfaceVersion(protocol_version_t protocolVersion, context::CommonFlags commonFlags
    , interface_version_t minimumInterfaceVersion, const Id& outputTypeId, BinVectorT& output)
{
    context::SCommon ctx(output, protocolVersion, context::Message::Status, commonFlags);
    CS_RUN(serializeFullContext(ctx, Status::ErrorNotSupportedInterfaceVersion));
    return status::BodyProcessor::serializeErrorNotSupportedInterfaceVersion(ctx, minimumInterfaceVersion, outputTypeId);
}
} // namespace common_serialization::csp::processing::status
