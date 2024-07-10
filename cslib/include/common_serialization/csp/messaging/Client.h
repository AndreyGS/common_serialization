/**
 * @file cslib/include/common_serialization/csp/messaging/Client.h
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

#include "common_serialization/csp/Concepts.h"
#include "common_serialization/csp/messaging/IClientDataHandlerTraits.h"
#include "common_serialization/csp/messaging/IClientSpeaker.h"
#include "common_serialization/csp/messaging/service_structs/Interface.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataBodyProcessor.h"
#include "common_serialization/csp/processing/DataVersionConverters.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

/// @brief Common CSP Client
/// @details See documentation of CSP
/// @note IClientSpeaker must be valid all the time when Client is used
///     and behavior will be undefined otherwise.
///     Thread-safe after initialization as long as IClientSpeaker is thread-safe.
class Client
{
public:
    explicit Client(IClientSpeaker& clientSpeaker);
    Client(IClientSpeaker& clientSpeaker, const service_structs::CspPartySettings<>& settings);

    Status init(const service_structs::CspPartySettings<>& settings) noexcept;
    Status init(const service_structs::CspPartySettings<>& clientSettings, service_structs::CspPartySettings<>& serverSettings) noexcept;

    bool isValid() const noexcept;

    IClientSpeaker& getClientSpeaker() noexcept;

    /// @brief Shortcut to receive server supported CSP versions
    /// @param output Server supported CSP versions
    /// @return Status of operation
    Status getServerProtocolVersions(RawVectorT<protocol_version_t>& output) const noexcept;

    Status getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& serverSettings) const noexcept;

    /// @brief Get server handler minimum supported interface version and ID of its output type
    /// @param minimumInterfaceVersion Minimum supported interface version
    /// @param outputTypeId Handler output type
    /// @return Status of operation
    template<ISerializableBased InputType>
    Status getServerHandlerSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept;

    constexpr const service_structs::CspPartySettings<>& getSettings() const noexcept;

    constexpr interface_version_t getInterfaceVersion(const Id& id) const noexcept;

    /// @brief Send input data to server(s) and get output data on response
    /// @details See another handleData() overloading
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server 
    /// @param unmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<IClientDataHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, VectorT<GenericPointerKeeperT>* pUnmanagedPointers = nullptr);

    /// @brief Send input data to server(s) and get output data on response
    /// @details See another handleData() overloading
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server
    /// @param dataFlags Data flags that must be applied to current operation
    /// @param unmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<IClientDataHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::DataFlags dataFlags, VectorT<GenericPointerKeeperT>* pUnmanagedPointers = nullptr);

    /// @brief Send input data to server(s) and get output data on response
    /// @details Input data serialized according to arguments of function 
    ///     and predefined settings of Client object instance.
    ///     Then serialized data being sent to server(s) and received serialized response.
    ///     Response can be status of operation or expected output data.
    ///     - if response is a status that implying input message resend, but with adjusted
    ///         options that can be corrected automatically, input data repacking with it
    ///         and sends to server(s) again;
    ///     - if response is an error status that can't be handled automatically or
    ///         successed status, this status is returned;
    ///     - if response is expected output data, then it's being deserialized according
    ///         to arguments of function and predefined settings of Client object instance
    ///         and function will return status of operation.
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server 
    ///     (use ISerializableDummy<> if no output data is expected)
    /// @param additionalCommonFlags Common flags that must be applied to current operation
    /// @param additionalDataFlags Data flags that must be applied to current operation
    /// @param pUnmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<IClientDataHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::CommonFlags additionalCommonFlags
        , context::DataFlags additionalDataFlags, VectorT<GenericPointerKeeperT>* pUnmanagedPointers = nullptr);

private:
    service_structs::CspPartySettings<> m_settings;
    IClientSpeaker& m_clientSpeaker;
    bool m_isValid{ false };
};

inline Client::Client(IClientSpeaker& clientSpeaker)
    : m_clientSpeaker(clientSpeaker)
{
}

inline Client::Client(IClientSpeaker& clientSpeaker, const service_structs::CspPartySettings<>& settings)
    : m_clientSpeaker(clientSpeaker)
{
    init(settings);
}

inline Status Client::init(const service_structs::CspPartySettings<>& settings) noexcept
{
    if (isValid())
        return Status::ErrorAlreadyInited;

    if (!settings.isValid())
        return Status::ErrorInvalidArgument;

    m_settings.clear();

    CS_RUN(m_settings.init(settings));

    m_isValid = m_settings.isValid();

    return m_isValid ? Status::NoError : Status::ErrorNotInited;
}

inline Status Client::init(const service_structs::CspPartySettings<>& clientSettings, service_structs::CspPartySettings<>& serverSettings) noexcept
{
    if (isValid())
        return Status::ErrorAlreadyInited;

    if (!clientSettings.isValid())
        return Status::ErrorInvalidArgument;

    m_settings.clear();

    RawVectorT<protocol_version_t> serverCspVersions;
    CS_RUN(getServerProtocolVersions(serverCspVersions));

    protocol_version_t tempServerProtocolVersion = traits::kProtocolVersionUndefined;

    for (auto serverProtocolVersion : serverCspVersions)
    {
        for (auto clientProtocolVersion : clientSettings.getProtocolVersions())
        {
            if (clientProtocolVersion < serverProtocolVersion)
                break;
            else if (serverProtocolVersion == clientProtocolVersion)
            {
                tempServerProtocolVersion = serverProtocolVersion;
                break;
            }
        }

        if (tempServerProtocolVersion != traits::kProtocolVersionUndefined)
            break;
    }

    if (tempServerProtocolVersion == traits::kProtocolVersionUndefined)
    {
        CS_RUN(serverSettings.init(std::move(serverCspVersions), {}, {}, {}));
        return Status::ErrorNotSupportedProtocolVersion;
    }

    CS_RUN(getServerSettings(tempServerProtocolVersion, serverSettings));
    CS_RUN(m_settings.getCompatibleSettings(clientSettings, serverSettings));

    m_isValid = m_settings.isValid();

    return m_isValid ? Status::NoError : Status::ErrorNotInited;
}

CS_ALWAYS_INLINE bool Client::isValid() const noexcept
{
    return m_isValid;
}

CS_ALWAYS_INLINE IClientSpeaker& Client::getClientSpeaker() noexcept
{
    return m_clientSpeaker;
}

inline Status Client::getServerProtocolVersions(RawVectorT<protocol_version_t>& output) const noexcept
{
    BinVectorT binInput;
    context::SCommon ctxIn(binInput, traits::kProtocolVersionUndefined);
    CS_RUN(processing::serializeCommonContextNoChecks(ctxIn));

    BinWalkerT binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::DCommon ctxOut(binOutput);
    CS_RUN(processing::deserializeCommonContextNoChecks(ctxOut));

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::ErrorDataCorrupted;

    Status statusOut = Status::NoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::ErrorNotSupportedProtocolVersion)
        return Status::ErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedProtocolVersionBody(ctxOut, output);
}

inline Status Client::getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& cspPartySettings) const noexcept
{
    BinVectorT binInput;
    context::SCommon ctxIn(binInput, serverCspVersion, context::Message::GetSettings, {});
    CS_RUN(processing::serializeCommonContext(ctxIn));

    BinWalkerT binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    return cspPartySettings.deserialize(binOutput);
}

template<ISerializableBased InputType>
Status Client::getServerHandlerSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept
{
    if (!isValid())
        return Status::ErrorNotInited;

    const Interface& interface_ = InputType::getInterface();

    if (getInterfaceVersion(interface_.id) == traits::kInterfaceVersionUndefined)
        return Status::ErrorNotSupportedInterface;

    BinVectorT binInput;
    context::SData ctxIn(binInput, m_settings.getLatestProtocolVersion(), m_settings.getMandatoryCommonFlags());

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContextNoChecks<InputType>(ctxIn));

    BinWalkerT binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::DCommon ctxOut(binOutput);

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::ErrorDataCorrupted;

    Status statusOut = Status::NoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::ErrorNotSupportedInterfaceVersion)
        return Status::ErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedInterfaceVersionBody(ctxOut, minimumInterfaceVersion, outputTypeId);
}

constexpr const service_structs::CspPartySettings<>& Client::getSettings() const noexcept
{
    return m_settings;
}

constexpr interface_version_t Client::getInterfaceVersion(const Id& id) const noexcept
{
    for (const auto& interface_ : m_settings.getInterfaces())
        if (id == interface_.id)
            return interface_.version;

    return traits::kInterfaceVersionUndefined;
}

template<IClientDataHandlerTraits _Cht>
Status Client::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, VectorT<GenericPointerKeeperT>* pUnmanagedPointers)
{
    return handleData<_Cht>(input, output, {}, {}, pUnmanagedPointers);
}

template<IClientDataHandlerTraits _Cht>
Status Client::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::DataFlags additionalDataFlags
    , VectorT<GenericPointerKeeperT>* pUnmanagedPointers)
{
    return handleData<_Cht>(input, output, {}, additionalDataFlags, pUnmanagedPointers);
}

template<IClientDataHandlerTraits _Cht>
Status Client::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::CommonFlags additionalCommonFlags
    , context::DataFlags additionalDataFlags, VectorT<GenericPointerKeeperT>* pUnmanagedPointers)
{
    using InputType = typename _Cht::InputType;
    using OutputType = typename _Cht::OutputType;
    constexpr bool kForTempUseHeap = _Cht::kForTempUseHeap;

    if (!isValid())
        return Status::ErrorNotInited;

    const Interface& interface_ = InputType::getInterface();

    interface_version_t interfaceVersionToUse = getInterfaceVersion(interface_.id);

    if (interfaceVersionToUse == traits::kInterfaceVersionUndefined)
        return Status::ErrorNotSupportedInterface;

    if (InputType::getOriginPrivateVersion() > interfaceVersionToUse || OutputType::getOriginPrivateVersion() > interfaceVersionToUse)
        return Status::ErrorNotSupportedInterfaceVersion;

    if (additionalCommonFlags & m_settings.getForbiddenCommonFlags())
        return Status::ErrorNotCompatibleCommonFlagsSettings;

    if (additionalDataFlags & interface_.forbiddenDataFlags)
        return Status::ErrorNotCompatibleDataFlagsSettings;

    BinVectorT binInput;

    context::SData ctxIn(
          binInput
        , m_settings.getLatestProtocolVersion()
        , m_settings.getMandatoryCommonFlags() | additionalCommonFlags
        , InputType::getEffectiveMandatoryDataFlags() | additionalDataFlags
        , kForTempUseHeap
        , interfaceVersionToUse
        , nullptr);

    context::SPointersMap pointersMapIn;
    if (ctxIn.checkRecursivePointers())
        ctxIn.setPointersMap(&pointersMapIn);

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.allowUnmanagedPointers() && pUnmanagedPointers == nullptr)
        return Status::ErrorInvalidArgument;

    CS_RUN(processing::data::BodyProcessor::serialize(input, ctxIn));

    pointersMapIn.clear();

    BinWalkerT binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    ctxIn.clear();

    context::DCommon ctxOut(binOutput);

    CS_RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxIn.getCommonFlags() != ctxOut.getCommonFlags())
        return Status::ErrorDataCorrupted;

    if (ctxOut.getMessageType() == context::Message::Data)
    {
        ctxIn.clear();

        Id outId;
        context::DData ctxOutData(ctxOut);

        CS_RUN(processing::deserializeDataContext(ctxOutData, outId));

        context::DataFlags outDataFlags = ctxOutData.getDataFlags();

        if (Status status = processing::testDataFlagsCompatibility<OutputType>(outDataFlags); !statusSuccess(status))
            return status == Status::ErrorNotCompatibleDataFlagsSettings ? Status::ErrorDataCorrupted : status;

        ctxOutData.setAddedPointers(pUnmanagedPointers);

        context::DPointersMap pointersMapOut;
        if (ctxOutData.checkRecursivePointers())
            ctxOutData.setPointersMap(&pointersMapOut);

        CS_RUN(processing::deserializeDataContextPostprocessId<OutputType>(outId));
        // Here we no need to check minimum value of output version, because if we run Client::handleData
        // we're already agreed with interface version that server have
        CS_RUN(processing::deserializeDataContextPostprocessRest<OutputType>(ctxOutData, OutputType::getOriginPrivateVersion()));

        ctxOutData.setHeapUseForTemp(kForTempUseHeap);

        CS_RUN(processing::data::BodyProcessor::deserialize(ctxOutData, output));
    }
    else if (ctxOut.getMessageType() == context::Message::Status)
    {
        Status statusOut = Status::NoError;
        CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

        return statusOut;
    }

    return Status::NoError;
}

} // namespace common_serialization::csp::messaging
