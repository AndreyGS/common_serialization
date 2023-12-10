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

#include "common_serialization/CSP/MessagingServiceStructs.h"
#include "common_serialization/CSP/Processing.h"
#include "common_serialization/CSP/ProcessingDataProcessor.h"
#include "common_serialization/CSP/ProcessingStatus.h"

namespace common_serialization::csp::messaging
{

class IDataClient
{
public:
    template<
          typename InputType
        , typename OutputType
        , bool forTempUseHeap = true
        , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
        , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    >
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* unmanagedPointers = nullptr);

    template<
          typename InputType
        , typename OutputType
        , bool forTempUseHeap = true
        , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
        , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    >
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status handleData(
          const InputType& input
        , OutputType& output
        , context::DataFlags flags
        , interface_version_t preferedInputInterfaceVersion = m_serverInterfaceVersion
        , interface_version_t preferedOutputInterfaceVersion = m_serverInterfaceVersion
        , Vector<GenericPointerKeeper>* pUnmanagedPointers = nullptr
        , protocol_version_t protocolVersion = m_defaultProtocolVersion
    );

    inline protocol_version_t getDefaultProtocolVersion() const noexcept;
    inline context::DataFlags getDefaultFlags() const noexcept;
    inline interface_version_t getServerInterfaceVersion() const noexcept;

protected:
    IDataClient() {}
    IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultFlags, interface_version_t targetInterfaceVersion);

private:
    template<typename T, interface_version_t minimumInterfaceVersion>
        requires IsISerializableBased<T>
    interface_version_t chooseInterfaceVersion(interface_version_t preferedInterfaceVersion);

    virtual Status handleBinData(BinVector& binInput, BinWalker& binOutput) = 0;

    protocol_version_t m_defaultProtocolVersion{ traits::getLatestProtocolVersion() };
    context::DataFlags m_defaultFlags;
    interface_version_t m_serverInterfaceVersion{ traits::kInterfaceVersionUndefined };
};

template<typename InputType, typename OutputType, bool forTempUseHeap, interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* unmanagedPointers)
{
    return sendData(input, output, m_defaultProtocolVersion, m_defaultFlags, m_serverInterfaceVersion, m_serverInterfaceVersion, unmanagedPointers);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status IDataClient::handleData(const InputType& input, OutputType& output, context::DataFlags flags, interface_version_t preferedInputInterfaceVersion
    , interface_version_t preferedOutputInterfaceVersion, Vector<GenericPointerKeeper>* pUnmanagedPointers, protocol_version_t protocolVersion 
)
{
    interface_version_t inputInterfaceVersion = chooseInterfaceVersion<InputType, minimumInputInterfaceVersion>(preferedInputInterfaceVersion);
    if (inputInterfaceVersion == traits::kInterfaceVersionUndefined)
        return Status::kErrorInvalidArgument;

    interface_version_t outputInterfaceVersion = chooseInterfaceVersion<OutputType, minimumOutputInterfaceVersion>(preferedOutputInterfaceVersion);
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
    RUN(processing::serializeInOutDataContext(ctxIn));
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

        RUN(processing::deserializeDataContext(ctxOut));
        RUN(processing::deserializeDataContextPostprocess<OutputType>(ctxOut, OutputType::getUuid(), minimumOutputInterfaceVersion));

        if (flags != ctxOut.getFlags())
            return Status::kErrorDataCorrupted;

        if (flags.allowUnmanagedPointers)
            ctxOut.setAddedPointers(*pUnmanagedPointers);

        std::unordered_map<uint64_t, const void*> pointersMapOut;
        if (flags.checkRecursivePointers)
            ctxOut.setPointersMap(pointersMapOut);

        RUN(processing::DataProcessor::deserializeData(ctxOut, output));
    }
    else if (ctxOut.getMessageType() == context::Message::kStatus)
    {
        Status statusOut = Status::kNoError;
        RUN(processing::deserializeStatusGetStatus(output, statusOut));

        if constexpr (std::is_same_v<OutputType, messaging::ISerializableDummy>)
            if (statusOut == Status::kNoError)
                return Status::kNoError;

        if (statusOut == Status::kErrorNotSupportedInOutInterfaceVersion)
        {
            // Scenario when all minimum and prefered interface versions are the same 
            // does not imply any further processing when we've get Status::kErrorNotSupportedInOutInterfaceVersion Output
            if (minimumInputInterfaceVersion == preferedInputInterfaceVersion && minimumOutputInterfaceVersion == preferedOutputInterfaceVersion)
                return statusOut;

            StatusErrorNotSupportedInOutInterfaceVersion statusStruct{ 0 };
            RUN(processing::deserializeStatusGetStruct(output, statusStruct));

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
            return handleData<InputType, OutputType, forTempUseHeap, compatInputInterfaceVersion, compatOutputInterfaceVersion>
                (input, output, flags, compatInputInterfaceVersion, compatOutputInterfaceVersion, pUnmanagedPointers, protocolVersion);
        }
        else
            return statusOut;
    }

    return Status::kNoError;
}


inline protocol_version_t IDataClient::getDefaultProtocolVersion() const noexcept
{
    return m_defaultProtocolVersion;
}

inline context::DataFlags IDataClient::getDefaultFlags() const noexcept
{
    return m_defaultFlags;
}

inline interface_version_t IDataClient::getServerInterfaceVersion() const noexcept
{
    return m_serverInterfaceVersion;
}

inline IDataClient::IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultFlags, interface_version_t targetInterfaceVersion)
    : m_defaultProtocolVersion(defaultProtocolVersion), m_defaultFlags(defaultFlags), m_serverInterfaceVersion(targetInterfaceVersion)
{ }

template<typename T, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<T>
interface_version_t IDataClient::chooseInterfaceVersion(interface_version_t preferedInterfaceVersion)
{
    interface_version_t interfaceVersion
        = preferedInterfaceVersion == minimumInterfaceVersion
        ? preferedInterfaceVersion
        : preferedInterfaceVersion == traits::kInterfaceVersionUndefined
            ? m_serverInterfaceVersion
            : preferedInterfaceVersion;

    // This is also applies when interfaceVersion == traits::kInterfaceVersionUndefined 
    if (interfaceVersion > T::getInterfaceVersion())
        interfaceVersion = T::getInterfaceVersion();
    else if (interfaceVersion < T::getMinimumInterfaceVersion())
        interfaceVersion = traits::kInterfaceVersionUndefined;

    return interfaceVersion;
}

} // namespace common_serialization::csp::messaging
