/**
 * @file cslib/include/common_serialization/csp/messaging/Server.h
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

#include "common_serialization/Containers/UniquePtr.h"
#include "common_serialization/csp/context/Common.h"
#include "common_serialization/csp/messaging/IDataHandlersRegistrar.h"
#include "common_serialization/csp/messaging/IDataHandlerBase.h"
#include "common_serialization/csp/service_structs/Interface.h"

namespace common_serialization::csp::messaging
{

/// @brief Common CSP server for handling input/output
/// @note Thread-safe as long as IDataHandlersRegistrar is thread-safe.
///     Also there is no memory barriers and because of this
///     there can be data race if init function will be invoked
///     with any other function in parallel.
///     So, before using object you should successfully init it and 
///     only after this all it can be used safely in multithreaded environment.
class Server
{
public:
    Server() = default;

    Server(const service_structs::CspPartySettings<>& settings, UniquePtr<IDataHandlersRegistrar>&& dataHandlersRegistrar) noexcept;

    template<typename T>
    Status init(const service_structs::CspPartySettings<>& settings) noexcept;

    CS_ALWAYS_INLINE constexpr bool isValid() const noexcept;

    /// @brief Entry point for all CSP client requests
    /// @param binInput Binary data received from client
    /// @param binOutput Binary data that should be send back to client
    /// @return Status of operation
    Status handleMessage(BinWalker& binInput, const GenericPointerKeeper& clientId, BinVector& binOutput) const;

    CS_ALWAYS_INLINE const UniquePtr<IDataHandlersRegistrar>& getDataHandlersRegistrar() const noexcept;

private:
    CS_ALWAYS_INLINE Status handleGetSettings(protocol_version_t cspVersion, BinVector& binOutput) const noexcept;

    /// @brief Common entry point on data messages handling
    /// @param ctxCommon Deserialized from input common context
    /// @param binOutput Binary data output
    /// @return Status of operation
    CS_ALWAYS_INLINE Status handleData(context::Common<BinWalker>& ctxCommon, const GenericPointerKeeper& clientId, BinVector& binOutput) const;

    service_structs::CspPartySettings<> m_settings;
    UniquePtr<IDataHandlersRegistrar> m_dataHandlersRegistrar;
    bool m_isInited{ false };
};

inline Server::Server(const service_structs::CspPartySettings<>& settings, UniquePtr<IDataHandlersRegistrar>&& dataHandlersRegistrar) noexcept
{
    if (!settings.isValid() || !dataHandlersRegistrar)
        return;

    m_dataHandlersRegistrar = std::move(dataHandlersRegistrar);

    m_isInited = statusSuccess(m_settings.init(settings));
}

template<typename T>
inline Status Server::init(const service_structs::CspPartySettings<>& settings) noexcept
{
    m_isInited = false;

    m_dataHandlersRegistrar = std::move(makeUniqueNoThrowForOverwrite<T>());

    if (!m_dataHandlersRegistrar)
        return Status::kErrorNoMemory;

    if (!settings.isValid())
        return Status::kErrorInvalidArgument;

    CS_RUN(m_settings.init(settings));

    m_isInited = true;

    return Status::kNoError;
}

CS_ALWAYS_INLINE constexpr bool Server::isValid() const noexcept
{
    return m_isInited;
}

inline Status Server::handleMessage(BinWalker& binInput, const GenericPointerKeeper& clientId, BinVector& binOutput) const
{
    if (!isValid())
        return Status::kErrorNotInited;

    context::Common<BinWalker> ctx(binInput, m_settings.protocolVersions[m_settings.protocolVersions.size() - 1]);

    if (Status status = processing::deserializeCommonContext(ctx); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedProtocolVersion)
        {
            binOutput.clear();
            return processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, m_settings.protocolVersions, m_settings.mandatoryCommonFlags);
        }
        else
            return status;
    }

    Status status{ Status::kNoError };

    if (ctx.getMessageType() == context::Message::GetSettings)
        status = handleGetSettings(ctx.getProtocolVersion(), binOutput);

    else if (ctx.getMessageType() == context::Message::Data)
    {
        status = processing::testCommonFlagsCompatibility(ctx.getCommonFlags(), m_settings.forbiddenCommonFlags, m_settings.mandatoryCommonFlags);
        if (statusSuccess(status))
            status = handleData(ctx, clientId, binOutput);
    }
    else
        status = Status::kErrorDataCorrupted;

    if (binOutput.size() == 0)
        status = processing::serializeStatusFullContext(binOutput, ctx.getProtocolVersion(), m_settings.mandatoryCommonFlags, status);

    return status;
}

CS_ALWAYS_INLINE const UniquePtr<IDataHandlersRegistrar>& Server::getDataHandlersRegistrar() const noexcept
{
    return m_dataHandlersRegistrar;
}

CS_ALWAYS_INLINE Status Server::handleGetSettings(protocol_version_t cspVersion, BinVector& binOutput) const noexcept
{
    binOutput.clear();

    context::SData<BinVector> ctxOut(binOutput, cspVersion, m_settings.mandatoryCommonFlags, context::DataFlags{}, true, cspVersion);

    return m_settings.serialize(ctxOut);
}

CS_ALWAYS_INLINE Status Server::handleData(context::Common<BinWalker>& ctxCommon, const GenericPointerKeeper& clientId, BinVector& binOutput) const
{
    context::DData<> ctx(ctxCommon);
    Id id;

    CS_RUN(processing::deserializeDataContext(ctx, id));

    Vector<GenericPointerKeeper> addedPointers;
    if (ctx.allowUnmanagedPointers())
        ctx.setAddedPointers(&addedPointers);

    std::unordered_map<uint64_t, void*> pointersMap;
    if (ctx.checkRecursivePointers())
        ctx.setPointersMap(&pointersMap);

    IDataHandlerBase* pDataHandler{ nullptr };
    Status status = Status::kNoError;

    if (Status status = m_dataHandlersRegistrar->findHandler(id, pDataHandler); statusSuccess(status))
    {
        status = pDataHandler->handleDataCommon(ctx, clientId, binOutput);
    }
    // If we have more than one DataServer
    else if (status == Status::kErrorMoreEntires)
    {
        Vector<IDataHandlerBase*, RawStrategicAllocatorHelper<IDataHandlerBase*>> dataHandlers;
        CS_RUN(m_dataHandlersRegistrar->findHandlers(id, dataHandlers));

        status = Status::kNoError;

        typename BinWalker::size_type bodyPosition = ctx.getBinaryData().tell();

        for (auto pDataHandler : dataHandlers)
        {
            SET_NEW_ERROR(pDataHandler->handleDataCommon(ctx, clientId, binOutput));
            ctx.getBinaryData().seek(bodyPosition);
        }
    }

    return status;
}

} // namespace common_serialization::csp::messaging
