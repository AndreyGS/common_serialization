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
    DataClient(IDataClientSpeaker* pDataClientSpeaker, traits::Interface _interface = service_structs::properties)
        : m_dataClientSpeaker(pDataClientSpeaker), m_interface(_interface)
    {
    }

    DataClient(
          IDataClientSpeaker* pDataClientSpeaker
        , traits::Interface _interface
        , protocol_version_t protocolVersion
        , context::CommonFlags mandatoryCommonFlags
        , context::CommonFlags forbiddenCommonFlags
        , context::DataFlags mandatoryDataFlags
        , context::DataFlags forbiddenDataFlags
    )
        : m_dataClientSpeaker(pDataClientSpeaker)
        , m_interface(_interface)
        , m_protocolVersion(protocolVersion)
        , m_mandatoryCommonFlags(mandatoryCommonFlags)
        , m_forbiddenCommonFlags(forbiddenCommonFlags)
        , m_mandatoryDataFlags(mandatoryDataFlags)
        , m_forbiddenDataFlags(forbiddenDataFlags)
    {
        m_isReady = static_cast<bool>(pDataClientSpeaker);
    }

    Status init(service_structs::CspPartySettings<>* pCspPartySettings)
    {
        service_structs::CspPartySettings<> cspPartySettings;
        Status status = handleData(service_structs::ISerializableDummy<>(), cspPartySettings);
    }

    bool isReady()
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
    /// @param dataFlags Data flags that must be applied to current operation
    /// @param commonFlags Common flags that must be applied to current operation
    /// @param pUnmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , context::DataFlags dataFlags
        , context::CommonFlags commonFlags
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
    );

    /// @brief Shortcut to receive server supported CSP versions
    /// @param output Server supported CSP versions
    /// @return Status of operation
    Status getServerProtocolVersions(service_structs::SupportedProtocolVersions<>& output) noexcept;

    constexpr const traits::Interface& getInterface() const noexcept
    {
        return m_interface;
    }


private:
    bool m_isReady{ false };

    protocol_version_t m_protocolVersion{ traits::getLatestProtocolVersion() };

    context::CommonFlags m_mandatoryCommonFlags{ helpers::isBitness32(), helpers::isModuleIsBigEndian() };
    context::CommonFlags m_forbiddenCommonFlags;

    context::DataFlags m_mandatoryDataFlags;
    context::DataFlags m_forbiddenDataFlags;

    traits::Interface& m_interface;

    UniquePtr<IDataClientSpeaker> m_dataClientSpeaker;
};

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    return handleData(input, output, m_mandatoryDataFlags, m_mandatoryCommonFlags, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, context::DataFlags dataFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    return handleData(input, output, dataFlags, m_mandatoryCommonFlags, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status DataClient::handleData(const InputType& input, OutputType& output, context::DataFlags dataFlags
    , context::CommonFlags commonFlags, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    if (!isReady())
        return Status::kErrorNotInited;

    if (InputType::getOriginPrivateVersion() > m_serverInterfaceVersion || OutputType::getOriginPrivateVersion() > m_serverInterfaceVersion)
        return Status::kErrorNotSupportedInterfaceVersion;

    if (dataFlags & m_forbiddenDataFlags || commonFlags & m_forbiddenCommonFlags)
        return Status::kErrorNotCompatibleFlagsSettings;

    BinVector binInput;

    // First attempt of sending data is made of interface version function parameters
    context::SData<> ctxIn(binInput, m_protocolVersion, commonFlags, dataFlags, forTempUseHeap, getInterface().version, nullptr);

    if constexpr (std::is_same_v<InputType, service_structs::ISerializableDummy<>>)
        ctxIn.setMessageType(context::Message::kGetSettings);

    std::unordered_map<const void*, uint64_t> pointersMapIn;
    if (ctxIn.getDataFlags().checkRecursivePointers)
        ctxIn.setPointersMap(&pointersMapIn);

    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.getDataFlags().allowUnmanagedPointers && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    RUN(processing::DataProcessor::serializeData(input, ctxIn));

    pointersMapIn.clear();

    BinWalker binOutput;

    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    ctxIn.clear();

    context::Common<BinWalker> ctxOut(binOutput);

    RUN(processing::deserializeCommonContext(ctxOut));

    if (ctxOut.getCommonFlags() & m_forbiddenCommonFlags)
        return Status::kErrorNotCompatibleFlagsSettings;

    if (ctxOut.getMessageType() == context::Message::kData)
    {
        ctxIn.clear();

        Id outId;

        context::DData<> ctxOutData(ctxOut);

        RUN(processing::deserializeDataContext(ctxOutData, outId));

        if (ctxIn.getDataFlags() != ctxOutData.getDataFlags())
            return Status::kErrorDataCorrupted;

        ctxOutData.setAddedPointers(pUnmanagedPointers);

        std::unordered_map<uint64_t, void*> pointersMapOut;
        if (ctxOutData.getDataFlags().checkRecursivePointers)
            ctxOutData.setPointersMap(&pointersMapOut);

        RUN(processing::deserializeDataContextPostprocess<OutputType>(ctxOutData, outId, minimumOutputInterfaceVersion));

        if (ctxOutData.getDataFlags() & m_forbiddenDataFlags)
            return Status::kErrorNotCompatibleFlagsSettings;

        RUN(processing::DataProcessor::deserializeData(ctxOutData, output));
    }
    else if (ctxOut.getMessageType() == context::Message::kStatus)
    {
        Status statusOut = Status::kNoError;
        RUN(processing::deserializeStatusContext(ctxOut, statusOut));

        if constexpr (std::is_same_v<OutputType, ISerializableDummy<>>)
            if (statusOut == Status::kNoError)
                return Status::kNoError;
        
        if (statusOut == Status::kErrorNotSupportedProtocolVersion)
        {
            Vector<protocol_version_t> serverCspVersions;
            RUN(processing::deserializeStatusErrorNotSupportedProtocolVersionBody(ctxOut, serverCspVersions));

            protocol_version_t compatProtocolVersion = traits::kProtocolVersionUndefined;

            for (protocol_version_t serverCspVersion : serverCspVersions)
                if (traits::isProtocolVersionSupported(serverCspVersion))
                {
                    compatProtocolVersion = serverCspVersion;
                    break;
                }

            if (compatProtocolVersion == traits::kProtocolVersionUndefined)
                return statusOut;

            // Here we should free resources that we do no need now
            ctxOut.clear();

            // Try again with the compatible protocol version
            return handleData<InputType, OutputType, forTempUseHeap>(input, output, ctxIn.getDataFlags(), inputInterfaceVersion, outputInterfaceVersion
                , minimumInputInterfaceVersion, minimumOutputInterfaceVersion, compatProtocolVersion, pUnmanagedPointers);
        }
        else if (statusOut == Status::kErrorNotSupportedInOutInterfaceVersion)
        {
            // Scenario when all minimum and prefered interface versions are the same 
            // does not imply any further processing when we've get Status::kErrorNotSupportedInOutInterfaceVersion Output
            if (minimumInputInterfaceVersion == preferedInputInterfaceVersion && minimumOutputInterfaceVersion == preferedOutputInterfaceVersion)
                return statusOut;

            interface_version_t serverMinInInterfaceVersion = 0;
            interface_version_t serverMaxInInterfaceVersion = 0;
            interface_version_t serverMinOutInterfaceVersion = 0;
            interface_version_t serverMaxOutInterfaceVersion = 0;

            RUN(processing::deserializeStatusErrorNotSupportedInOutInterfaceVersionBody(ctxOut
                , serverMinInInterfaceVersion, serverMaxInInterfaceVersion, serverMinOutInterfaceVersion, serverMaxOutInterfaceVersion));

            interface_version_t compatInputInterfaceVersion
                = traits::getBestSupportedInterfaceVersion<InputType>(serverMinInInterfaceVersion, serverMaxInInterfaceVersion, minimumInputInterfaceVersion);

            if (compatInputInterfaceVersion == traits::kInterfaceVersionUndefined)
                return statusOut;

            interface_version_t compatOutputInterfaceVersion
                = traits::getBestSupportedInterfaceVersion<OutputType>(serverMinOutInterfaceVersion, serverMaxOutInterfaceVersion, minimumOutputInterfaceVersion);

            if (compatOutputInterfaceVersion == traits::kInterfaceVersionUndefined)
                return statusOut;

            // Here we should free resources that we do no need now
            ctxOut.clear();

            // Try again with the compatible InputType and OutputType versions
            return handleData<InputType, OutputType, forTempUseHeap>(input, output, ctxIn.getDataFlags(), compatInputInterfaceVersion, compatOutputInterfaceVersion
                , compatInputInterfaceVersion, compatOutputInterfaceVersion, protocolVersion, pUnmanagedPointers);
        }
        else
            return statusOut;
    }

    return Status::kNoError;
}

inline Status DataClient::getServerProtocolVersions(service_structs::SupportedProtocolVersions<>& output) noexcept
{
    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, 1, context::CommonFlags{}, context::Message::kCommonCapabilitiesRequest);
    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeCommonCapabilitiesRequest(context::CommonCapabilities::kSupportedProtocolVersions, ctxIn));

    BinWalker binOutput;

    RUN(m_dataClientSpeaker->speak(binInput, binOutput));

    return output.deserialize(binOutput);
}

} // namespace common_serialization::csp::messaging
