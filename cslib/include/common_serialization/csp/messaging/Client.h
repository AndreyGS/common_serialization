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
#include "common_serialization/Containers/UniquePtr.h"
#include "common_serialization/csp/messaging/ClientHandlerTraits.h"
#include "common_serialization/csp/messaging/IClientSpeaker.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataBodyProcessor.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

/// @brief Common CSP Client
/// @details See documentation of CSP
/// @note IClientSpeaker must be valid all the time when Client is used
///     and behavior will be undefined otherwise'
template<SdContainers _Sdcs = traits::DefaultSdContainers>
class Client
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

    explicit Client(IClientSpeaker<Sbin, Dbin>& clientSpeaker);
    Client(IClientSpeaker<Sbin, Dbin>& clientSpeaker, const service_structs::CspPartySettings<>& settings);

    Status init(const service_structs::CspPartySettings<>& settings) noexcept;
    Status init(const service_structs::CspPartySettings<>& clientSettings, service_structs::CspPartySettings<>& serverSettings) noexcept;

    bool isValid() const noexcept;

    IClientSpeaker<Sbin, Dbin>& getClientSpeaker() noexcept;

    /// @brief Shortcut to receive server supported CSP versions
    /// @param output Server supported CSP versions
    /// @return Status of operation
    Status getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept;

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
    template<ClientHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, Gkc* pUnmanagedPointers = nullptr);

    /// @brief Send input data to server(s) and get output data on response
    /// @details See another handleData() overloading
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server
    /// @param dataFlags Data flags that must be applied to current operation
    /// @param unmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<ClientHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::DataFlags dataFlags, Gkc* pUnmanagedPointers = nullptr);

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
    template<ClientHandlerTraits _Cht>
    Status handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::CommonFlags additionalCommonFlags
        , context::DataFlags additionalDataFlags, Gkc* pUnmanagedPointers = nullptr);

private:
    IClientSpeaker<Sbin, Dbin>& m_clientSpeaker;
    service_structs::CspPartySettings<> m_settings;

    // Distinct variable is for not having calculate valid condition every time
    bool m_isValid{ false };
};

template<SdContainers _Sdcs>
Client<_Sdcs>::Client(IClientSpeaker<Sbin, Dbin>& clientSpeaker)
    : m_clientSpeaker(clientSpeaker)
{
}

template<SdContainers _Sdcs>
Client<_Sdcs>::Client(IClientSpeaker<Sbin, Dbin>& clientSpeaker, const service_structs::CspPartySettings<>& settings)
    : m_clientSpeaker(clientSpeaker)
{
    init(settings);
}

template<SdContainers _Sdcs>
Status Client<_Sdcs>::init(const service_structs::CspPartySettings<>& settings) noexcept
{
    if (!settings.isValid())
        return Status::kErrorInvalidArgument;

    m_isValid = false;

    m_settings.clear();

    CS_RUN(m_settings.init(settings));

    m_isValid = m_clientSpeaker.isValid();

    return m_isValid ? Status::kNoError : Status::kErrorNotInited;
}

template<SdContainers _Sdcs>
Status Client<_Sdcs>::init(const service_structs::CspPartySettings<>& clientSettings, service_structs::CspPartySettings<>& serverSettings) noexcept
{
    m_isValid = false;

    m_settings.clear();

    Vector<protocol_version_t> serverCspVersions;
    CS_RUN(getServerProtocolVersions(serverCspVersions));

    protocol_version_t tempServerProtocolVersion = traits::kProtocolVersionUndefined;

    for (auto serverProtocolVersion : serverCspVersions)
    {
        for (auto clientProtocolVersion : clientSettings.protocolVersions)
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
        CS_RUN(serverSettings.protocolVersions.init(std::move(serverCspVersions)));
        return Status::kErrorNotSupportedProtocolVersion;
    }

    CS_RUN(getServerSettings(tempServerProtocolVersion, serverSettings));
    CS_RUN(m_settings.getCompatibleSettings(clientSettings, serverSettings));

    m_isValid = m_settings.isValid() && m_clientSpeaker.isValid();

    return m_isValid ? Status::kNoError : Status::kErrorNotInited;
}

template<SdContainers _Sdcs>
bool Client<_Sdcs>::isValid() const noexcept
{
    return m_isValid;
}

template<SdContainers _Sdcs>
IClientSpeaker<typename _Sdcs::Sbin, typename  _Sdcs::Dbin>& Client<_Sdcs>::getClientSpeaker() noexcept
{
    return m_clientSpeaker;
}

template<SdContainers _Sdcs>
Status Client<_Sdcs>::getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept
{
    if (!m_clientSpeaker.isValid())
        return Status::kErrorNotInited;

    Sbin binInput;
    context::Common ctxIn(binInput, traits::kProtocolVersionUndefined);
    CS_RUN(processing::serializeCommonContextNoChecks(ctxIn));

    Dbin binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::Common ctxOut(binOutput);
    CS_RUN(processing::deserializeCommonContextNoChecks(ctxOut));

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::kErrorNotSupportedProtocolVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedProtocolVersionBody(ctxOut, output);
}

template<SdContainers _Sdcs>
Status Client<_Sdcs>::getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& cspPartySettings) const noexcept
{
    if (!m_clientSpeaker.isValid())
        return Status::kErrorNotInited;

    Sbin binInput;
    context::Common ctxIn(binInput, serverCspVersion, context::Message::GetSettings, {});
    CS_RUN(processing::serializeCommonContext(ctxIn));

    Dbin binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    return cspPartySettings.deserialize(binOutput);
}

template<SdContainers _Sdcs>
template<ISerializableBased InputType>
Status Client<_Sdcs>::getServerHandlerSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept
{
    if (!isValid())
        return Status::kErrorNotInited;

    const Interface& interface_ = InputType::getInterface();

    if (getInterfaceVersion(interface_.id) == traits::kInterfaceVersionUndefined)
        return Status::kErrorNotSupportedInterface;

    Sbin binInput;
    context::Data<Scs> ctxIn(binInput, m_settings.protocolVersions[0], m_settings.mandatoryCommonFlags);

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContextNoChecks<InputType>(ctxIn));

    Dbin binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::Common ctxOut(binOutput);

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::kErrorNotSupportedInterfaceVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedInterfaceVersionBody(ctxOut, minimumInterfaceVersion, outputTypeId);
}

template<SdContainers _Sdcs>
constexpr const service_structs::CspPartySettings<>& Client<_Sdcs>::getSettings() const noexcept
{
    return m_settings;
}

template<SdContainers _Sdcs>
constexpr interface_version_t Client<_Sdcs>::getInterfaceVersion(const Id& id) const noexcept
{
    for (const auto& interface_ : m_settings.interfaces)
        if (id == interface_.id)
            return interface_.version;

    return traits::kInterfaceVersionUndefined;
}

template<SdContainers _Sdcs>
template<ClientHandlerTraits _Cht>
Status Client<_Sdcs>::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, Gkc* pUnmanagedPointers)
{
    return handleData<_Cht>(input, output, {}, {}, pUnmanagedPointers);
}

template<SdContainers _Sdcs>
template<ClientHandlerTraits _Cht>
Status Client<_Sdcs>::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::DataFlags additionalDataFlags, Gkc* pUnmanagedPointers)
{
    return handleData<_Cht>(input, output, {}, additionalDataFlags, pUnmanagedPointers);
}

template<SdContainers _Sdcs>
template<ClientHandlerTraits _Cht>
Status Client<_Sdcs>::handleData(const typename _Cht::InputType& input, typename _Cht::OutputType& output, context::CommonFlags additionalCommonFlags
    , context::DataFlags additionalDataFlags, Gkc* pUnmanagedPointers)
{
    using InputType = typename _Cht::InputType;
    using OutputType = typename _Cht::OutputType;
    constexpr bool kForTempUseHeap = _Cht::kForTempUseHeap;

    // We are additionally checking of m_clientSpeaker ptr state
    // because Client interface allows us to manipulate with it directly
    if (!isValid())
        return Status::kErrorNotInited;

    const Interface& interface_ = InputType::getInterface();

    interface_version_t interfaceVersionToUse = getInterfaceVersion(interface_.id);

    if (interfaceVersionToUse == traits::kInterfaceVersionUndefined)
        return Status::kErrorNotSupportedInterface;

    if (InputType::getOriginPrivateVersion() > interfaceVersionToUse || OutputType::getOriginPrivateVersion() > interfaceVersionToUse)
        return Status::kErrorNotSupportedInterfaceVersion;

    if (additionalCommonFlags & m_settings.forbiddenCommonFlags)
        return Status::kErrorNotCompatibleCommonFlagsSettings;

    if (additionalDataFlags & interface_.forbiddenDataFlags)
        return Status::kErrorNotCompatibleDataFlagsSettings;

    Sbin binInput;

    context::Data<Scs> ctxIn(
          binInput
        , m_settings.protocolVersions[0]
        , m_settings.mandatoryCommonFlags | additionalCommonFlags
        , InputType::getEffectiveMandatoryDataFlags() | additionalDataFlags
        , kForTempUseHeap
        , interfaceVersionToUse
        , nullptr);

    Spm pointersMapIn;
    if (ctxIn.checkRecursivePointers())
        ctxIn.setPointersMap(&pointersMapIn);

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.allowUnmanagedPointers() && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    CS_RUN(processing::data::BodyProcessor::serialize(input, ctxIn));

    pointersMapIn.clear();

    Dbin binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    ctxIn.clear();

    context::Common ctxOut(binOutput);

    CS_RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxIn.getCommonFlags() != ctxOut.getCommonFlags())
        return Status::kErrorDataCorrupted;

    if (ctxOut.getMessageType() == context::Message::Data)
    {
        ctxIn.clear();

        Id outId;
        context::Data<Dcs> ctxOutData(ctxOut);

        CS_RUN(processing::deserializeDataContext(ctxOutData, outId));

        context::DataFlags outDataFlags = ctxOutData.getDataFlags();

        if (Status status = processing::testDataFlagsCompatibility<OutputType>(outDataFlags); !statusSuccess(status))
            return status == Status::kErrorNotCompatibleDataFlagsSettings ? Status::kErrorDataCorrupted : status;

        ctxOutData.setAddedPointers(pUnmanagedPointers);

        Dpm pointersMapOut;
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
        Status statusOut = Status::kNoError;
        CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

        return statusOut;
    }

    return Status::kNoError;
}

} // namespace common_serialization::csp::messaging
