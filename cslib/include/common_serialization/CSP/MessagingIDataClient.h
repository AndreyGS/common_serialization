/**
 * @file cslib/include/common_serialization/CSP/MessagingIDataClient.h
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

#include "common_serialization/CSP/Processing.h"
#include "common_serialization/CSP/ProcessingCommonCapabilities.h"
#include "common_serialization/CSP/ProcessingDataProcessor.h"
#include "common_serialization/CSP/ProcessingStatus.h"

namespace common_serialization::csp::messaging
{

class IDataClient
{
public:
    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* unmanagedPointers = nullptr);

    template<typename InputType, typename OutputType, bool forTempUseHeap = true>
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , context::DataFlags flags
        , interface_version_t preferedInputInterfaceVersion = m_defaultServerInterfaceVersion
        , interface_version_t preferedOutputInterfaceVersion = m_defaultServerInterfaceVersion
        , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
        , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
        , protocol_version_t protocolVersion = m_defaultProtocolVersion
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
    );

    Status getServerProtocolVersions(SupportedProtocolVersions<>& output) noexcept;

    protocol_version_t getDefaultProtocolVersion() const noexcept;
    context::DataFlags getDefaultFlags() const noexcept;
    const Uuid& getDefaultInterfaceId() const noexcept;
    interface_version_t getDefaultServerInterfaceVersion() const noexcept;

    void setDefaultProtocolVersion(protocol_version_t defaultProtocolVersion) noexcept;
    void setDefaultFlags(context::DataFlags defaultFlags) noexcept;
    void setDefaultInterfaceId(const Uuid& defaultInterfaceId) noexcept;
    void setDefaultServerInterfaceVersion(interface_version_t defaultServerInterfaceVersion) noexcept;

protected:
    IDataClient() {}
    IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultFlags, const Uuid& defaultInterfaceId, interface_version_t targetInterfaceVersion);

private:
    template<typename T>
        requires IsISerializableBased<T>
    interface_version_t chooseInterfaceVersion(interface_version_t preferedInterfaceVersion, interface_version_t minimumInterfaceVersion);

    virtual Status handleBinData(BinVector& binInput, BinWalker& binOutput) = 0;

    protocol_version_t m_defaultProtocolVersion{ traits::getLatestProtocolVersion() };
    context::DataFlags m_defaultFlags;

    // There can be more than one interfaces on server
    // And for simplicity we should apply default parameters to only one of it
    // ID of that interface is filling for informational purposes
    Uuid m_defaultInterfaceId;
    interface_version_t m_defaultServerInterfaceVersion{ traits::kInterfaceVersionUndefined };
};

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* unmanagedPointers)
{
    return handleData(input, output, m_defaultFlags, m_defaultServerInterfaceVersion, m_defaultServerInterfaceVersion
        , InputType::getMinimumInterfaceVersion(), OutputType::getMinimumInterfaceVersion(), m_defaultProtocolVersion, unmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, context::DataFlags flags, interface_version_t preferedInputInterfaceVersion, interface_version_t preferedOutputInterfaceVersion
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion, protocol_version_t protocolVersion, Vector<GenericPointerKeeper>* pUnmanagedPointers
)
{
    interface_version_t inputInterfaceVersion = chooseInterfaceVersion<InputType>(preferedInputInterfaceVersion, minimumInputInterfaceVersion);
    if (inputInterfaceVersion == traits::kInterfaceVersionUndefined)
        return Status::kErrorInvalidArgument;

    interface_version_t outputInterfaceVersion = chooseInterfaceVersion<OutputType>(preferedOutputInterfaceVersion, minimumOutputInterfaceVersion);
    if (outputInterfaceVersion == traits::kInterfaceVersionUndefined)
        return Status::kErrorInvalidArgument;

    if (flags.allowUnmanagedPointers && pUnmanagedPointers == nullptr)
        return Status::kErrorInvalidArgument;

    BinVector binInput;

    // First attempt of sending data is made of interface version function parameters
    context::SInOutData<> ctxIn(binInput, protocolVersion, flags, forTempUseHeap, inputInterfaceVersion, outputInterfaceVersion, nullptr);

    std::unordered_map<const void*, uint64_t> pointersMapIn;
    if (flags.checkRecursivePointers)
        ctxIn.setPointersMap(pointersMapIn);

    RUN(processing::serializeHeaderContext(ctxIn));
    RUN(processing::serializeInOutDataContext<InputType>(ctxIn));
    RUN(processing::DataProcessor::serializeData(input, ctxIn));

    pointersMapIn.clear();

    BinWalker binOutput;

    RUN(handleBinData(binInput, binOutput));

    ctxIn.clear();

    context::DData<> ctxOut(binOutput);

    RUN(processing::deserializeHeaderContext(ctxOut));

    if (ctxOut.getMessageType() == context::Message::kData)
    {
        ctxIn.clear();

        Uuid outId;

        RUN(processing::deserializeDataContext(ctxOut, outId));
        RUN(processing::deserializeDataContextPostprocess<OutputType>(ctxOut, outId, minimumOutputInterfaceVersion));

        if (flags != ctxOut.getFlags())
            return Status::kErrorDataCorrupted;

        if (flags.allowUnmanagedPointers)
            ctxOut.setAddedPointers(*pUnmanagedPointers);

        std::unordered_map<uint64_t, void*> pointersMapOut;
        if (flags.checkRecursivePointers)
            ctxOut.setPointersMap(pointersMapOut);

        RUN(processing::DataProcessor::deserializeData(ctxOut, output));
    }
    else if (ctxOut.getMessageType() == context::Message::kStatus)
    {
        Status statusOut = Status::kNoError;
        RUN(processing::deserializeStatusGetStatus(ctxOut.getBinaryData(), statusOut));

        if constexpr (std::is_same_v<OutputType, messaging::ISerializableDummy<>>)
            if (statusOut == Status::kNoError)
                return Status::kNoError;
        
        if (statusOut == Status::kErrorNotSupportedProtocolVersion)
        {
            protocol_version_t protVersionsCount = traits::kProtocolVersionUndefined;
            RUN(ctxOut.getBinaryData().readArithmeticValue(protVersionsCount));
            
            if (ctxOut.getBinaryData().tell() + protVersionsCount / sizeof(protocol_version_t) > ctxOut.getBinaryData().size())
                return Status::kErrorInternal;
            
            const protocol_version_t* pProtVersions = 
                static_cast<const protocol_version_t*>(static_cast<const void*>(&*(ctxOut.getBinaryData().getVector().begin() + ctxOut.getBinaryData().tell())));

            protocol_version_t compatProtocolVersion = traits::kProtocolVersionUndefined;

            for (protocol_version_t i = 0; i < protVersionsCount; ++i)
                if (traits::isProtocolVersionSupported(pProtVersions[i]))
                {
                    compatProtocolVersion = pProtVersions[i];
                    break;
                }

            if (compatProtocolVersion == traits::kProtocolVersionUndefined)
                return statusOut;

            // Here we should free resources that we do no need now
            ctxOut.clear();

            // Try again with the compatible protocol version
            return handleData<InputType, OutputType, forTempUseHeap>(input, output, flags, inputInterfaceVersion, outputInterfaceVersion
                , minimumInputInterfaceVersion, minimumOutputInterfaceVersion, compatProtocolVersion, pUnmanagedPointers);
        }
        else if (statusOut == Status::kErrorNotSupportedInOutInterfaceVersion)
        {
            // Scenario when all minimum and prefered interface versions are the same 
            // does not imply any further processing when we've get Status::kErrorNotSupportedInOutInterfaceVersion Output
            if (minimumInputInterfaceVersion == preferedInputInterfaceVersion && minimumOutputInterfaceVersion == preferedOutputInterfaceVersion)
                return statusOut;

            StatusErrorNotSupportedInOutInterfaceVersion statusStruct{ 0 };
            RUN(processing::deserializeStatusGetStruct(ctxOut.getBinaryData(), statusStruct));

            interface_version_t compatInputInterfaceVersion
                = traits::getBestSupportedInterfaceVersion<InputType>(
                      statusStruct.inMinimumSupportedInterfaceVersion
                    , statusStruct.inMaximumSupportedInterfaceVersion
                    , minimumInputInterfaceVersion
                    );

            if (compatInputInterfaceVersion == traits::kInterfaceVersionUndefined)
                return statusOut;

            interface_version_t compatOutputInterfaceVersion
                = traits::getBestSupportedInterfaceVersion<OutputType>(
                      statusStruct.outMinimumSupportedInterfaceVersion
                    , statusStruct.outMaximumSupportedInterfaceVersion
                    , minimumOutputInterfaceVersion
                    );

            if (compatOutputInterfaceVersion == traits::kInterfaceVersionUndefined)
                return statusOut;

            // Here we should free resources that we do no need now
            ctxOut.clear();

            // Try again with the compatible InputType and OutputType versions
            return handleData<InputType, OutputType, forTempUseHeap>(input, output, flags, compatInputInterfaceVersion, compatOutputInterfaceVersion
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
    context::Common<BinVector> ctxIn(binInput, 1, context::Message::kCommonCapabilitiesRequest);
    RUN(processing::serializeHeaderContext(ctxIn));
    RUN(processing::serializeCommonCapabilitiesRequest(context::CommonCapabilities::kSupportedProtocolVersions, ctxIn));

    BinWalker binOutput;

    RUN(handleBinData(binInput, binOutput));

    context::DData<BinWalker> ctxOut(binOutput);

    RUN(processing::deserializeHeaderContext(ctxOut));

    if (ctxOut.getMessageType() != context::Message::kCommonCapabilitiesResponse)
        return Status::kErrorDataCorrupted;

    return processing::deserializeCommonCapabilitiesResponse(ctxOut, context::CommonCapabilities::kSupportedProtocolVersions, output);
}

inline protocol_version_t IDataClient::getDefaultProtocolVersion() const noexcept
{
    return m_defaultProtocolVersion;
}

inline context::DataFlags IDataClient::getDefaultFlags() const noexcept
{
    return m_defaultFlags;
}

inline const Uuid& IDataClient::getDefaultInterfaceId() const noexcept
{
    return m_defaultInterfaceId;
}

inline interface_version_t IDataClient::getDefaultServerInterfaceVersion() const noexcept
{
    return m_defaultServerInterfaceVersion;
}

inline void IDataClient::setDefaultProtocolVersion(protocol_version_t defaultProtocolVersion) noexcept
{
    m_defaultProtocolVersion = defaultProtocolVersion;
}

inline void IDataClient::setDefaultFlags(context::DataFlags defaultFlags) noexcept
{
    m_defaultFlags = defaultFlags;
}

inline void IDataClient::setDefaultInterfaceId(const Uuid& defaultInterfaceId) noexcept
{
    m_defaultInterfaceId = defaultInterfaceId;
}

inline void IDataClient::setDefaultServerInterfaceVersion(interface_version_t defaultServerInterfaceVersion) noexcept
{
    m_defaultServerInterfaceVersion = defaultServerInterfaceVersion;
}

inline IDataClient::IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultFlags, const Uuid& defaultInterfaceId, interface_version_t targetInterfaceVersion)
    : m_defaultProtocolVersion(defaultProtocolVersion), m_defaultFlags(defaultFlags), m_defaultInterfaceId(defaultInterfaceId), m_defaultServerInterfaceVersion(targetInterfaceVersion)
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
    if (interfaceVersion > T::getInterfaceVersion())
        interfaceVersion = T::getInterfaceVersion();
    else if (interfaceVersion < T::getMinimumInterfaceVersion())
        interfaceVersion = traits::kInterfaceVersionUndefined;

    return interfaceVersion;
}

} // namespace common_serialization::csp::messaging
