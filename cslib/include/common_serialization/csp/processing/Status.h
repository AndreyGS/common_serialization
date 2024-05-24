/**
 * @file cslib/include/common_serialization/csp/processing/Status.h
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

#include "common_serialization/csp/processing/Contexts.h"

namespace common_serialization::csp::processing
{

constexpr Status serializeStatusFullContext(context::SCommon& ctx, Status statusOut, bool noChecks = false)
{
    if (!noChecks)
        CS_RUN(serializeCommonContext(ctx))
    else
        CS_RUN(serializeCommonContextNoChecks(ctx));
        
    CS_RUN(serializeStatusContext(ctx, statusOut));

    return Status::NoError;
}

CS_ALWAYS_INLINE Status serializeStatusFullContext(BinVectorT& output, protocol_version_t protocolVersion, context::CommonFlags commonFlags, Status statusOut)
{
    context::SCommon ctx{ output, protocolVersion, context::Message::Status, commonFlags };
    return serializeStatusFullContext(ctx, statusOut);
}

inline Status serializeStatusErrorNotSupportedProtocolVersion(BinVectorT& output, const RawVectorT<protocol_version_t>& supportedProtocolVersions, context::CommonFlags commonFlags)
{
    // For unsupported protocol version always using kProtocolVersionUndefined in response context
    context::SCommon ctx{ output, traits::kProtocolVersionUndefined, context::Message::Status, commonFlags };
    CS_RUN(serializeStatusFullContext(ctx, Status::ErrorNotSupportedProtocolVersion, true));

    CS_RUN(writePrimitive(static_cast<protocol_version_t>(supportedProtocolVersions.size()), ctx));
    CS_RUN(writeRawData(supportedProtocolVersions.data(), supportedProtocolVersions.size(), ctx));

    return Status::NoError;
}

constexpr Status deserializeStatusErrorNotSupportedProtocolVersionBody(context::DCommon& ctx, RawVectorT<protocol_version_t>& value)
{
    value.clear();

    protocol_version_t protocolVersionsSize = 0;
    CS_RUN(readPrimitive(ctx, protocolVersionsSize));

    CS_RUN(value.reserve(protocolVersionsSize));
    CS_RUN(readRawData(ctx, protocolVersionsSize, value.data()));
    value.setSize(protocolVersionsSize);
    
    return Status::NoError;
}

constexpr Status serializeStatusErrorNotSupportedInterfaceVersion(interface_version_t minimumInterfaceVersion, const Id& outputTypeId, context::SCommon& ctx)
{
    CS_RUN(serializeStatusFullContext(ctx, Status::ErrorNotSupportedInterfaceVersion));

    CS_RUN(writePrimitive(minimumInterfaceVersion, ctx));
    CS_RUN(writeRawData(&outputTypeId, 1, ctx));

    return Status::NoError;
}

constexpr Status deserializeStatusErrorNotSupportedInterfaceVersionBody(context::DCommon& ctx, interface_version_t& minimumInterfaceVersion, Id& outputTypeId
)
{
    CS_RUN(readPrimitive(ctx, minimumInterfaceVersion));
    CS_RUN(readRawData(ctx, 1, &outputTypeId));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing