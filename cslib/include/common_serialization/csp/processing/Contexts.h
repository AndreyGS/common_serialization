/**
 * @file cslib/include/common_serialization/csp/processing/DataContext.h
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

#include "common_serialization/csp/context/InOutData.h"

namespace common_serialization::csp::processing
{

template<ISerializationCapableContainer S>
constexpr Status serializeCommonContext(context::Common<S>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    if (!traits::isProtocolVersionSupported(ctx.getProtocolVersion()))
        return Status::kErrorNotSupportedProtocolVersion;

    RUN(output.pushBackArithmeticValue(static_cast<uint16_t>(ctx.getProtocolVersion())));
    RUN(output.pushBackArithmeticValue(static_cast<uint16_t>(ctx.getCommonFlags())));
    RUN(output.pushBackArithmeticValue(ctx.getMessageType()));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D>
constexpr Status deserializeCommonContext(context::Common<D>& ctx) noexcept
{
    D& input = ctx.getBinaryData();

    uint16_t version = 0;
    RUN(input.readArithmeticValue(version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    if (minimumSupportedVersion > ctx.getProtocolVersion() || traits::getLatestProtocolVersion() < ctx.getProtocolVersion())
        return Status::kErrorNotSupportedProtocolVersion;

    uint16_t intFlags = 0;
    RUN(input.readArithmeticValue(intFlags));
    context::CommonFlags commonFlags(intFlags);
    ctx.setCommonFlags(commonFlags);

    context::Message messageType = context::Message::kData;
    RUN(input.readArithmeticValue(messageType));

    ctx.setMessageType(messageType);

    return Status::kNoError;
}


template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status serializeDataContext(context::SData<S, PM>& ctx) noexcept
{
    S& output = ctx.getBinaryData();
    context::DataFlags dataFlags = ctx.getDataFlags();

    Id id = T::getId();

    RUN(output.pushBackArithmeticValue(id.leftPart));
    RUN(output.pushBackArithmeticValue(id.rightPart));
    
    if (dataFlags.checkRecursivePointers)
    {
        if (ctx.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;

        dataFlags.allowUnmanagedPointers = true;
    }

    ctx.setDataFlags(dataFlags);

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getDataFlags())));

    RUN(output.pushBackArithmeticValue(ctx.getInterfaceVersion()))

    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), T::getOriginPrivateVersion(), T::getLatestInterfaceVersion()))
        return Status::kErrorNotSupportedInterfaceVersion;

    if (T::getLatestInterfaceVersion() != ctx.getInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    return Status::kNoError;
}

template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeDataContext(context::DData<D, PM>& ctx, Id& id)
{
    D& input = ctx.getBinaryData();

    RUN(input.readArithmeticValue(id.leftPart));
    RUN(input.readArithmeticValue(id.rightPart));

    uint32_t intFlags = 0;
    RUN(input.readArithmeticValue(intFlags));
    context::DataFlags dataFlags(intFlags);
    ctx.setDataFlags(dataFlags);

    interface_version_t inputInterfaceVersion = 0;
    RUN(input.readArithmeticValue(inputInterfaceVersion));
    ctx.setInterfaceVersion(inputInterfaceVersion);

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status serializeInOutDataContext(context::SInOutData<S, PM>& ctx) noexcept
{
    RUN(serializeDataContext<T>(ctx));
    RUN(ctx.getBinaryData().pushBackArithmeticValue(ctx.getOutputInterfaceVersion()));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeInOutDataContext(context::DInOutData<D, PM>& ctx, Id& id) noexcept
{
    RUN(deserializeDataContext(ctx, id));

    interface_version_t outputInterfaceVersion = 0;
    RUN(ctx.getBinaryData().readArithmeticValue(outputInterfaceVersion));
    ctx.setOutputInterfaceVersion(outputInterfaceVersion);

    return Status::kNoError;
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeDataContextPostprocess(context::DData<D, PM>& ctx, const Id& id, interface_version_t minimumSupportedInterfaceVersion)
{
    Id tUuid = T::getId();
    if (tUuid != id)
        return Status::kErrorMismatchOfStructId;

    context::DataFlags dataFlags = ctx.getDataFlags();

    if (dataFlags.allowUnmanagedPointers && ctx.getAddedPointers() == nullptr)
        return Status::kErrorInvalidArgument;
    
    if (dataFlags.checkRecursivePointers)
    {
        if (ctx.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;
        else if (!dataFlags.allowUnmanagedPointers)
            return Status::kErrorNotCompatibleFlagsSettings;
    }

    // minimumSupportedInterfaceVersion should be getOriginPrivateVersion value by default
    // however for some special subscribers of data struct you may override it by
    // value that is higher than minimum defined in interface version
    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), minimumSupportedInterfaceVersion, T::getLatestInterfaceVersion()))
        return Status::kErrorNotSupportedInterfaceVersion;
    else if (ctx.getInterfaceVersion() != T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    return Status::kNoError;
}


template<typename In, typename Out, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeInOutDataContextPostprocess(context::DInOutData<D, PM>& ctx, const Id& id
    , interface_version_t inputMinimumSupportedInterfaceVersion, interface_version_t outputMinimumSupportedInterfaceVersion) noexcept
{
    RUN(deserializeDataContextPostprocess<In>(ctx, id, inputMinimumSupportedInterfaceVersion));
    
    if (!traits::isInterfaceVersionSupported(ctx.getOutputInterfaceVersion(), outputMinimumSupportedInterfaceVersion, Out::getLatestInterfaceVersion()))
        return Status::kErrorNotSupportedInOutInterfaceVersion;

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusContext(context::Common<S>& ctx, Status statusOut) noexcept
{
    RUN(ctx.getBinaryData().pushBackArithmeticValue(statusOut));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D>
constexpr Status deserializeStatusContext(context::Common<D>& ctx, Status& statusOut) noexcept
{
    RUN(ctx.getBinaryData().readArithmeticValue(statusOut));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
