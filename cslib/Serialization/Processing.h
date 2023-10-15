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
constexpr Status serializeHeaderContext(context::Common<S>& context) noexcept
{
    S& output = context.getBinaryData();

    if (!traits::isProtocolVersionSupported(context.getProtocolVersion()))
        return Status::kErrorNotSupportedProtocolVersion;

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(context.getProtocolVersion())));
    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(context.getFlags())));
    RUN(output.pushBackArithmeticValue(context.getMessageType()));

    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeHeaderContext(context::Common<D>& context) noexcept
{
    D& input = context.getBinaryData();

    uint32_t version = 0;
    RUN(input.readArithmeticValue(version));
    uint8_t minimumSupportedVersion = context.getProtocolVersion();
    context.setProtocolVersion(static_cast<uint8_t>(version));

    if (minimumSupportedVersion > context.getProtocolVersion() || traits::getLatestProtocolVersion() < context.getProtocolVersion())
        return Status::kErrorNotSupportedProtocolVersion;

    uint32_t intFlags = 0;
    RUN(input.readArithmeticValue(intFlags));
    context::Flags flags(intFlags);

    if (!traits::isProtocolVersionSameAsLatestOur(context.getProtocolVersion()))
        flags.protocolVersionsNotMatch = true;

    context.setFlags(flags);

    context::Message messageType = context::Message::kData;
    RUN(input.readArithmeticValue(messageType));

    context.setMessageType(messageType);

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataContext(context::Data<S, PM>& context) noexcept
{
    S& output = context.getBinaryData();

    RUN(output.pushBackArithmeticValue(T::getNameHash()));

    const context::Flags flags = context.getFlags();

    if (flags)
    {
        if (flags.interfaceVersionsNotMatch
            && traits::isInterfaceVersionSupported(context.getInterfaceVersion()
                , T::getVersionsHierarchy()[T::getVersionsHierarchySize() - 1].thisVersion, T::getVersionsHierarchy()[0].thisVersion))
        {
            return Status::kErrorNotSupportedInterfaceVersion;
        }

        if (flags.extendedPointersProcessing && context.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;    
    }

    if (flags.interfaceVersionsNotMatch)
        RUN(output.pushBackArithmeticValue(context.getInterfaceVersion()))
    else
        RUN(output.pushBackArithmeticValue(T::getInterfaceVersion()));
    
    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeDataContext(context::Data<D, PM>& context, uint64_t& nameHash)
{
    D& input = context.getBinaryData();

    RUN(input.readArithmeticValue(nameHash));

    uint32_t inputInterfaceVersion = 0;
    RUN(input.readArithmeticValue(inputInterfaceVersion));
    context.setInterfaceVersion(inputInterfaceVersion);

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeDataContextPostprocess(context::Data<D, PM>& context, uint64_t nameHash, uint32_t minimumSupportedInterfaceVersion)
{
    if (T::getNameHash() != nameHash)
        return Status::kErrorMismatchOfStructNameHash;

    if (!traits::isInterfaceVersionSupported(context.getInterfaceVersion(), minimumSupportedInterfaceVersion, T::getInterfaceVersion()))
        return Status::kErrorNotSupportedInterfaceVersion;
    else if (context.getInterfaceVersion() != T::getInterfaceVersion() && !context.getFlags().interfaceVersionsNotMatch)
        return Status::kErrorMismatchOfInterfaceVersions;

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
