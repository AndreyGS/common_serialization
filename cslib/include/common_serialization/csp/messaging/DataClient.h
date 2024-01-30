/**
 * @file cslib/include/common_serialization/csp/messaging/DataClient.h
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
#include "common_serialization/csp/messaging/IDataClientSpeaker.h"
#include "common_serialization/csp/processing/CommonCapabilities.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataProcessor.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of client in CSP messaging model
/// @details See documentation of CSP
/// @note This class itself contains defined methods of handling
///     data in CSP messaging model.
class DataClient
{
public:
    DataClient(IDataClientSpeaker* pDataClientSpeaker)
        : m_dataClientSpeaker(pDataClientSpeaker)
    {
    }

    DataClient(
          IDataClientSpeaker* pDataClientSpeaker
        , protocol_version_t protocolVersion
        , context::CommonFlags mandatoryCommonFlags
        , context::CommonFlags forbiddenCommonFlags
        , const Vector<traits::Interface>& interfaces
    )
        : m_dataClientSpeaker(pDataClientSpeaker)
        , m_protocolVersion(protocolVersion)
        , m_mandatoryCommonFlags(mandatoryCommonFlags)
        , m_forbiddenCommonFlags(forbiddenCommonFlags)
        , m_interfaces(interfaces)
    {
        m_isReady = static_cast<bool>(pDataClientSpeaker)
            && m_protocolVersion != traits::kProtocolVersionUndefined
            && !static_cast<bool>(m_mandatoryCommonFlags & m_forbiddenCommonFlags)
            && m_interfaces.size() > 0;
    }

    Status init(
          const Vector<protocol_version_t>& acceptableCspVersions
        , context::CommonFlags clientMandatoryCommonFlags
        , context::CommonFlags clientForbiddenCommonFlags
        , const Vector<traits::Interface>& acceptableInterfaces
        , service_structs::CspPartySettings<>* pCspPartySettings) noexcept;

    Status getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& cspPartySettings) const noexcept;

    bool isReady() const noexcept
    {
        return m_isReady;
    }

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
    ///     and predefined settings of DataClient object instance.
    ///     Then serialized data being sent to server(s) and received serialized response.
    ///     Response can be status of operation or expected output data.
    ///     - if response is a status that implying input message resend, but with adjusted
    ///         options that can be corrected automatically, input data repacking with it
    ///         and sends to server(s) again;
    ///     - if response is an error status that can't be handled automatically or
    ///         successed status, this status is returned;
    ///     - if response is expected output data, then it's being deserialized according
    ///         to arguments of function and predefined settings of DataClient object instance
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

    /// @brief Shortcut to receive server supported CSP versions
    /// @param output Server supported CSP versions
    /// @return Status of operation
    Status getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept;

    template<typename InputType>
        requires IsISerializableBased<InputType>
    Status getServerStructInterfaceSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept;

    constexpr const Vector<traits::Interface>& getInterfaces() const noexcept
    {
        return m_interfaces;
    }

    const traits::Interface* getInterface(const Id& id) const noexcept
    {
        for (const auto& _interface : getInterfaces())
            if (id == _interface.id)
                return &_interface;

        return nullptr;
    }


private:
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , const traits::Interface& _interface
        , context::CommonFlags additionalCommonFlags
        , context::DataFlags additionalDataFlags
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
    );

    bool m_isReady{ false };

    protocol_version_t m_protocolVersion{ traits::getLatestProtocolVersion() };

    context::CommonFlags m_mandatoryCommonFlags;
    context::CommonFlags m_forbiddenCommonFlags;

    Vector<traits::Interface> m_interfaces;

    UniquePtr<IDataClientSpeaker> m_dataClientSpeaker;
};

Status DataClient::init(
      const Vector<protocol_version_t>& acceptableCspVersions
    , context::CommonFlags clientMandatoryCommonFlags
    , context::CommonFlags clientForbiddenCommonFlags
    , const Vector<traits::Interface>& acceptableInterfaces
    , service_structs::CspPartySettings<>* pCspPartySettings) noexcept
{
    if (!m_dataClientSpeaker)
        return Status::kErrorNotInited;

    Vector<protocol_version_t> serverCspVersions;
    RUN(getServerProtocolVersions(serverCspVersions));

    protocol_version_t tempServerProtocolVersion = traits::kProtocolVersionUndefined;

    for (auto serverProtocolVersion : serverCspVersions)
    {
        for (auto clientProtocolVersion : acceptableCspVersions)
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
        if (pCspPartySettings)
            RUN(pCspPartySettings->supportedCspVersions.init(std::move(serverCspVersions)));

        return Status::kErrorNotSupportedProtocolVersion;
    }

    service_structs::CspPartySettings<> cspPartySettings;
    RUN(getServerSettings(serverCspVersions[0], cspPartySettings));

    if (pCspPartySettings)
        RUN(pCspPartySettings->init(std::move(cspPartySettings)));

    service_structs::CspPartySettings<>& cspPartySettingsR = pCspPartySettings ? *pCspPartySettings : cspPartySettings;

    context::CommonFlags tempMandatoryCommonFlags = clientMandatoryCommonFlags | cspPartySettingsR.mandatoryCommonFlags;
    context::CommonFlags tempForbiddenCommonFlags = clientForbiddenCommonFlags | cspPartySettingsR.forbiddenCommonFlags;

    if (tempMandatoryCommonFlags & tempForbiddenCommonFlags)
        return Status::kErrorNotCompatibleCommonFlagsSettings;

    for (auto serverInterfaceVersion : cspPartySettingsR.interfaces)
        for (auto clientInterface : acceptableInterfaces)
        {
            if (serverInterfaceVersion.id == clientInterface.id)
            {
                RUN(m_interfaces.pushBack(clientInterface));
                m_interfaces[m_interfaces.size() - 1].version = serverInterfaceVersion.version > clientInterface.version ? clientInterface.version : serverInterfaceVersion.version;
                break;
            }
        }

    if (m_interfaces.size() == 0)
        return Status::kErrorNoSupportedInterfaces;

    // if we are here, than everything is ok and we should init DataClient member fields
    m_protocolVersion = tempServerProtocolVersion;
    m_mandatoryCommonFlags = tempMandatoryCommonFlags;
    m_forbiddenCommonFlags = tempForbiddenCommonFlags;
    
    m_isReady = true;

    return Status::kNoError;
}

Status DataClient::getServerSettings(protocol_version_t serverCspVersion, service_structs::CspPartySettings<>& cspPartySettings) const noexcept
{
    if (!m_dataClientSpeaker)
        return Status::kErrorNotInited;

    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, serverCspVersion);
    RUN(processing::serializeCommonContext(ctxIn));

    BinWalker binOutput;

    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    return cspPartySettings.deserialize(binOutput);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    if (!isReady())
        return Status::kErrorNotInited;

    traits::Interface* pInterface = getInterface(InputType::getInterface().id);
    if (!pInterface)
        return Status::kErrorNotSupportedInterface;

    return handleData(input, output, *pInterface, context::CommonFlags{}, context::DataFlags{}, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, context::DataFlags additionalDataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    if (!isReady())
        return Status::kErrorNotInited;

    traits::Interface* pInterface = getInterface(InputType::getInterface().id);
    if (!pInterface)
        return Status::kErrorNotSupportedInterface;

    return handleData(input, output, *pInterface, context::CommonFlags{}, additionalDataFlags, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, context::CommonFlags additionalCommonFlags
    , context::DataFlags additionalDataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    if (!isReady())
        return Status::kErrorNotInited;

    traits::Interface* pInterface = getInterface(InputType::getInterface().id);
    if (!pInterface)
        return Status::kErrorNotSupportedInterface;

    return handleData(input, output, *pInterface, additionalCommonFlags, additionalDataFlags, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, const traits::Interface& _interface, context::CommonFlags additionalCommonFlags
    , context::DataFlags additionalDataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    if (InputType::getOriginPrivateVersion() > _interface.version || OutputType::getOriginPrivateVersion() > _interface.version)
        return Status::kErrorNotSupportedInterfaceVersion;

    if (additionalCommonFlags & m_forbiddenCommonFlags)
        return Status::kErrorNotCompatibleCommonFlagsSettings;

    if (additionalDataFlags & _interface.forbiddenDataFlags)
        return Status::kErrorNotCompatibleDataFlagsSettings;

    BinVector binInput;

    context::SData<> ctxIn(
          binInput
        , m_protocolVersion
        , m_mandatoryCommonFlags | additionalCommonFlags
        , _interface.mandatoryDataFlags | InputType::getAddtionalMandatoryDataFlags() | additionalDataFlags
        , forTempUseHeap
        , _interface.version
        , nullptr);

    std::unordered_map<const void*, uint64_t> pointersMapIn;
    if (ctxIn.getDataFlags().checkRecursivePointers())
        ctxIn.setPointersMap(&pointersMapIn);

    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.getDataFlags().allowUnmanagedPointers() && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    RUN(processing::DataProcessor::serializeData(input, ctxIn));

    pointersMapIn.clear();

    BinWalker binOutput;

    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    ctxIn.clear();

    context::Common<BinWalker> ctxOut(binOutput);

    RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxIn.getCommonFlags() != ctxOut.getCommonFlags())
        return Status::kErrorDataCorrupted;

    if (ctxOut.getMessageType() == context::Message::kData)
    {
        ctxIn.clear();

        Id outId;
        context::DData<> ctxOutData(ctxOut);

        RUN(processing::deserializeDataContext(ctxOutData, outId));

        context::DataFlags outDataFlags = ctxOutData.getDataFlags();

        if (   outDataFlags & OutputType::getEffectiveForbiddenDataFlags()
            || outDataFlags & _interface.forbiddenDataFlags
            || (outDataFlags & OutputType::getEffectiveMandatoryDataFlags()) != OutputType::getEffectiveMandatoryDataFlags()
            || (outDataFlags & _interface.mandatoryDataFlags) != _interface.mandatoryDataFlags
        )
            return Status::kErrorDataCorrupted;

        ctxOutData.setAddedPointers(pUnmanagedPointers);

        std::unordered_map<uint64_t, void*> pointersMapOut;
        if (outDataFlags.checkRecursivePointers())
            ctxOutData.setPointersMap(&pointersMapOut);

        // Here we no need to check minimum value of output version, because if we run DataClient::handleData
        // we're already agreed with interface version that server have
        RUN(processing::deserializeDataContextPostprocess<OutputType>(ctxOutData, outId, OutputType::getOriginPrivateVersion()));

        RUN(processing::DataProcessor::deserializeData(ctxOutData, output));
    }
    else if (ctxOut.getMessageType() == context::Message::kStatus)
    {
        Status statusOut = Status::kNoError;
        RUN(processing::deserializeStatusContext(ctxOut, statusOut));

        return statusOut;
    }

    return Status::kNoError;
}

inline Status DataClient::getServerProtocolVersions(Vector<protocol_version_t>& output) const noexcept
{
    if (!m_dataClientSpeaker)
        return Status::kErrorNotInited;

    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, traits::kProtocolVersionUndefined);
    RUN(processing::serializeCommonContext(ctxIn));

    BinWalker binOutput;
    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    context::Common<BinWalker> ctxOut(binOutput);
    RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxOut.getMessageType() != context::Message::kStatus)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    RUN(processing::deserializeStatusContext(ctxOut, statusOut));
    
    if (statusOut != Status::kErrorNotSupportedProtocolVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedProtocolVersionBody(ctxOut, output);
}

template<typename InputType>
    requires IsISerializableBased<InputType>
Status DataClient::getServerStructInterfaceSettings(interface_version_t& minimumInterfaceVersion, Id& outputTypeId) const noexcept
{
    if (!isReady())
        return Status::kErrorNotInited;

    traits::Interface* pInterface = getInterface(InputType::getInterface().id);
    if (!pInterface)
        return Status::kErrorNotSupportedInterface;

    BinVector binInput;
    context::SData<> ctxIn(binInput, m_protocolVersion, m_mandatoryCommonFlags, pInterface->mandatoryDataFlags);
    
    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeDataContextNoChecks<InputType>(ctxIn));

    BinWalker binOutput;
    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    context::Common<BinWalker> ctxOut(binOutput);

    if (ctxOut.getMessageType() != context::Message::kStatus)
        return Status::kErrorDataCorrupted;

    Status statusOut = Status::kNoError;
    RUN(processing::deserializeStatusContext(ctxOut, statusOut));

    if (statusOut != Status::kErrorNotSupportedInterfaceVersion)
        return Status::kErrorDataCorrupted;

    return processing::deserializeStatusErrorNotSupportedInterfaceVersionBody(ctxOut, minimumInterfaceVersion, outputTypeId);
}

} // namespace common_serialization::csp::messaging
