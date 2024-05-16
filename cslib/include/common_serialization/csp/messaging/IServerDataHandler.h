/**
 * @file cslib/include/common_serialization/csp/messaging/IServerDataHandler.h
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

#include "common_serialization/csp/messaging/IServerDataHandlerBase.h"
#include "common_serialization/csp/messaging/IServerDataHandlerRegistrar.h"
#include "common_serialization/csp/messaging/IServerDataHandlerTraits.h"

namespace common_serialization::csp::messaging
{

/// @brief Interface of concrete CSP Data handlers
template<IServerDataHandlerTraits _T>
class IServerDataHandler : public IServerDataHandlerBase
{
public:
    using InputType = typename _T::InputType;
    using OutputType = typename _T::OutputType;

    static constexpr bool kForTempUseHeap = _T::kForTempUseHeap;
    static constexpr bool kMulticast = _T::kMulticast;
    static constexpr interface_version_t kMinimumInterfaceVersion  = _T::kMinimumInterfaceVersion;
    
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
    virtual Status handleData(const InputType& input, VectorT<GenericPointerKeeperT>* pUnmanagedPointers, const GenericPointerKeeperT& clientId, OutputType& output) = 0;
    virtual Status checkPoliciesCompliance(const InputType* input, const context::DData& ctx, const GenericPointerKeeperT& clientId);

    [[nodiscard]] interface_version_t getMinimumInterfaceVersion();

protected:
    IServerDataHandler(IServerDataHandlerRegistrar& serviceServer);
    IServerDataHandler(const IServerDataHandler&) = delete;
    IServerDataHandler(IServerDataHandler&&) = delete;
    IServerDataHandler& operator=(const IServerDataHandler&) = delete;
    IServerDataHandler& operator=(IServerDataHandler&&) = delete;
    ~IServerDataHandler();

private:
    Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override;

    CS_ALWAYS_INLINE Status handleDataOnStack(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput);
    CS_ALWAYS_INLINE Status handleDataOnHeap(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput);
    // This is the common code between handleDataOnStack and handleDataOnHeap
    CS_ALWAYS_INLINE Status handleDataMain(InputType& input, context::DData& ctx, const GenericPointerKeeperT& clientId, OutputType& output, BinVectorT& binOutput);

    IServerDataHandlerRegistrar& m_dataHandlersRegistrar;
};

template<IServerDataHandlerTraits _T>
Status IServerDataHandler<_T>::checkPoliciesCompliance(const InputType* input, const context::DData& ctx, const GenericPointerKeeperT& clientId)
{
    return Status::kNoError;
}

template<IServerDataHandlerTraits _T>
[[nodiscard]] interface_version_t IServerDataHandler<_T>::getMinimumInterfaceVersion()
{
    return kMinimumInterfaceVersion;
}

template<IServerDataHandlerTraits _T>
Status IServerDataHandler<_T>::handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput
)
{
    CS_RUN(this->checkPoliciesCompliance(static_cast<const InputType*>(nullptr), ctx, clientId));

    // We already checked equality of ID in context and in subscriber
    // so here it is only placeholder
    Id id = InputType::getId();

    if (Status status = processing::deserializeDataContextPostprocessRest<InputType>(ctx, getMinimumInterfaceVersion()); !statusSuccess(status))
    {
        if (status == Status::kErrorNotSupportedInterfaceVersion)
        {
            context::SCommon ctxOut(binOutput, ctx.getProtocolVersion(), context::Message::Status, ctx.getCommonFlags());
            CS_RUN(processing::serializeStatusErrorNotSupportedInterfaceVersion(getMinimumInterfaceVersion(), OutputType::getId(), ctxOut));
        }
        
        return status;
    }

    ctx.setHeapUseForTemp(kForTempUseHeap);

    if constexpr (kForTempUseHeap)
        return handleDataOnHeap(ctx, clientId, binOutput);
    else
        return handleDataOnStack(ctx, clientId, binOutput);
}

template<IServerDataHandlerTraits _T>
IServerDataHandler<_T>::IServerDataHandler(IServerDataHandlerRegistrar& dataHandlersRegistrar)
    : m_dataHandlersRegistrar(dataHandlersRegistrar)
{
    m_dataHandlersRegistrar.registerHandler(InputType::getId(), kMulticast, this);
}

template<IServerDataHandlerTraits _T>
IServerDataHandler<_T>::~IServerDataHandler()
{
    m_dataHandlersRegistrar.unregisterHandler(InputType::getId(), this);
}

template<IServerDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IServerDataHandler<_T>::handleDataOnStack(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput
)
{
    InputType input;
    OutputType output;

    return handleDataMain(input, ctx, clientId, output, binOutput);
}

template<IServerDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IServerDataHandler<_T>::handleDataOnHeap(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput
)
{
    GenericPointerKeeperT input;
    if (!input.allocateAndConstructOne<InputType>())
        return Status::kErrorNoMemory;

    if constexpr (std::is_same_v<OutputType, service_structs::ISerializableDummy<>>)
        return handleDataMain(*input.get<InputType>(), ctx, clientId, service_structs::ISerializableDummy<>{}, binOutput);
    else
    {
        GenericPointerKeeperT output;
        if (!output.allocateAndConstructOne<OutputType>())
            return Status::kErrorNoMemory;

        return handleDataMain(*input.get<InputType>(), ctx, clientId, *output.get<OutputType>(), binOutput);
    }
}

template<IServerDataHandlerTraits _T>
CS_ALWAYS_INLINE Status IServerDataHandler<_T>::handleDataMain(InputType& input, context::DData& ctxIn, const GenericPointerKeeperT& clientId, OutputType& output, BinVectorT& binOutput
)
{
    CS_RUN(processing::data::BodyProcessor::deserialize(ctxIn, input));
    
    CS_RUN(this->handleData(input, ctxIn.getAddedPointers(), clientId, output))

    if constexpr (!std::is_same_v<OutputType, service_structs::ISerializableDummy<>>)
    {
        context::SPointersMap pointersMapOut;

        binOutput.clear();

        context::SData ctxOut(
              binOutput
            , ctxIn.getProtocolVersion()
            , ctxIn.getCommonFlags()
            , ctxIn.getDataFlags()
            , kForTempUseHeap
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
