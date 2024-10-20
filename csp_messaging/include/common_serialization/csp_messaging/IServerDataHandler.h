/**
 * @file common_serialization/csp_messaging/IServerDataHandler.h
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

#include <common_serialization/csp_messaging/IServerDataHandlerBase.h>
#include <common_serialization/csp_messaging/IServerDataHandlerRegistrar.h>
#include <common_serialization/csp_messaging/IServerDataHandlerTraits.h>
#include <common_serialization/csp_base/processing/data/ContextProcessor.h>
#include <common_serialization/csp_base/processing/status/Helpers.h>

namespace common_serialization::csp::messaging
{

/// @brief Interface of concrete CSP Data handlers
template<IServerDataHandlerTraitsImpl T>
class IServerDataHandler : public IServerDataHandlerBase
{
public:
    using InputType = typename T::InputType;
    using OutputType = typename T::OutputType;

    static constexpr bool kForTempUseHeap = T::kForTempUseHeap;
    static constexpr bool kMulticast = T::kMulticast;
    static constexpr interface_version_t kMinimumInterfaceVersion  = T::kMinimumInterfaceVersion;
    
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

    AGS_CS_ALWAYS_INLINE Status registerHandler(IServerDataHandlerRegistrar& handlerRegistrar, void* pService);
    AGS_CS_ALWAYS_INLINE void unregisterHandler(IServerDataHandlerRegistrar& handlerRegistrar);
    [[nodiscard]] interface_version_t getMinimumInterfaceVersion();

protected:
    IServerDataHandler() = default;
    IServerDataHandler(const IServerDataHandler&) = delete;
    IServerDataHandler& operator=(const IServerDataHandler&) = delete;

private:
    Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override;

    AGS_CS_ALWAYS_INLINE Status handleDataOnStack(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput);
    AGS_CS_ALWAYS_INLINE Status handleDataOnHeap(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput);
    // This is the common code between handleDataOnStack and handleDataOnHeap
    AGS_CS_ALWAYS_INLINE Status handleDataMain(InputType& input, context::DData& ctx, const GenericPointerKeeperT& clientId, OutputType& output, BinVectorT& binOutput);
};

template<IServerDataHandlerTraitsImpl T>
Status IServerDataHandler<T>::checkPoliciesCompliance(const InputType* input, const context::DData& ctx, const GenericPointerKeeperT& clientId)
{
    return Status::NoError;
}

template<IServerDataHandlerTraitsImpl T>
AGS_CS_ALWAYS_INLINE Status IServerDataHandler<T>::registerHandler(IServerDataHandlerRegistrar& handlerRegistrar, void* pService)
{
    return handlerRegistrar.registerHandler(InputType::getId(), kMulticast, pService, *this);
}

template<IServerDataHandlerTraitsImpl T>
AGS_CS_ALWAYS_INLINE void IServerDataHandler<T>::unregisterHandler(IServerDataHandlerRegistrar& handlerRegistrar)
{
    handlerRegistrar.unregisterHandler(InputType::getId(), *this);
}

template<IServerDataHandlerTraitsImpl T>
interface_version_t IServerDataHandler<T>::getMinimumInterfaceVersion()
{
    return kMinimumInterfaceVersion;
}

template<IServerDataHandlerTraitsImpl T>
Status IServerDataHandler<T>::handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput)
{
    AGS_CS_RUN(this->checkPoliciesCompliance(static_cast<const InputType*>(nullptr), ctx, clientId));

    // We already checked equality of ID in context and in subscriber
    // so here it is only placeholder
    Id id = InputType::getId();

    if (Status status = processing::data::ContextProcessor::deserializePostprocessRest<InputType>(ctx, getMinimumInterfaceVersion()); !statusSuccess(status))
    {
        if (status == Status::ErrorNotSupportedInterfaceVersion)
            AGS_CS_RUN(processing::status::Helpers::serializeErrorNotSupportedInterfaceVersion(ctx.getProtocolVersion(), ctx.getCommonFlags()
                , getMinimumInterfaceVersion(), OutputType::getId(), binOutput));
        
        return status;
    }

    ctx.setHeapUseForTemp(kForTempUseHeap);

    if constexpr (kForTempUseHeap)
        return handleDataOnHeap(ctx, clientId, binOutput);
    else
        return handleDataOnStack(ctx, clientId, binOutput);
}

template<IServerDataHandlerTraitsImpl T>
AGS_CS_ALWAYS_INLINE Status IServerDataHandler<T>::handleDataOnStack(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput)
{
    InputType input;
    OutputType output;

    return handleDataMain(input, ctx, clientId, output, binOutput);
}

template<IServerDataHandlerTraitsImpl T>
AGS_CS_ALWAYS_INLINE Status IServerDataHandler<T>::handleDataOnHeap(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput)
{
    GenericPointerKeeperT input;
    if (!input.allocateAndConstructOne<InputType>())
        return Status::ErrorNoMemory;

    if constexpr (std::is_same_v<OutputType, service_structs::ISerializableDummy>)
        return handleDataMain(*input.get<InputType>(), ctx, clientId, service_structs::ISerializableDummy{}, binOutput);
    else
    {
        GenericPointerKeeperT output;
        if (!output.allocateAndConstructOne<OutputType>())
            return Status::ErrorNoMemory;

        return handleDataMain(*input.get<InputType>(), ctx, clientId, *output.get<OutputType>(), binOutput);
    }
}

template<IServerDataHandlerTraitsImpl T>
AGS_CS_ALWAYS_INLINE Status IServerDataHandler<T>::handleDataMain(InputType& input, context::DData& ctxIn, const GenericPointerKeeperT& clientId, OutputType& output, BinVectorT& binOutput)
{
    AGS_CS_RUN(processing::data::BodyProcessor::deserialize(ctxIn, input));
    
    AGS_CS_RUN(this->handleData(input, ctxIn.getAddedPointers(), clientId, output))

    if constexpr (!std::is_same_v<OutputType, service_structs::ISerializableDummy>)
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
        return Status::NoError;
}

} // namespace common_serialization::csp::messaging
