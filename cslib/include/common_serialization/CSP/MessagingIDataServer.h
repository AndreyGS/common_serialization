/**
 * @file cslib/include/common_serialization/CSP/MessagingIDataServer.h
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

#include "common_serialization/CSP/MessagingIDataServerBase.h"
#include "common_serialization/CSP/ContextInOutData.h"
#include "common_serialization/CSP/StatusMessages.h"

namespace common_serialization::csp::messaging
{

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
class IDataServer : public IDataServerBase
{
public:
    Status handleDataConcrete(context::DInOutData<>& ctx, BinVector& binOutput) override;
    [[nodiscard]] interface_version_t getMinimumHandlerSupportedInterfaceVersion() override;

protected:
    IDataServer();
    ~IDataServer();

private:
    // It is a default implementation replacement for static handlers
    virtual Status handleData(const InputType& input, Vector<GenericPointerKeeper>* unmanagedPointers, OutputType& output);

    Status handleDataOnStack(context::DInOutData<>& ctx, BinVector& binOutput);
    Status handleDataOnHeap(context::DInOutData<>& ctx, BinVector& binOutput);
};

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataConcrete(context::DInOutData<>& ctx, BinVector& binOutput)
{
    Status status = processing::deserializeInOutDataContextPostprocess<InputType, OutputType>(
        ctx, InputType::getNameHash(), minimumInputInterfaceVersion, minimumOutputInterfaceVersion);

    if (!statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedInterfaceVersion)
            RUN(processing::serializeStatusErrorNotSupportedInterfaceVersionInOut(minimumInputInterfaceVersion, InputType::getInterfaceVersion()
                , minimumOutputInterfaceVersion, OutputType::getInterfaceVersion(), binOutput));
        
        return status;
    }
    
    ctx.setAuxUsingHeapAllocation(handleOnHeap);

    if constexpr (handleOnHeap)
        return handleDataOnHeap(ctx, binOutput);
    else
        return handleDataOnStack(ctx, binOutput);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::getMinimumHandlerSupportedInterfaceVersion()
{
    return minimumInputInterfaceVersion;
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::IDataServer()
{
    GetDataServersKeeper().addServer(InputType::getNameHash(), multicast, this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::~IDataServer()
{
    GetDataServersKeeper().removeServer(InputType::getNameHash(), this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleData(const InputType& input, Vector<GenericPointerKeeper>* unmanagedPointers, OutputType& output)
{
    assert(false);
    return Status::kErrorNoSuchHandler;
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataOnStack(context::DInOutData<>& ctx, BinVector& binOutput)
{
    InputType input;
    OutputType output;

    RUN(processing::DataProcessor::deserializeData(ctx, input));

    if constexpr (std::is_same_v<InstanceType, Dummy>)
        RUN(this->handleData(input, ctx.getAddedPointers(), output))
    else
        RUN(InstanceType::handleDataStatic(input, ctx.getAddedPointers(), output));

    std::unordered_map<const void*, uint64_t> pointersMap;

    context::SData ctxOut(
          binOutput
        , traits::getLatestProtocolVersion()
        , context::Message::kData
        , ctx.getFlags()
        , handleOnHeap
        , ctx.getOutputInterfaceVersion()
        , &pointersMap);

    return output.serialize(ctxOut);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataOnHeap(context::DInOutData<>& ctx, BinVector& binOutput)
{
    GenericPointerKeeper input;
    if (!input.allocateAndConstruct<InputType, ConstructorGenericAllocatorHelper<InputType>>(1))
        return Status::kErrorNoMemory;

    GenericPointerKeeper output;
    if (!output.allocateAndConstruct<OutputType, ConstructorGenericAllocatorHelper<OutputType>>(1))
        return Status::kErrorNoMemory;

    RUN(processing::DataProcessor::deserializeData(ctx, *input.get<InputType>()));

    if constexpr (std::is_same_v<InstanceType, Dummy>)
        RUN(this->handleData(*input.get<InputType>(), ctx.getAddedPointers(), *output.get<OutputType>()))
    else
        RUN(InstanceType::handleDataStatic(*input.get<InputType>(), ctx.getAddedPointers(), *output.get<OutputType>()));

    GenericPointerKeeper pointersMap;

    context::SData<> ctxOut(
          binOutput
        , traits::getLatestProtocolVersion()
        , context::Message::kData
        , ctx.getFlags()
        , handleOnHeap
        , ctx.getOutputInterfaceVersion()
        , nullptr);

    if (ctx.getFlags().checkRecursivePointers)
    {
        if (!input.allocateAndConstruct<std::unordered_map<const void*, uint64_t>, ConstructorGenericAllocatorHelper<std::unordered_map<const void*, uint64_t>>>(1))
            return Status::kErrorNoMemory;

        ctxOut.setPointersMap(*pointersMap.get<std::unordered_map<const void*, uint64_t>>());
    }

    return output.get<OutputType>()->serialize(ctxOut);
}

template<typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using IMethodDataServer = IDataServer<Dummy, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion, handleOnHeap, multicast>;

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using IStaticDataServer = IDataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion, handleOnHeap, multicast>;

} // namespace common_serialization::csp::messaging
