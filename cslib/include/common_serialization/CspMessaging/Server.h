/**
 * @file cslib/include/common_serialization/CspMessaging/Server.h
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

#include <common_serialization/CspBase/processing/common/ContextProcessor.h>
#include <common_serialization/CspBase/processing/data/BodyProcessor.h>
#include <common_serialization/CspBase/processing/data/ContextProcessor.h>
#include <common_serialization/CspBase/processing/status/Helpers.h>
#include <common_serialization/CspMessaging/IServerDataHandlerRegistrar.h>
#include <common_serialization/CspMessaging/IServerDataHandlerBase.h>
#include <common_serialization/CspMessaging/service_structs/Interface.h>

namespace common_serialization::csp::messaging
{

/// @brief Common CSP server for handling input/output
/// @details See documentation of CSP
/// @note Thread-safe after intitialization as long as IServerDataHandlerRegistrar is thread-safe.
class Server
{
public:
    Server() = default;

    /// @brief Intitializing constructor
    /// @param settings Server settings. Must be valid or initialization will fail.
    /// @param dataHandlersRegistrar Data handler registrar
    Server(const service_structs::CspPartySettings<>& settings, UniquePtrT<IServerDataHandlerRegistrar>&& dataHandlersRegistrar) noexcept;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) = default;
    Server& operator=(Server&&) = default;


    /// @brief Init server by settings and custom data handler registrar type
    /// @tparam _T Registrar type (must implement IServerDataHandlerRegistrar)
    /// @tparam _Ts Registrar constructor params types
    /// @param settings Server settings. Must be valid or initialization will fail.
    /// @param ts Registrar constructor params
    /// @return Status of operation
    /// @note Can be inited one time. After this if object becomes valid no changes are allowed.
    template<typename _T, typename... _Ts>
    Status init(const service_structs::CspPartySettings<>& settings, _Ts&&... ts) noexcept;

    CS_ALWAYS_INLINE constexpr bool isValid() const noexcept;
    CS_ALWAYS_INLINE const UniquePtrT<IServerDataHandlerRegistrar>& getDataHandlersRegistrar() const noexcept;

    /// @brief Get settings installed in current Server instance
    /// @return Server settings
    constexpr const service_structs::CspPartySettings<>& getSettings() const noexcept;

    /// @brief Entry point for all CSP client requests
    /// @param binInput Binary data received from client
    /// @param binOutput Binary data that should be send back to client
    /// @return Status of operation
    Status handleMessage(BinWalkerT& binInput, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) const;

private:
    CS_ALWAYS_INLINE Status handleGetSettings(protocol_version_t cspVersion, BinVectorT& binOutput) const noexcept;

    /// @brief Common entry point on data messages handling
    /// @param ctxCommon Deserialized from input common context
    /// @param binOutput Binary data output
    /// @return Status of operation
    CS_ALWAYS_INLINE Status handleData(context::DCommon& ctxCommon, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) const;

    service_structs::CspPartySettings<> m_settings;
    UniquePtrT<IServerDataHandlerRegistrar> m_dataHandlersRegistrar;
    bool m_isInited{ false };
};

inline Server::Server(const service_structs::CspPartySettings<>& settings, UniquePtrT<IServerDataHandlerRegistrar>&& dataHandlersRegistrar) noexcept
{
    if (!settings.isValid() || !dataHandlersRegistrar)
        return;

    m_dataHandlersRegistrar = std::move(dataHandlersRegistrar);

    m_isInited = statusSuccess(m_settings.init(settings));
}

template<typename _T, typename... _Ts>
inline Status Server::init(const service_structs::CspPartySettings<>& settings, _Ts&&... ts) noexcept
{
    if (isValid())
        return Status::ErrorAlreadyInited;

    m_dataHandlersRegistrar = makeUniqueNoThrowForOverwrite<_T>(ts...);

    if (!m_dataHandlersRegistrar)
        return Status::ErrorNoMemory;

    if (!settings.isValid())
        return Status::ErrorInvalidArgument;

    CS_RUN(m_settings.init(settings));

    m_isInited = true;

    return Status::NoError;
}

CS_ALWAYS_INLINE constexpr bool Server::isValid() const noexcept
{
    return m_isInited;
}

CS_ALWAYS_INLINE const UniquePtr<IServerDataHandlerRegistrar>& Server::getDataHandlersRegistrar() const noexcept
{
    return m_dataHandlersRegistrar;
}

CS_ALWAYS_INLINE constexpr const service_structs::CspPartySettings<>& Server::getSettings() const noexcept
{
    return m_settings;
}

inline Status Server::handleMessage(BinWalkerT& binInput, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) const
{
    if (!isValid())
        return Status::ErrorNotInited;

    binOutput.clear();

    Status status{ Status::NoError };
    context::DCommon ctx(binInput, m_settings.getOldestProtocolVersion());

    if (statusSuccess(status = processing::common::ContextProcessor::deserialize(ctx)))
        switch (ctx.getMessageType())
        {
        case context::Message::GetSettings:
            status = handleGetSettings(ctx.getProtocolVersion(), binOutput);
            break;
        case context::Message::Data:
            status = processing::common::ContextProcessor::testCommonFlagsCompatibility(ctx.getCommonFlags(), m_settings.getForbiddenCommonFlags(), m_settings.getMandatoryCommonFlags());
            if (statusSuccess(status))
                status = handleData(ctx, clientId, binOutput);
            break;
        default:
            status = Status::ErrorDataCorrupted;
            break;
        }
    else if (status == Status::ErrorNotSupportedProtocolVersion)
        status = processing::status::Helpers::serializeErrorNotSupportedProtocolVersion(binOutput, m_settings.getProtocolVersions(), m_settings.getMandatoryCommonFlags());

    if (binOutput.size() == 0)
        CS_SET_NEW_ERROR(processing::status::Helpers::serializeFullContext(binOutput, ctx.getProtocolVersion(), ctx.getCommonFlags(), status));

    return status;
}

CS_ALWAYS_INLINE Status Server::handleGetSettings(protocol_version_t cspVersion, BinVectorT& binOutput) const noexcept
{
    context::SData ctxOut(binOutput, cspVersion, m_settings.getMandatoryCommonFlags(), {}, true, cspVersion);

    return m_settings.serialize(ctxOut);
}

CS_ALWAYS_INLINE Status Server::handleData(context::DCommon& ctxCommon, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) const
{
    context::DData ctx(ctxCommon);
    Id id;

    CS_RUN(processing::data::ContextProcessor::deserializeNoChecks(ctx, id));

    VectorT<GenericPointerKeeperT> addedPointers;
    if (ctx.allowUnmanagedPointers())
        ctx.setAddedPointers(&addedPointers);

    context::DPointersMap pointersMap;
    if (ctx.checkRecursivePointers())
        ctx.setPointersMap(&pointersMap);

    IServerDataHandlerBase* pHandler{ nullptr };
    Status status = m_dataHandlersRegistrar->aquireHandler(id, pHandler);

    if (statusSuccess(status))
    {
        status = pHandler->handleDataCommon(ctx, clientId, binOutput);
        m_dataHandlersRegistrar->releaseHandler(pHandler);
    }
    else if (status == Status::ErrorMoreEntires) // if we have more than one DataHandler
    {
        RawVectorT<IServerDataHandlerBase*> handlers;
        CS_RUN(m_dataHandlersRegistrar->aquireHandlers(id, handlers));

        status = Status::NoError;

        typename BinVectorT::size_type bodyPosition = ctx.getBinaryData().tell();

        for (auto pHandlerM : handlers)
        {
            CS_SET_NEW_ERROR(pHandlerM->handleDataCommon(ctx, clientId, binOutput));
            m_dataHandlersRegistrar->releaseHandler(pHandlerM);
            ctx.getBinaryData().seek(bodyPosition);
        }
    }

    return status;
}

} // namespace common_serialization::csp::messaging
