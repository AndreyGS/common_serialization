/**
 * @file cslib/include/common_serialization/csp/messaging/IDataServer.h
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

#include "common_serialization/csp/messaging/CommonServer.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of concrete CSP data servers
/// @tparam InstanceType If instance type same as Dummy
///     then current server using method functions for handling data
///     and otherwise it must be derived class in CRTP pattern
///     and server using data handling with static function
/// @tparam InputType Type of data that receiving from client. Must implement ISerializable.
/// @tparam OutputType Type of data by that will be sent to client as response. Must implement ISerializable.
/// @tparam forTempUseHeap Flag indicating that big memory consumption operations
///     will use heap allocation instead of stack
/// @tparam multicast Flag indicating that this server can be not the only one
///     that may process request expressed in InputType
/// @tparam minimumInterfaceVersion Minimum interface version that this handler may process
template<
      typename InputType
    , typename OutputType
    , bool forTempUseHeap = true
    , bool multicast = false
    , interface_version_t minimumInterfaceVersion 
        = InputType::getOriginPrivateVersion() < OutputType::getOriginPrivateVersion()
        ? InputType::getOriginPrivateVersion() :  OutputType::getOriginPrivateVersion()
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
class IDataServer : public IDataServerBase
{
public:
    /// @brief This method must be overriden in concrete class.
    /// @details It receives deserialized input data and returns output data
    /// @note If concrete handler should be static this method is not overriden,
    ///     instead static function handleDataStatic() with the same signature
    ///     (with exception of "this" of course) must be implemented.
    /// @remark It is not pure virtual to be as default implementation replacement for static handlers
    /// @param input Deserialized input data
    /// @param unmanagedPointers Pointer to container with unmanaged pointers
    ///     received on input deserialization process
    /// @param clientId Any ID info that is intended to client including security,
    ///     that may help in processing decisions
    /// @param output Data that should be returned to client
    /// @return Status of operation
    virtual Status handleData(const InputType& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, const BinVector& clientId, OutputType& output) = 0;

    [[nodiscard]] interface_version_t getMinimumInterfaceVersion() override;

protected:
    IDataServer();
    ~IDataServer();

private:
    Status handleDataConcrete(context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput) override;

    Status handleDataOnStack(context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput);
    Status handleDataOnHeap(context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput);
    // This is the common code between handleDataOnStack and handleDataOnHeap
    Status handleDataMain(InputType& input, context::DData<>& ctx, const BinVector& clientId, OutputType& output, BinVector& binOutput);
};

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::getMinimumInterfaceVersion()
{
    return minimumInterfaceVersion;
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::handleDataConcrete(
    context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput
)
{
    // We already checked equality of ID in context and in subscriber
    // so here it is only placeholder
    Id id = InputType::getId();

    //RUN(CheckPoliciesCompliance<InputType, OutputType>(ctx, clientId));

    if (Status status = processing::deserializeDataContextPostprocess<InputType>(ctx, id, minimumInterfaceVersion); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedInterfaceVersion)
        {
            context::Common<BinVector> ctxOut(binOutput, ctx.getProtocolVersion(), ctx.getCommonFlags(), context::Message::kStatus);
            RUN(processing::serializeStatusErrorNotSupportedInterfaceVersion(minimumInterfaceVersion, OutputType::getId(), ctxOut));
        }
        
        return status;
    }

    ctx.setAuxUsingHeapAllocation(forTempUseHeap);

    if constexpr (forTempUseHeap)
        return handleDataOnHeap(ctx, clientId, binOutput);
    else
        return handleDataOnStack(ctx, clientId, binOutput);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::IDataServer()
{
    GetDataServersKeeper().addServer(InputType::getId(), multicast, this);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::~IDataServer()
{
    GetDataServersKeeper().removeServer(InputType::getId(), this);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::handleDataOnStack(
    context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput
)
{
    InputType input;
    OutputType output;

    return handleDataMain(input, ctx, clientId, output, binOutput);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::handleDataOnHeap(
    context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput
)
{
    GenericPointerKeeper input;
    if (!input.allocateAndConstruct<InputType, ConstructorGenericAllocatorHelper<InputType>>(1))
        return Status::kErrorNoMemory;

    GenericPointerKeeper output;
    if (!output.allocateAndConstruct<OutputType, ConstructorGenericAllocatorHelper<OutputType>>(1))
        return Status::kErrorNoMemory;

    return handleDataMain(*input.get<InputType>(), ctx, clientId, *output.get<OutputType>(), binOutput);
}

template<typename InputType, typename OutputType, bool forTempUseHeap, bool multicast, interface_version_t minimumInterfaceVersion>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status IDataServer<InputType, OutputType, forTempUseHeap, multicast, minimumInterfaceVersion>::handleDataMain(
    InputType& input, context::DData<>& ctxIn, const BinVector& clientId, OutputType& output, BinVector& binOutput
)
{
    RUN(processing::DataProcessor::deserializeData(ctxIn, input));
    
    RUN(this->handleData(input, ctxIn.getAddedPointers(), clientId, output))

    if constexpr (!std::is_same_v<OutputType, service_structs::ISerializableDummy<>>)
    {
        std::unordered_map<const void*, uint64_t> pointersMapOut;

        binOutput.clear();

        context::SData<> ctxOut(
            binOutput
            , ctxIn.getProtocolVersion()
            , ctxIn.getCommonFlags()
            , ctxIn.getDataFlags()
            , forTempUseHeap
            , ctxIn.getInterfaceVersion()
            , nullptr);

        if (ctxOut.getDataFlags().checkRecursivePointers())
            ctxOut.setPointersMap(&pointersMapOut);

        return output.serialize(ctxOut);
    }
    else
        return Status::kNoError;
}

} // namespace common_serialization::csp::messaging
