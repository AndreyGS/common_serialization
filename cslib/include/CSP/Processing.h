/**
 * @file Processing.h
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

#include "Context.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeHeaderContext(context::Common<S>& ctx) noexcept
{
    S& output = ctx.getBinaryData();

    if (!traits::isProtocolVersionSupported(ctx.getProtocolVersion()))
        return Status::kErrorNotSupportedProtocolVersion;

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getProtocolVersion())));
    RUN(output.pushBackArithmeticValue(ctx.getMessageType()));

    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeHeaderContext(context::Common<D>& ctx) noexcept
{
    D& input = ctx.getBinaryData();

    uint32_t version = 0;
    RUN(input.readArithmeticValue(version));
    protocol_version_t minimumSupportedVersion = ctx.getProtocolVersion();
    ctx.setProtocolVersion(static_cast<protocol_version_t>(version));

    if (minimumSupportedVersion > ctx.getProtocolVersion() || traits::getLatestProtocolVersion() < ctx.getProtocolVersion())
        return Status::kErrorNotSupportedProtocolVersion;

    context::Message messageType = context::Message::kData;
    RUN(input.readArithmeticValue(messageType));

    ctx.setMessageType(messageType);

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status serializeDataContext(context::SData<S, PM>& ctx) noexcept
{
    S& output = ctx.getBinaryData();
    context::DataFlags flags = ctx.getFlags();

    RUN(output.pushBackArithmeticValue(T::getNameHash()));
    RUN(output.pushBackArithmeticValue(ctx.getInterfaceVersion()))

    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), T::getMinimumInterfaceVersion(), T::getInterfaceVersion()))
        return Status::kErrorNotSupportedInterfaceVersion;

    if (T::getInterfaceVersion() != ctx.getInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    if (flags.checkRecursivePointers)
    {
        if (ctx.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;

        flags.allowUnmanagedPointers = true;
    }

    ctx.setFlags(flags);

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(ctx.getFlags())));


    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status deserializeDataContext(context::DData<D, PM>& ctx, name_hash_t& nameHash)
{
    D& input = ctx.getBinaryData();

    RUN(input.readArithmeticValue(nameHash));

    interface_version_t inputInterfaceVersion = 0;
    RUN(input.readArithmeticValue(inputInterfaceVersion));
    ctx.setInterfaceVersion(inputInterfaceVersion);

    uint32_t intFlags = 0;
    RUN(input.readArithmeticValue(intFlags));
    context::DataFlags flags(intFlags);
    ctx.setFlags(flags);

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status deserializeDataContextPostprocess(context::DData<D, PM>& ctx, name_hash_t nameHash, interface_version_t minimumSupportedInterfaceVersion)
{
    if (T::getNameHash() != nameHash)
        return Status::kErrorMismatchOfStructNameHash;

    // minimumSupportedInterfaceVersion should be getMinimumInterfaceVersion value by default
    // however for some special subscribers of data struct you may override it by
    // value that is higher than minimum defined in interface version
    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), minimumSupportedInterfaceVersion, T::getInterfaceVersion()))
        return Status::kErrorNotSupportedInterfaceVersion;
    else if (ctx.getInterfaceVersion() != T::getInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    context::DataFlags flags = ctx.getFlags();

    if (flags.allowUnmanagedPointers && ctx.getAddedPointers() == nullptr)
        return Status::kErrorInvalidArgument;
    
    if (flags.checkRecursivePointers && (ctx.getPointersMap() == nullptr || !flags.allowUnmanagedPointers))
        return Status::kErrorInvalidArgument;

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
