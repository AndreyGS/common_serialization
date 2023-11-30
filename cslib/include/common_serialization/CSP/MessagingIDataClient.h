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
#include "common_serialization/CSP/ProcessingDataProcessor.h"

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
    Status sendData(const InputType& input, OutputType& output, Vector<GenericPointerKeeper>* unmanagedPointers = nullptr)
    {
        return sendData(input, output, m_defaultProtocolVersion, m_defaultFlags, m_serverInterfaceVersion, m_serverInterfaceVersion, unmanagedPointers);
    }

    template<
          typename InputType
        , typename OutputType
        , bool forTempUseHeap = true
        , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
        , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    >
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status sendData(
          const InputType& input
        , OutputType& output
        , context::DataFlags flags
        , interface_version_t preferedInputInterfaceVersion = m_serverInterfaceVersion
        , interface_version_t preferedOutputInterfaceVersion = m_serverInterfaceVersion
        , Vector<GenericPointerKeeper>* unmanagedPointers = nullptr
        , protocol_version_t protocolVersion = m_defaultProtocolVersion
    )
    {
        interface_version_t inputInterfaceVersion = getFirstAttemptInterfaceVersion(preferedInputInterfaceVersion);
        if (inputInterfaceVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInvalidArgument;

        interface_version_t outputInterfaceVersion = getFirstAttemptInterfaceVersion(preferedOutputInterfaceVersion);
        if (outputInterfaceVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInvalidArgument;

        if constexpr (forTempUseHeap)
            return sendDataOnHeap<InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion>
                (input, output, protocolVersion, flags, inputInterfaceVersion, outputInterfaceVersion, unmanagedPointers);
        else
            return sendDataOnStack<InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion>
                (input, output, protocolVersion, flags, inputInterfaceVersion, outputInterfaceVersion, unmanagedPointers);
    }

protected:
    IDataClient() {}
    IDataClient(protocol_version_t defaultProtocolVersion, context::DataFlags defaultFlags, interface_version_t targetInterfaceVersion)
        : m_defaultProtocolVersion(defaultProtocolVersion), m_defaultFlags(defaultFlags), m_serverInterfaceVersion(targetInterfaceVersion)
    { }

private:
    template<typename T>
        requires IsISerializableBased<T>
    interface_version_t getFirstAttemptInterfaceVersion(interface_version_t preferedInterfaceVersion)
    {
        interface_version_t interfaceVersion
            = preferedInterfaceVersion == traits::kInterfaceVersionUndefined
                ? m_serverInterfaceVersion
                : preferedInterfaceVersion;

        if (interfaceVersion > T::getInterfaceVersion())
            interfaceVersion = T::getInterfaceVersion();
        else if (interfaceVersion < T::getMinimumInterfaceVersion())
            interfaceVersion = traits::kInterfaceVersionUndefined;

        return interfaceVersion;
    }

    template<
          typename InputType
        , typename OutputType
        , interface_version_t minimumInputInterfaceVersion
        , interface_version_t minimumOutputInterfaceVersion
    >
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status sendDataOnHeap(const InputType& input, OutputType& output, protocol_version_t protocolVersion, context::DataFlags flags
        , interface_version_t inputInterfaceVersion, interface_version_t outputInterfaceVersion, Vector<GenericPointerKeeper>* unmanagedPointers)
    {
        GenericPointerKeeper pointersMap;

        BinVector binInput;

        // First attempt of sending data is made of interface version function parameters
        context::SInOutData<> ctx(binInput, protocolVersion, flags, true, inputInterfaceVersion, outputInterfaceVersion, nullptr);

        if (flags.checkRecursivePointers)
        {
            if (!input.allocateAndConstruct<std::unordered_map<const void*, uint64_t>, ConstructorGenericAllocatorHelper<std::unordered_map<const void*, uint64_t>>>(1))
                return Status::kErrorNoMemory;

            ctx.setPointersMap(*pointersMap.get<std::unordered_map<const void*, uint64_t>>());
        }

        RUN(processing::serializeHeaderContext(ctx));
        RUN(processing::serializeInOutDataContext(ctx));
        RUN(processing::DataProcessor::serializeData(input, ctx));

        BinWalker binOutput;

        RUN(sendBinData(binInput, binOutput));

        context::DData<> ctx(binOutput);

        RUN(processing::deserializeHeaderContext(ctx));

        if (ctx.getMessageType() == context::Message::kData)
        {
            if (std::is_same_v<OutputType, Dummy>)
                return Status::kErrorInternal;

            RUN(processing::deserializeDataContext(ctx));
            RUN(processing::DataProcessor::deserializeData(ctx, output));
        }
        else if (ctx.getMessageType() == context::Message::kStatus)
        {


            if constexpr (std::is_same_v<OutputType, Dummy>)
            {
            }
        }

        return Status::kNoError;
    }

    template<
          typename InputType
        , typename OutputType
        , interface_version_t minimumInputInterfaceVersion
        , interface_version_t minimumOutputInterfaceVersion
    >
        requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
    Status sendDataOnStack(const InputType& input, OutputType& output, protocol_version_t protocolVersion, context::DataFlags flags
        , interface_version_t inputInterfaceVersion, interface_version_t outputInterfaceVersion, Vector<GenericPointerKeeper>* unmanagedPointers)
    {
        return Status::kErrorOverflow;
    }

    virtual Status sendBinData(const BinVector& binInput, BinWalker& binOutput) = 0;

    protocol_version_t m_defaultProtocolVersion{ traits::getLatestProtocolVersion() };
    context::DataFlags m_defaultFlags;
    interface_version_t m_serverInterfaceVersion{ traits::kInterfaceVersionUndefined };
};

} // namespace common_serialization::csp::messaging
