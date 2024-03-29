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

#include "common_serialization/csp/context/Data.h"

namespace common_serialization::csp::processing
{

template<ISerializationCapableContainer S>
constexpr Status serializeCommonContext(context::Common<S>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    if (!traits::isProtocolVersionSupported(ctx.getProtocolVersion()))
        return Status::kErrorNotSupportedProtocolVersion;

    CS_RUN(output.pushBackArithmeticValue(static_cast<uint16_t>(ctx.getProtocolVersion())));
    CS_RUN(output.pushBackArithmeticValue(ctx.getMessageType()));
    CS_RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getCommonFlags())));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D>
constexpr Status deserializeCommonContext(context::Common<D>& ctx) noexcept
{
    D& input = ctx.getBinaryData();

    uint16_t version = 0;
    CS_RUN(input.readArithmeticValue(version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    if (minimumSupportedVersion > ctx.getProtocolVersion() || traits::getLatestProtocolVersion() < ctx.getProtocolVersion())
        return Status::kErrorNotSupportedProtocolVersion;

    context::Message messageType = context::Message::kData;
    CS_RUN(input.readArithmeticValue(messageType));

    ctx.setMessageType(messageType);

    uint32_t intFlags = 0;
    CS_RUN(input.readArithmeticValue(intFlags));
    context::CommonFlags commonFlags(intFlags);
    ctx.setCommonFlags(commonFlags);

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeCommonContextNoChecks(context::Common<S>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    CS_RUN(output.pushBackArithmeticValue(static_cast<uint16_t>(ctx.getProtocolVersion())));
    CS_RUN(output.pushBackArithmeticValue(ctx.getMessageType()));
    CS_RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getCommonFlags())));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D>
constexpr Status deserializeCommonContextNoChecks(context::Common<D>& ctx) noexcept
{
    D& input = ctx.getBinaryData();

    uint16_t version = 0;
    CS_RUN(input.readArithmeticValue(version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    context::Message messageType = context::Message::kData;
    CS_RUN(input.readArithmeticValue(messageType));

    ctx.setMessageType(messageType);

    uint32_t intFlags = 0;
    CS_RUN(input.readArithmeticValue(intFlags));
    context::CommonFlags commonFlags(intFlags);
    ctx.setCommonFlags(commonFlags);

    return Status::kNoError;
}

template<typename T>
constexpr bool areDataFlagsNotCompatible(context::DataFlags dataFlags)
{
    if constexpr (!StructHaveDataFlags<T>)
        return true;
    else
    {
        constexpr context::DataFlags effectiveMandatoryDataFlags = T::getEffectiveMandatoryDataFlags();
        constexpr context::DataFlags effectiveForbiddenDataFlags = T::getEffectiveForbiddenDataFlags();

        return (dataFlags & effectiveMandatoryDataFlags) != effectiveMandatoryDataFlags || (dataFlags & effectiveForbiddenDataFlags);
    }
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status serializeDataContext(context::SData<S, PM>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    Id id = T::getId();

    CS_RUN(output.pushBackN(id.id, sizeof(id.id)));

    constexpr interface_version_t interfaceVersion = T::getInterface().version;

    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), T::getOriginPrivateVersion(), interfaceVersion))
        return Status::kErrorNotSupportedInterfaceVersion;

    CS_RUN(output.pushBackArithmeticValue(ctx.getInterfaceVersion()))

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);
    
    if (ctx.checkRecursivePointers())
    {
        if (ctx.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;
        else if (!ctx.allowUnmanagedPointers())
            return Status::kErrorNotCompatibleDataFlagsSettings;
    }

    context::DataFlags dataFlags = ctx.getDataFlags();

    if (areDataFlagsNotCompatible<T>(dataFlags))
        return Status::kErrorNotCompatibleDataFlagsSettings;

    CS_RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(dataFlags)));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeDataContext(context::DData<D, PM>& ctx, Id& id)
{
    D& input = ctx.getBinaryData();

    CS_RUN(input.read(id.id, sizeof(id.id)));

    interface_version_t inputInterfaceVersion = 0;
    CS_RUN(input.readArithmeticValue(inputInterfaceVersion));
    ctx.setInterfaceVersion(inputInterfaceVersion);

    uint32_t intFlags = 0;
    CS_RUN(input.readArithmeticValue(intFlags));
    context::DataFlags dataFlags(intFlags);
    ctx.setDataFlags(dataFlags);

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status serializeDataContextNoChecks(context::SData<S, PM>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    Id id = T::getId();

    CS_RUN(output.pushBackN(id.id, sizeof(id.id)));
    CS_RUN(output.pushBackArithmeticValue(ctx.getInterfaceVersion()))

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    CS_RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getDataFlags())));
    
    return Status::kNoError;
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status deserializeDataContextPostprocess(context::DData<D, PM>& ctx, const Id& id, interface_version_t minimumSupportedInterfaceVersion)
{
    Id tUuid = T::getId();
    if (tUuid != id)
        return Status::kErrorMismatchOfStructId;

    constexpr interface_version_t interfaceVersion = T::getInterface().version;

    // minimumSupportedInterfaceVersion should be getOriginPrivateVersion value by default
    // however for some special subscribers of data struct you may override it by
    // value that is higher than minimum defined in interface version
    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), minimumSupportedInterfaceVersion, interfaceVersion))
        return Status::kErrorNotSupportedInterfaceVersion;
    else if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    if (areDataFlagsNotCompatible<T>(ctx.getDataFlags()))
        return Status::kErrorNotCompatibleDataFlagsSettings;

    if (ctx.allowUnmanagedPointers() && ctx.getAddedPointers() == nullptr)
        return Status::kErrorInvalidArgument;
    
    if (ctx.checkRecursivePointers())
    {
        if (ctx.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;
        else if (!ctx.allowUnmanagedPointers())
            return Status::kErrorNotCompatibleDataFlagsSettings;
    }

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusContext(context::Common<S>& ctx, Status statusOut) noexcept
{
    CS_RUN(ctx.getBinaryData().pushBackArithmeticValue(statusOut));

    return Status::kNoError;
}

template<IDeserializationCapableContainer D>
constexpr Status deserializeStatusContext(context::Common<D>& ctx, Status& statusOut) noexcept
{
    CS_RUN(ctx.getBinaryData().readArithmeticValue(statusOut));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
