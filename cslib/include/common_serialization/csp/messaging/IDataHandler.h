/**
 * @file cslib/include/common_serialization/csp/messaging/IDataHandler.h
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

#include "common_serialization/csp/messaging/IDataHandlerBase.h"
#include "common_serialization/csp/messaging/IDataHandlersRegistrar.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of concrete CSP Data handlers
template<IDataHandlerTraits _T>
class IDataHandler : public IDataHandlerBase<typename _T::Sdcs>
{
public:
    using InputType = typename _T::InputType;
    using OutputType = typename _T::OutputType;

    static constexpr bool forTempUseHeap = _T::forTempUseHeap;
    static constexpr bool forTempUseHeapExtended = _T::forTempUseHeapExtended;
    static constexpr bool multicast = _T::multicast;
    static constexpr interface_version_t minimumInterfaceVersion  = _T::minimumInterfaceVersion;
    
    using Sdcs = typename _T::Sdcs;
    using Sbin = typename Sdcs::Sbin;
    using Dbin = typename Sdcs::Dbin;
    using Spm = typename Sdcs::Spm;
    using Dpm = typename Sdcs::Dpm;
    using Gkc = typename Sdcs::Gkc;

    using Scs = typename Sdcs::Scs;
    using Dcs = typename Sdcs::Dcs;

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
    virtual Status handleData(const InputType& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, const GenericPointerKeeper& clientId, OutputType& output) = 0;
    virtual Status checkPoliciesCompliance(const InputType* input, const context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId);

    [[nodiscard]] interface_version_t getMinimumInterfaceVersion() override;

protected:
    IDataHandler(IDataHandlersRegistrar<Sdcs>& serviceServer);
    IDataHandler(const IDataHandler&) = delete;
    IDataHandler(IDataHandler&&) = delete;
    IDataHandler& operator=(const IDataHandler&) = delete;
    IDataHandler& operator=(IDataHandler&&) = delete;
    ~IDataHandler();

private:
    Status handleDataCommon(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput) override;

    CS_ALWAYS_INLINE Status handleDataOnStack(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput);
    CS_ALWAYS_INLINE Status handleDataOnHeap(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput);
    // This is the common code between handleDataOnStack and handleDataOnHeap
    CS_ALWAYS_INLINE Status handleDataMain(InputType& input, context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, OutputType& output, Sbin& binOutput);

    IDataHandlersRegistrar<Sdcs>& m_dataHandlersRegistrar;
};

template<IDataHandlerTraits _T>
Status IDataHandler<_T>::checkPoliciesCompliance(const InputType* input, const context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId)
{
    return Status::kNoError;
}

template<IDataHandlerTraits _T>
[[nodiscard]] interface_version_t IDataHandler<_T>::getMinimumInterfaceVersion()
{
    return minimumInterfaceVersion;
}

template<IDataHandlerTraits _T>
Status IDataHandler<_T>::handleDataCommon(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput
)
{
    CS_RUN(this->checkPoliciesCompliance(static_cast<const InputType*>(nullptr), ctx, clientId));

    // We already checked equality of ID in context and in subscriber
    // so here it is only placeholder
    Id id = InputType::getId();

    if (Status status = processing::deserializeDataContextPostprocess<InputType>(ctx, id, getMinimumInterfaceVersion()); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedInterfaceVersion)
        {
            context::Common<BinVector> ctxOut(binOutput, ctx.getProtocolVersion(), context::Message::Status, ctx.getCommonFlags());
            CS_RUN(processing::serializeStatusErrorNotSupportedInterfaceVersion(getMinimumInterfaceVersion(), OutputType::getId(), ctxOut));
        }
        
        return status;
    }

    ctx.setAuxUsingHeapAllocation(forTempUseHeap);

    if constexpr (forTempUseHeap)
        return handleDataOnHeap(ctx, clientId, binOutput);
    else
        return handleDataOnStack(ctx, clientId, binOutput);
}

template<IDataHandlerTraits _T>
IDataHandler<_T>::IDataHandler(IDataHandlersRegistrar<Sdcs>& dataHandlersRegistrar)
    : m_dataHandlersRegistrar(dataHandlersRegistrar)
{
    m_dataHandlersRegistrar.addHandler(InputType::getId(), multicast, this);
}

template<IDataHandlerTraits _T>
IDataHandler<_T>::~IDataHandler()
{
    m_dataHandlersRegistrar.removeHandler(InputType::getId(), this);
}

template<IDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IDataHandler<_T>::handleDataOnStack(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput
)
{
    InputType input;
    OutputType output;

    return handleDataMain(input, ctx, clientId, output, binOutput);
}

template<IDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IDataHandler<_T>::handleDataOnHeap(context::Data<Dcs>& ctx, const GenericPointerKeeper& clientId, Sbin& binOutput
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

template<IDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IDataHandler<_T>::handleDataMain(InputType& input, context::Data<Dcs>& ctxIn, const GenericPointerKeeper& clientId, OutputType& output, Sbin& binOutput
)
{
    CS_RUN(processing::data::BodyProcessor::deserialize(ctxIn, input));
    
    CS_RUN(this->handleData(input, ctxIn.getAddedPointers(), clientId, output))

    if constexpr (!std::is_same_v<OutputType, service_structs::ISerializableDummy<>>)
    {
        Spm pointersMapOut;

        binOutput.clear();

        context::Data<Scs> ctxOut(
            binOutput
            , ctxIn.getProtocolVersion()
            , ctxIn.getCommonFlags()
            , ctxIn.getDataFlags()
            , forTempUseHeap
            , ctxIn.getInterfaceVersion()
            , nullptr);

        if (ctxOut.checkRecursivePointers())
            ctxOut.setPointersMap(&pointersMapOut);

        return output.serialize(ctxOut);
    }
    else
        return Status::kNoError;
}

} // namespace common_serialization::csp::messaging
