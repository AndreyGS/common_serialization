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
template<SdContainers _Sdcs = traits::DefaultSdContainers>
class Server
{
public:
    using Sdcs = _Sdcs;
    using Sbin = typename Sdcs::Sbin;
    using Dbin = typename Sdcs::Dbin;
    using Spm = typename Sdcs::Spm;
    using Dpm = typename Sdcs::Dpm;
    using Gkc = typename Sdcs::Gkc;

    using Scs = typename Sdcs::Scs;
    using Dcs = typename Sdcs::Dcs;

    Server() = default;

    Server(const service_structs::CspPartySettings<>& settings, UniquePtr<IDataHandlersRegistrar<Sdcs>>&& dataHandlersRegistrar) noexcept;

    template<typename _T>
    Status init(const service_structs::CspPartySettings<>& settings) noexcept;

    CS_ALWAYS_INLINE constexpr bool isValid() const noexcept;

    /// @brief Entry point for all CSP client requests
    /// @param binInput Binary data received from client
    /// @param binOutput Binary data that should be send back to client
    /// @return Status of operation
    Status handleMessage(Dbin& binInput, const GenericPointerKeeper& clientId, Sbin& binOutput) const;

    CS_ALWAYS_INLINE const UniquePtr<IDataHandlersRegistrar<Sdcs>>& getDataHandlersRegistrar() const noexcept;

private:
    CS_ALWAYS_INLINE Status handleGetSettings(protocol_version_t cspVersion, Sbin& binOutput) const noexcept;

    /// @brief Common entry point on data messages handling
    /// @param ctxCommon Deserialized from input common context
    /// @param binOutput Binary data output
    /// @return Status of operation
    CS_ALWAYS_INLINE Status handleData(context::Common<Dbin>& ctxCommon, const GenericPointerKeeper& clientId, Sbin& binOutput) const;

    service_structs::CspPartySettings<> m_settings;
    UniquePtr<IDataHandlersRegistrar<Sdcs>> m_dataHandlersRegistrar;
    bool m_isInited{ false };
};

template<SdContainers _Sdcs>
Server<_Sdcs>::Server(const service_structs::CspPartySettings<>& settings, UniquePtr<IDataHandlersRegistrar<Sdcs>>&& dataHandlersRegistrar) noexcept
{
    if (!settings.isValid() || !dataHandlersRegistrar)
        return;

    m_dataHandlersRegistrar = std::move(dataHandlersRegistrar);

    m_isInited = statusSuccess(m_settings.init(settings));
}

template<SdContainers _Sdcs>
template<typename _T>
Status Server<_Sdcs>::init(const service_structs::CspPartySettings<>& settings) noexcept
{
    m_isInited = false;

    m_dataHandlersRegistrar = std::move(makeUniqueNoThrowForOverwrite<_T>());

    if (!m_dataHandlersRegistrar)
        return Status::kErrorNoMemory;

    if (!settings.isValid())
        return Status::kErrorInvalidArgument;

    CS_RUN(m_settings.init(settings));

    m_isInited = true;

    return Status::kNoError;
}

template<SdContainers _Sdcs>
CS_ALWAYS_INLINE constexpr bool Server<_Sdcs>::isValid() const noexcept
{
    return m_isInited;
}

template<SdContainers _Sdcs>
inline Status Server<_Sdcs>::handleMessage(Dbin& binInput, const GenericPointerKeeper& clientId, Sbin& binOutput) const
{
    if (!isValid())
        return Status::kErrorNotInited;

    context::Common<Dbin> ctx(binInput, m_settings.protocolVersions[m_settings.protocolVersions.size() - 1]);

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

template<SdContainers _Sdcs>
CS_ALWAYS_INLINE const UniquePtr<IDataHandlersRegistrar<_Sdcs>>& Server<_Sdcs>::getDataHandlersRegistrar() const noexcept
{
    return m_dataHandlersRegistrar;
}

template<SdContainers _Sdcs>
CS_ALWAYS_INLINE Status Server<_Sdcs>::handleGetSettings(protocol_version_t cspVersion, Sbin& binOutput) const noexcept
{
    binOutput.clear();

    context::Data<Scs> ctxOut(binOutput, cspVersion, m_settings.mandatoryCommonFlags, {}, true, cspVersion);

    return m_settings.serialize(ctxOut);
}

template<SdContainers _Sdcs>
CS_ALWAYS_INLINE Status Server<_Sdcs>::handleData(context::Common<Dbin>& ctxCommon, const GenericPointerKeeper& clientId, Sbin& binOutput) const
{
    context::Data<Dcs> ctx(ctxCommon);
    Id id;

    CS_RUN(processing::deserializeDataContext(ctx, id));

    Gkc addedPointers;
    if (ctx.allowUnmanagedPointers())
        ctx.setAddedPointers(&addedPointers);

    Dpm pointersMap;
    if (ctx.checkRecursivePointers())
        ctx.setPointersMap(&pointersMap);

    IDataHandlerBase<Sdcs>* pDataHandler{ nullptr };
    Status status = m_dataHandlersRegistrar->findHandler(id, pDataHandler);

    if (statusSuccess(status))
    {
        status = pDataHandler->handleDataCommon(ctx, clientId, binOutput);
    }
    else if (status == Status::kErrorMoreEntires) // if we have more than one DataHandler
    {
        Vector<IDataHandlerBase<Sdcs>*, RawStrategicAllocatorHelper<IDataHandlerBase<Sdcs>*>> dataHandlers;
        CS_RUN(m_dataHandlersRegistrar->findHandlers(id, dataHandlers));

        status = Status::kNoError;

        typename Dbin::size_type bodyPosition = ctx.getBinaryData().tell();

        for (auto pDataHandler : dataHandlers)
        {
            SET_NEW_ERROR(pDataHandler->handleDataCommon(ctx, clientId, binOutput));
            ctx.getBinaryData().seek(bodyPosition);
        }
    }

    return status;
}

} // namespace common_serialization::csp::messaging
