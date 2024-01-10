/**
 * @file cslib/include/common_serialization/csp/messaging/IDataClient.h
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

#include "common_serialization/csp/processing/CommonCapabilities.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataProcessor.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of client in CSP messaging model
/// @details See documentation of CSP
/// @note This class itself contains defined methods of handling
///     data in CSP messaging model. Concrete client must implement
///     only Status handleBinData(BinVector& binInput, BinWalker& binOutput) method.
class IDataClient
{
public:

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
    /// @details Input data serialized according to arguments of function 
    ///     and predefined settings of IDataClient object instance.
    ///     Then serialized data being sent to server(s) and received serialized response.
    ///     Response can be status of operation or expected output data.
    ///     - if response is a status that implying input message resend, but with adjusted
    ///         options that can be corrected automatically, input data repacking with it
    ///         and sends to server(s) again;
    ///     - if response is an error status that can't be handled automatically or
    ///         successed status, this status is returned;
    ///     - if response is expected output data, then it's being deserialized according
    ///         to arguments of function and predefined settings of IDataClient object instance
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
    /// @param preferedInputInterfaceVersion Version of input struct interface version that is preferable to send
    /// @param preferedOutputInterfaceVersion Version of output struct interface version that is preferable to receive
    /// @param minimumInputInterfaceVersion Minimum version of input struct interface version to which can be converted
    /// @param minimumOutputInterfaceVersion Minimum version of output struct interface version from which can be converted
    /// @param protocolVersion Protocol version that is preferable in operation
    /// @param pUnmanagedPointers Pointer on unmanaged pointers that were received on output struct deserialization
    /// @return Status of operation
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , context::DataFlags dataFlags
        , interface_version_t preferedInputInterfaceVersion = InputType::getLatestInterfaceVersion()
        , interface_version_t preferedOutputInterfaceVersion = InputType::getLatestInterfaceVersion()
        , interface_version_t minimumInputInterfaceVersion = InputType::getOriginPrivateVersion()
        , interface_version_t minimumOutputInterfaceVersion = OutputType::getOriginPrivateVersion()
        , protocol_version_t protocolVersion = traits::getLatestProtocolVersion()
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
    );

    /// @brief Shortcut to receive server supported CSP versions
    /// @param output Server supported CSP versions
    /// @return Status of operation
    Status getServerProtocolVersions(SupportedProtocolVersions<>& output) noexcept;

    /// @brief Get protocol version that is using as default in communications
    /// @return Default protocol version
    protocol_version_t getDefaultProtocolVersion() const noexcept;

    /// @brief Set protocol version that is using as default in communications
    /// @param defaultProtocolVersion Default protocol version
    void setDefaultProtocolVersion(protocol_version_t defaultProtocolVersion) noexcept;

    /// @brief Get default data flags that are using in communications
    /// @return Default data flags
    context::DataFlags getDefaultDataFlags() const noexcept;

    /// @brief Set default data flags that are using in communications
    /// @param defaultDataFlags Default data flags
    void setdefaultDataFlags(context::DataFlags defaultDataFlags) noexcept;

    /// @brief Get interface ID for which this instance is constructed
    /// @remark Interface ID is using only for informational purpose
    ///     for adding meaning of default interface version.
    /// @return Interface ID of default interface version
    const Id& getDefaultInterfaceId() const noexcept;

    /// @brief Set interface ID for which this instance is constructed
    /// @remark Interface ID is using only for informational purpose
    ///     for adding meaning of default interface version.
    /// @param defaultInterfaceId Interface ID of default interface version
    void setDefaultInterfaceId(const Id& defaultInterfaceId) noexcept;

    /// @brief Get default interface version that is using in communications
    /// @details If it is known in advance or we discovered it manually 
    ///     which interface version target server supports
    ///     we can set up this number for implicit applying it for every operation
    /// @return Default interface version
    interface_version_t getDefaultServerInterfaceVersion() const noexcept;

    /// @brief Set default interface version that is using in communications
    /// @details If it is known in advance or we discovered it manually 
    ///     which interface version target server supports
    ///     we can set up this number for implicit applying it for every operation
    /// @param defaultServerInterfaceVersion Default interface version
    void setDefaultServerInterfaceVersion(interface_version_t defaultServerInterfaceVersion) noexcept;

protected:
    /// @brief Default constructor
    IDataClient() {}

    /// @brief Constructor
    /// @param defaultProtocolVersion Protocol version that is using as default in communications
    /// @param defaultDataFlags Default data flags that are using in communications
    /// @param defaultInterfaceId Interface ID for which this instance is constructed (informational purpose only)
    /// @param targetInterfaceVersion Default interface version that is using in communications
    IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultDataFlags, const Id& defaultInterfaceId, interface_version_t targetInterfaceVersion);

private:
    template<typename T>
        requires IsISerializableBased<T>
    interface_version_t chooseInterfaceVersion(interface_version_t preferedInterfaceVersion, interface_version_t minimumInterfaceVersion);

    /// @brief Method for sending and receiving binary data to and from server
    /// @details This method must not make assumptions on what binary input and ouput data is.
    ///     It must be implemented as transport function from client to server and vice versa.
    ///     For example, it may be function that sends and receives data to and from socket.
    /// @param binInput Data that is prepared by handleData method
    /// @param binOutput Data that should be returned for processing by handleData method
    /// @return Status of operation
    virtual Status handleBinData(BinVector& binInput, BinWalker& binOutput) = 0;

    protocol_version_t m_defaultProtocolVersion{ traits::getLatestProtocolVersion() };
    context::CommonFlags m_defaultCommonFlags{ helpers::isModuleIsBigEndian() };
    context::DataFlags m_defaultDataFlags;

    // There can be more than one interfaces on server
    // ID of that interface is filling for informational purposes
    Id m_defaultInterfaceId;
    interface_version_t m_defaultServerInterfaceVersion{ traits::kInterfaceVersionUndefined };
};

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* pUnmanagedPointers)
{
    return handleData(input, output, m_defaultDataFlags, m_defaultServerInterfaceVersion, m_defaultServerInterfaceVersion
        , InputType::getOriginPrivateVersion(), OutputType::getOriginPrivateVersion(), m_defaultProtocolVersion, pUnmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, context::DataFlags dataFlags, interface_version_t preferedInputInterfaceVersion, interface_version_t preferedOutputInterfaceVersion
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion, protocol_version_t protocolVersion, Vector<GenericPointerKeeper>* pUnmanagedPointers
)
{
    interface_version_t inputInterfaceVersion = chooseInterfaceVersion<InputType>(preferedInputInterfaceVersion, minimumInputInterfaceVersion);
    if (inputInterfaceVersion == traits::kInterfaceVersionUndefined)
        return Status::kErrorInvalidArgument;

    interface_version_t outputInterfaceVersion = chooseInterfaceVersion<OutputType>(preferedOutputInterfaceVersion, minimumOutputInterfaceVersion);
    if (outputInterfaceVersion == traits::kInterfaceVersionUndefined)
        return Status::kErrorInvalidArgument;

    BinVector binInput;

    // First attempt of sending data is made of interface version function parameters
    context::SInOutData<> ctxIn(binInput, protocolVersion, m_defaultCommonFlags, dataFlags, forTempUseHeap, inputInterfaceVersion, outputInterfaceVersion, nullptr);

    std::unordered_map<const void*, uint64_t> pointersMapIn;
    if (ctxIn.getDataFlags().checkRecursivePointers)
        ctxIn.setPointersMap(&pointersMapIn);

    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeInOutDataContext<InputType>(ctxIn));

    // Flags may be changed after processing::serializeInOutDataContext
    if (ctxIn.getDataFlags().allowUnmanagedPointers && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    RUN(processing::DataProcessor::serializeData(input, ctxIn));

    pointersMapIn.clear();

    BinWalker binOutput;

    RUN(handleBinData(binInput, binOutput));

    ctxIn.clear();

    context::Common<BinWalker> ctxOut(binOutput);

    RUN(processing::deserializeCommonContext(ctxOut));

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
        RUN(processing::DataProcessor::deserializeData(ctxOutData, output));
    }
    else if (ctxOut.getMessageType() == context::Message::kStatus)
    {
        Status statusOut = Status::kNoError;
        RUN(processing::deserializeStatusContext(ctxOut, statusOut));

        if constexpr (std::is_same_v<OutputType, messaging::ISerializableDummy<>>)
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

inline Status IDataClient::getServerProtocolVersions(SupportedProtocolVersions<>& output) noexcept
{
    BinVector binInput;
    context::Common<BinVector> ctxIn(binInput, 1, context::CommonFlags{}, context::Message::kCommonCapabilitiesRequest);
    RUN(processing::serializeCommonContext(ctxIn));
    RUN(processing::serializeCommonCapabilitiesRequest(context::CommonCapabilities::kSupportedProtocolVersions, ctxIn));

    BinWalker binOutput;

    RUN(handleBinData(binInput, binOutput));

    return output.deserialize(binOutput);
}

inline protocol_version_t IDataClient::getDefaultProtocolVersion() const noexcept
{
    return m_defaultProtocolVersion;
}

inline void IDataClient::setDefaultProtocolVersion(protocol_version_t defaultProtocolVersion) noexcept
{
    m_defaultProtocolVersion = defaultProtocolVersion;
}

inline context::DataFlags IDataClient::getDefaultDataFlags() const noexcept
{
    return m_defaultDataFlags;
}

inline void IDataClient::setdefaultDataFlags(context::DataFlags defaultDataFlags) noexcept
{
    m_defaultDataFlags = defaultDataFlags;
}

inline const Id& IDataClient::getDefaultInterfaceId() const noexcept
{
    return m_defaultInterfaceId;
}

inline void IDataClient::setDefaultInterfaceId(const Id& defaultInterfaceId) noexcept
{
    m_defaultInterfaceId = defaultInterfaceId;
}

inline interface_version_t IDataClient::getDefaultServerInterfaceVersion() const noexcept
{
    return m_defaultServerInterfaceVersion;
}

inline void IDataClient::setDefaultServerInterfaceVersion(interface_version_t defaultServerInterfaceVersion) noexcept
{
    m_defaultServerInterfaceVersion = defaultServerInterfaceVersion;
}

inline IDataClient::IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultDataFlags, const Id& defaultInterfaceId, interface_version_t targetInterfaceVersion)
    : m_defaultProtocolVersion(defaultProtocolVersion), m_defaultDataFlags(defaultDataFlags), m_defaultInterfaceId(defaultInterfaceId), m_defaultServerInterfaceVersion(targetInterfaceVersion)
{ }

template<typename T>
    requires IsISerializableBased<T>
interface_version_t IDataClient::chooseInterfaceVersion(interface_version_t preferedInterfaceVersion, interface_version_t minimumInterfaceVersion)
{
    interface_version_t interfaceVersion
        = preferedInterfaceVersion == minimumInterfaceVersion
        ? preferedInterfaceVersion
        : preferedInterfaceVersion == traits::kInterfaceVersionUndefined
            ? m_defaultServerInterfaceVersion
            : preferedInterfaceVersion;

    // This is also applies when interfaceVersion == traits::kInterfaceVersionUndefined 
    if (interfaceVersion > T::getLatestInterfaceVersion())
        interfaceVersion = T::getLatestInterfaceVersion();
    else if (interfaceVersion < T::getOriginPrivateVersion())
        interfaceVersion = traits::kInterfaceVersionUndefined;

    return interfaceVersion;
}

} // namespace common_serialization::csp::messaging
