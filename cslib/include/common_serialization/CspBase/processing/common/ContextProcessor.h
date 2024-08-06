/**
 * @file cslib/include/common_serialization/CspBase/processing/common/ContextProcessor.h
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

#include <common_serialization/CspBase/context/Common.h>
#include <common_serialization/CspBase/processing/Rw.h>

namespace common_serialization::csp::processing::common
{

class ContextProcessor
{
public:
    static constexpr [[nodiscard]] Status testCommonFlagsCompatibility(context::CommonFlags commonFlags
        , context::CommonFlags forbiddenCommonFlags, context::CommonFlags mandatoryCommonFlags);

    static constexpr Status serialize(context::SCommon& ctx);
    static constexpr Status serializeNoChecks(context::SCommon& ctx);

    static constexpr Status deserialize(context::DCommon& ctx) noexcept;
    static constexpr Status deserializeNoChecks(context::DCommon& ctx) noexcept;
};

constexpr Status ContextProcessor::testCommonFlagsCompatibility(context::CommonFlags commonFlags
    , context::CommonFlags forbiddenCommonFlags, context::CommonFlags mandatoryCommonFlags)
{
    return commonFlags & (forbiddenCommonFlags | context::CommonFlags::kForbiddenFlagsMask) || (commonFlags & mandatoryCommonFlags) != mandatoryCommonFlags
        ? Status::ErrorNotCompatibleCommonFlagsSettings
        : Status::NoError;
}

constexpr Status ContextProcessor::serialize(context::SCommon& ctx)
{
    if (!traits::isProtocolVersionSupported(ctx.getProtocolVersion()))
        return Status::ErrorNotSupportedProtocolVersion;
    else if (ctx.isEndiannessNotMatch() && !ctx.endiannessDifference())
        return Status::ErrorNotCompatibleCommonFlagsSettings;

    return serializeNoChecks(ctx);
}

constexpr Status ContextProcessor::serializeNoChecks(context::SCommon& ctx)
{
    // Common context is always in little-endiann format.
    // CommonFlags should have no impact here.
    context::SCommon commonContextSpecial(ctx.getBinaryData());
    commonContextSpecial.setCommonFlags(context::CommonFlags::kNoFlagsMask);

    CS_RUN(writePrimitive(static_cast<uint16_t>(ctx.getProtocolVersion()), commonContextSpecial));
    CS_RUN(writePrimitive(ctx.getMessageType(), commonContextSpecial));
    CS_RUN(writePrimitive(static_cast<uint32_t>(ctx.getCommonFlags()), commonContextSpecial));

    return Status::NoError;
}

constexpr Status ContextProcessor::deserialize(context::DCommon& ctx) noexcept
{
    // Common context is always in little-endiann format.
    // CommonFlags should have no impact here.
    context::DCommon commonContextSpecial(ctx.getBinaryData());
    commonContextSpecial.setCommonFlags(context::CommonFlags::kNoFlagsMask);

    uint16_t version = 0;
    CS_RUN(readPrimitive(commonContextSpecial, version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    if (minimumSupportedVersion > ctx.getProtocolVersion() || traits::getLatestProtocolVersion() < ctx.getProtocolVersion())
        return Status::ErrorNotSupportedProtocolVersion;

    context::Message messageType = context::Message::Data;
    CS_RUN(readPrimitive(commonContextSpecial, messageType));
    ctx.setMessageType(messageType);

    uint32_t intFlags = 0;
    CS_RUN(readPrimitive(commonContextSpecial, intFlags));
    context::CommonFlags commonFlags(intFlags);
    ctx.setCommonFlags(commonFlags);
    if (ctx.isEndiannessNotMatch() && !ctx.endiannessDifference())
        return Status::ErrorNotCompatibleCommonFlagsSettings;

    return Status::NoError;
}

constexpr Status ContextProcessor::deserializeNoChecks(context::DCommon& ctx) noexcept
{
    // Common context must always be deserialized by the same rules
    // no matter of CommonFlags impact - it must always be in little-endian format
    context::DCommon commonContextSpecial(ctx.getBinaryData(), ctx.getProtocolVersion()
        , ctx.getMessageType(), context::CommonFlags{ context::CommonFlags::kNoFlagsMask });

    uint16_t version = 0;
    CS_RUN(readPrimitive(commonContextSpecial, version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    context::Message messageType = context::Message::Data;
    CS_RUN(readPrimitive(commonContextSpecial, messageType));

    ctx.setMessageType(messageType);

    uint32_t intFlags = 0;
    CS_RUN(readPrimitive(commonContextSpecial, intFlags));
    context::CommonFlags commonFlags(intFlags);
    ctx.setCommonFlags(commonFlags);

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::common
