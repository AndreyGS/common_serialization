/**
 * @file cslib/include/common_serialization/csp/messaging/CommonServer.h
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

#include "common_serialization/csp/context/Common.h"
#include "common_serialization/csp/messaging/IDataServerBase.h"


namespace common_serialization::csp::messaging
{

/// @brief Common CSP server for handling input/output
class CommonServer
{
public:
    /// @brief Entry point for CSP client requests
    /// @param binInput Binary data received from client
    /// @param binOutput Binary data that should be send back to client
    /// @return Status of operation
    static Status handleMessage(BinWalker& binInput, BinVector& binOutput);

    static constexpr context::CommonFlags kOutMandatoryCommonFlags{ helpers::isModuleIsBigEndian() };

private:
    static Status handleCommonCapabilitiesRequest(context::Common<BinWalker>& ctx, BinVector& binOutput);
};

inline Status CommonServer::handleMessage(BinWalker& binInput, BinVector& binOutput)
{
    context::Common<BinWalker> ctx(binInput);

    if (Status status = processing::deserializeCommonContext(ctx); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedProtocolVersion)
        {
            binOutput.clear();
            return processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, kOutMandatoryCommonFlags);
        }
        else
            return status;
    }

    Status status{ Status::kNoError };

    if (ctx.getMessageType() == context::Message::kInOutData)
        status = IDataServerBase::handleDataCommon(ctx, binOutput);
    else if (ctx.getMessageType() == context::Message::kCommonCapabilitiesRequest)
        status = handleCommonCapabilitiesRequest(ctx, binOutput);
    else
        status = Status::kErrorDataCorrupted;

    if (binOutput.size() == 0)
        status = processing::serializeStatusNoContext(binOutput, ctx.getProtocolVersion(), kOutMandatoryCommonFlags, status);

    return status;
}

inline Status CommonServer::handleCommonCapabilitiesRequest(context::Common<BinWalker>& ctx, BinVector& binOutput)
{
    context::CommonCapabilities requestedCapability;

    RUN(processing::deserializeCommonCapabilitiesRequest(ctx, requestedCapability));

    binOutput.clear();

    context::SData<BinVector> ctxOut(binOutput, 1);

    if (requestedCapability == context::CommonCapabilities::kSupportedProtocolVersions)
    {
        messaging::SupportedProtocolVersions<> supportedProtocolVersions;
        RUN(supportedProtocolVersions.list.pushBackN(traits::kProtocolVersions, std::size(traits::kProtocolVersions)));
        RUN(supportedProtocolVersions.serialize(ctxOut));
    }
    else
        return Status::kErrorDataCorrupted;

    return Status::kNoError;
}

} // namespace common_serialization::csp::messaging
