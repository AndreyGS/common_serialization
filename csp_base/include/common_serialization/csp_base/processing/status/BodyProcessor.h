/**
 * @file common_serialization/csp_base/processing/status/BodyProcessor.h
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

#include <common_serialization/csp_base/context/Common.h>
#include <common_serialization/csp_base/processing/rw.h>

namespace common_serialization::csp::processing::status
{

class BodyProcessor
{
public:
    static AGS_CS_ALWAYS_INLINE constexpr Status serializeErrorNotSupportedProtocolVersion(context::SCommon& ctx, const RawVectorT<protocol_version_t>& supportedProtocolVersions);
    static AGS_CS_ALWAYS_INLINE constexpr Status serializeErrorNotSupportedInterfaceVersion(context::SCommon& ctx, interface_version_t minimumInterfaceVersion, const Id& outputTypeId);

    static constexpr Status deserializeErrorNotSupportedProtocolVersion(context::DCommon& ctx, RawVectorT<protocol_version_t>& value);
    static AGS_CS_ALWAYS_INLINE constexpr Status deserializeErrorNotSupportedInterfaceVersion(context::DCommon& ctx, interface_version_t& minimumInterfaceVersion, Id& outputTypeId);
};

AGS_CS_ALWAYS_INLINE constexpr Status BodyProcessor::serializeErrorNotSupportedProtocolVersion(context::SCommon& ctx, const RawVectorT<protocol_version_t>& supportedProtocolVersions)
{
    AGS_CS_RUN(writePrimitive(static_cast<protocol_version_t>(supportedProtocolVersions.size()), ctx));
    return writeRawData(supportedProtocolVersions.data(), supportedProtocolVersions.size(), ctx);
}

AGS_CS_ALWAYS_INLINE constexpr Status BodyProcessor::serializeErrorNotSupportedInterfaceVersion(context::SCommon& ctx, interface_version_t minimumInterfaceVersion, const Id& outputTypeId)
{
    AGS_CS_RUN(writePrimitive(minimumInterfaceVersion, ctx));
    return writeRawData(&outputTypeId, 1, ctx);
}

constexpr Status BodyProcessor::deserializeErrorNotSupportedProtocolVersion(context::DCommon& ctx, RawVectorT<protocol_version_t>& value)
{
    value.clear();

    protocol_version_t protocolVersionsSize = 0;
    AGS_CS_RUN(readPrimitive(ctx, protocolVersionsSize));

    AGS_CS_RUN(value.reserve(protocolVersionsSize));
    AGS_CS_RUN(readRawData(ctx, protocolVersionsSize, value.data()));
    value.setSize(protocolVersionsSize);

    return Status::NoError;
}

AGS_CS_ALWAYS_INLINE constexpr Status BodyProcessor::deserializeErrorNotSupportedInterfaceVersion(context::DCommon& ctx, interface_version_t& minimumInterfaceVersion, Id& outputTypeId)
{
    AGS_CS_RUN(readPrimitive(ctx, minimumInterfaceVersion));
    return readRawData(ctx, 1, &outputTypeId);
}

} // namespace common_serialization::csp::processing::status
