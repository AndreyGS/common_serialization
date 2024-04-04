/**
 * @file cslib/include/common_serialization/csp/messaging/CommonServer.h
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

#include "common_serialization/csp/context/Common.h"
#include "common_serialization/csp/messaging/IDataServerBase.h"
#include "common_serialization/csp/service_structs/Interface.h"

namespace common_serialization::csp::messaging
{

/// @brief Common CSP server for handling input/output
class CommonServer
{
public:
    CommonServer() = default;

    CommonServer(const service_structs::CspPartySettings<>& serverSettings);

    Status init(const service_structs::CspPartySettings<>& serverSettings) noexcept;
    constexpr bool isValid() const noexcept;

    /// @brief Entry point for CSP client requests
    /// @param binInput Binary data received from client
    /// @param binOutput Binary data that should be send back to client
    /// @return Status of operation
    Status handleMessage(BinWalker& binInput, const BinVector& clientId, BinVector& binOutput) const noexcept;

private:
    Status handleGetSettingsRequest(protocol_version_t cspVersion, BinVector& binOutput) const noexcept;

    service_structs::CspPartySettings<> m_serverSettings;
};

inline CommonServer::CommonServer(const service_structs::CspPartySettings<>& serverSettings)
{
    init(serverSettings);
}

inline Status CommonServer::init(const service_structs::CspPartySettings<>& serverSettings) noexcept
{
    m_serverSettings.clear();

    if (!serverSettings.isValid())
        return Status::kErrorInvalidArgument;

    return m_serverSettings.init(serverSettings);
}

constexpr bool CommonServer::isValid() const noexcept
{
    return m_serverSettings.isValid();
}

inline Status CommonServer::handleMessage(BinWalker& binInput, const BinVector& clientId, BinVector& binOutput) const noexcept
{
    if (!isValid())
        return Status::kErrorNotInited;

    context::Common<BinWalker> ctx(binInput, m_serverSettings.protocolVersions[m_serverSettings.protocolVersions.size() - 1]);

    if (Status status = processing::deserializeCommonContext(ctx); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedProtocolVersion)
        {
            binOutput.clear();
            return processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, m_serverSettings.protocolVersions, m_serverSettings.mandatoryCommonFlags);
        }
        else
            return status;
    }

    Status status{ Status::kNoError };

    if (ctx.getMessageType() == context::Message::kGetSettings)
        status = handleGetSettingsRequest(ctx.getProtocolVersion(), binOutput);

    else if (ctx.getMessageType() == context::Message::kData)
    {
        status = processing::testCommonFlagsCompatibility(ctx.getCommonFlags(), m_serverSettings.forbiddenCommonFlags, m_serverSettings.mandatoryCommonFlags);
        if (statusSuccess(status))
            status = IDataServerBase::handleDataCommon(ctx, clientId, binOutput);
    }
    else
        status = Status::kErrorDataCorrupted;

    if (binOutput.size() == 0)
        status = processing::serializeStatusFullContext(binOutput, ctx.getProtocolVersion(), m_serverSettings.mandatoryCommonFlags, status);

    return status;
}

inline Status CommonServer::handleGetSettingsRequest(protocol_version_t cspVersion, BinVector& binOutput) const noexcept
{
    binOutput.clear();

    context::SData<BinVector> ctxOut(binOutput, cspVersion, m_serverSettings.mandatoryCommonFlags, context::DataFlags{}, true, cspVersion);

    return m_serverSettings.serialize(ctxOut);
}

} // namespace common_serialization::csp::messaging
