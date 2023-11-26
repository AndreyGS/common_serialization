/**
 * @file cslib/include/common_serialization/CSP/MessagingServerData.h
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

#include "common_serialization/CSP/MessagingIDataServer.h"
#include "common_serialization/CSP/ContextInOutData.h"

namespace common_serialization::csp::messaging
{

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
class DataServer : public IDataServer
{
public:
    Status handleDataConcrete(context::DInOutData<>& ctx, BinVector& binOutput) override;
    [[nodiscard]] interface_version_t getMinimumHandlerSupportedInterfaceVersion() override;

protected:
    DataServer();
    ~DataServer();

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
Status DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataConcrete(context::DInOutData<>& ctx, BinVector& binOutput)
{
    RUN((processing::deserializeInOutDataContextPostprocess<InputType, OutputType>(
        ctx, InputType::getNameHash(), minimumInputInterfaceVersion, minimumOutputInterfaceVersion)));
    
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
interface_version_t DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::getMinimumHandlerSupportedInterfaceVersion()
{
    return minimumInputInterfaceVersion;
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::DataServer()
{
    GetDataServersKeeper().addServer(InputType::getNameHash(), multicast, this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::~DataServer()
{
    GetDataServersKeeper().removeServer(InputType::getNameHash(), this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
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
Status DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataOnStack(context::DInOutData<>& ctx, BinVector& binOutput)
{
    InputType input;
    OutputType output;

    RUN(processing::DataProcessor::deserializeData(ctx, input));

    if constexpr (std::is_same_v<InstanceType, Dummy>)
        RUN(this->handleData(input, ctx.getAddedPointers(), output))
    else
        RUN(InstanceType::handleDataStatic(input, ctx.getAddedPointers(), output))

    context::SData ctxOut(binOutput, handleOnHeap, ctx.getOutputInterfaceVersion());
    ctxOut.setFlags(ctx.getFlags());
    std::unordered_map<uint64_t, void*> pointersMap;
    ctxOut.setPointersMap(pointersMap);

    return output.serialize(ctxOut);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion, interface_version_t minimumOutputInterfaceVersion
    , bool handleOnHeap, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion
    , handleOnHeap, multicast>::handleDataOnHeap(context::DInOutData<>& ctx, BinVector& binOutput)
{
    return Status::kErrorNoSuchHandler;
}

template<typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using MethodDataServer = DataServer<Dummy, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion, handleOnHeap, multicast>;

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInputInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , interface_version_t minimumOutputInterfaceVersion = OutputType::getMinimumInterfaceVersion()
    , bool handleOnHeap = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using StaticDataServer = DataServer<InstanceType, InputType, OutputType, minimumInputInterfaceVersion, minimumOutputInterfaceVersion, handleOnHeap, multicast>;

} // namespace common_serialization::csp::messaging
