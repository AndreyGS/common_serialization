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
#include "common_serialization/csp/messaging/IClientSpeaker.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataBodyProcessor.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of client in CSP messaging model
/// @details See documentation of CSP
/// @note IClientSpeaker must be valid all the time when Client is used
///     and behavior will be undefined otherwise
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
    Status getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept;

    Status getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& serverSettings) const noexcept;

    /// @brief Get server handler minimum supported interface version and ID of its output type
    /// @tparam InputType Handler input struct type
    /// @param minimumInterfaceVersion Minimum supported interface version
    /// @param outputTypeId Handler output type
    /// @return Status of operation
    template<typename InputType>
        requires IsISerializableBased<InputType>
    Status getServerHandlerSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept;

    constexpr const service_structs::CspPartySettings<>& getSettings() const noexcept;

    constexpr interface_version_t getInterfaceVersion(const Id& id) const noexcept;

    /// @brief Send input data to server(s) and get output data on response
    /// @details See another handleData() overloading
    /// @tparam InputType Type that implements ISerializable interface
    /// @tparam OutputType Type that implements ISerializable interface
    /// @tparam forTempUseHeap Should heap be used in large memory consumption operations
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server 
    /// @param unmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr);

    /// @brief Send input data to server(s) and get output data on response
    /// @details See another handleData() overloading
    /// @tparam InputType Type that implements ISerializable interface
    /// @tparam OutputType Type that implements ISerializable interface
    /// @tparam forTempUseHeap Should heap be used in large memory consumption operations
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server
    /// @param dataFlags Data flags that must be applied to current operation
    /// @param unmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(const InputType& input, OutputType& output, context::DataFlags dataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr);

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
    /// @tparam InputType Type that implements ISerializable interface
    /// @tparam OutputType Type that implements ISerializable interface
    /// @tparam forTempUseHeap Should heap be used in large memory consumption operations.
    ///     Using stack can be dangerous if our environment have small stack (kernels)
    ///     or for huge input or output structures, when versions convertations can be applied.
    /// @param input Struct that must be sent to server
    /// @param output Struct that is returned from server 
    ///     (use ISerializableDummy<> if no output data is expected)
    /// @param additionalCommonFlags Common flags that must be applied to current operation
    /// @param additionalDataFlags Data flags that must be applied to current operation
    /// @param pUnmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , context::CommonFlags additionalCommonFlags
        , context::DataFlags additionalDataFlags
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
    );

private:
    IClientSpeaker& m_clientSpeaker;
    service_structs::CspPartySettings<> m_settings;

    // Distinct variable is for not having calculate valid condition every time
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
    if (!settings.isValid())
        return Status::kErrorInvalidArgument;

    m_isValid = false;

    m_settings.clear();

    CS_RUN(m_settings.init(settings));

    m_isValid = m_clientSpeaker.isValid();

    return m_isValid ? Status::kNoError : Status::kErrorNotInited;
}

inline Status Client::init(const service_structs::CspPartySettings<>& clientSettings, service_structs::CspPartySettings<>& serverSettings) noexcept
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

inline bool Client::isValid() const noexcept
{
    return m_isValid;
}

inline IClientSpeaker& Client::getClientSpeaker() noexcept
{
    return m_clientSpeaker;
}

inline Status Client::getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept
{
    if (!m_clientSpeaker.isValid())
        return Status::kErrorNotInited;

    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, traits::kProtocolVersionUndefined);
    CS_RUN(processing::serializeCommonContextNoChecks(ctxIn));

    BinWalker binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::Common<BinWalker> ctxOut(binOutput);
    CS_RUN(processing::deserializeCommonContextNoChecks(ctxOut));

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::kErrorNotSupportedProtocolVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedProtocolVersionBody(ctxOut, output);
}

inline Status Client::getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& cspPartySettings) const noexcept
{
    if (!m_clientSpeaker.isValid())
        return Status::kErrorNotInited;

    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, serverCspVersion, context::Message::GetSettings, {});
    CS_RUN(processing::serializeCommonContext(ctxIn));

    BinWalker binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    return cspPartySettings.deserialize(binOutput);
}

template<typename InputType>
    requires IsISerializableBased<InputType>
Status Client::getServerHandlerSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept
{
    if (!isValid())
        return Status::kErrorNotInited;

    const Interface& interface_ = InputType::getInterface();

    if (getInterfaceVersion(interface_.id) == traits::kInterfaceVersionUndefined)
        return Status::kErrorNotSupportedInterface;

    BinVector binInput;
    context::SData<> ctxIn(binInput, m_settings.protocolVersions[0], m_settings.mandatoryCommonFlags);

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContextNoChecks<InputType>(ctxIn));

    BinWalker binOutput;
    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    context::Common<BinWalker> ctxOut(binOutput);

    if (ctxOut.getMessageType() != context::Message::Status)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    CS_RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::kErrorNotSupportedInterfaceVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedInterfaceVersionBody(ctxOut, minimumInterfaceVersion, outputTypeId);
}

constexpr const service_structs::CspPartySettings<>& Client::getSettings() const noexcept
{
    return m_settings;
}

constexpr interface_version_t Client::getInterfaceVersion(const Id& id) const noexcept
{
    for (const auto& interface_ : m_settings.interfaces)
        if (id == interface_.id)
            return interface_.version;

    return traits::kInterfaceVersionUndefined;
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status Client::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    return handleData(input, output, {}, {}, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status Client::handleData(const InputType& input, OutputType& output, context::DataFlags additionalDataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    return handleData(input, output, {}, additionalDataFlags, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status Client::handleData(const InputType& input, OutputType& output, context::CommonFlags additionalCommonFlags
    , context::DataFlags additionalDataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
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

    BinVector binInput;

    context::SData<> ctxIn(
          binInput
        , m_settings.protocolVersions[0]
        , m_settings.mandatoryCommonFlags | additionalCommonFlags
        , InputType::getEffectiveMandatoryDataFlags() | additionalDataFlags
        , forTempUseHeap
        , interfaceVersionToUse
        , nullptr);

    std::unordered_map<const void*, uint64_t> pointersMapIn;
    if (ctxIn.checkRecursivePointers())
        ctxIn.setPointersMap(&pointersMapIn);

    CS_RUN(processing::serializeCommonContext(ctxIn));
    CS_RUN(processing::serializeDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.allowUnmanagedPointers() && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    CS_RUN(processing::data::BodyProcessor::serialize(input, ctxIn));

    pointersMapIn.clear();

    BinWalker binOutput;

    CS_RUN(m_clientSpeaker.speak(binInput, binOutput));

    ctxIn.clear();

    context::Common<BinWalker> ctxOut(binOutput);

    CS_RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxIn.getCommonFlags() != ctxOut.getCommonFlags())
        return Status::kErrorDataCorrupted;

    if (ctxOut.getMessageType() == context::Message::Data)
    {
        ctxIn.clear();

        Id outId;
        context::DData<> ctxOutData(ctxOut);

        CS_RUN(processing::deserializeDataContext(ctxOutData, outId));

        context::DataFlags outDataFlags = ctxOutData.getDataFlags();

        if (Status status = processing::testDataFlagsCompatibility<OutputType>(outDataFlags); !statusSuccess(status))
            return status == Status::kErrorNotCompatibleDataFlagsSettings ? Status::kErrorDataCorrupted : status;

        ctxOutData.setAddedPointers(pUnmanagedPointers);

        std::unordered_map<uint64_t, void*> pointersMapOut;
        if (ctxOutData.checkRecursivePointers())
            ctxOutData.setPointersMap(&pointersMapOut);

        // Here we no need to check minimum value of output version, because if we run Client::handleData
        // we're already agreed with interface version that server have
        CS_RUN(processing::deserializeDataContextPostprocess<OutputType>(ctxOutData, outId, OutputType::getOriginPrivateVersion()));

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
