/**
 * @file cslib/include/common_serialization/csp/processing/Status.h
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

#include "common_serialization/csp/messaging/StatusMessages.h"
#include "common_serialization/csp/processing/Contexts.h"

namespace common_serialization::csp::processing
{

template<ISerializationCapableContainer S>
constexpr Status serializeStatusSuccess(S& output, protocol_version_t protocolVersion, context::CommonFlags commonFlags, Status statusOut) noexcept
{
    context::Common<S> ctx(output, protocolVersion, commonFlags, context::Message::kStatus);
    RUN(serializeCommonContext(ctx));
    RUN(serializeStatusContext(ctx, statusOut));

    return Status::kNoError;
}

template<ISerializationCapableContainer S, typename T>
constexpr Status serializeStatus(S& output, protocol_version_t protocolVersion, context::CommonFlags commonFlags, Status statusOut, T& statusMessage) noexcept
{
    context::Common<S> ctx(output, protocolVersion, commonFlags, context::Message::kStatus);
    RUN(serializeCommonContext(ctx));
    RUN(serializeStatusContext(ctx, statusOut));
    RUN(output.pushBackN(static_cast<uint8_t*>(static_cast<void*>(&statusMessage)), sizeof(T)));

    return Status::kNoError;
}

// Not applicable for variable-sized structs
template<IDeserializationCapableContainer D, typename T>
constexpr Status deserializeStatusGetBody(D& input, T& value) noexcept
{
    typename D::size_type readSize = 0;
    RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(&value)), sizeof(T), &readSize));
    if (readSize != sizeof(T))
        return Status::kErrorOverflow;

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusErrorNotSupportedProtocolVersion(S& output, context::CommonFlags commonFlags) noexcept
{
    messaging::StatusErrorNotSupportedProtocolVersion statusMessage;

    // For unsupported protocol version always using protocol version equal 1
    context::Common<S> ctx(output, 1, commonFlags, context::Message::kStatus);
    RUN(serializeCommonContext(ctx));
    RUN(serializeStatusContext(ctx, Status::kErrorNotSupportedProtocolVersion));

    RUN(ctx.getBinaryData().pushBackArithmeticValue(traits::getProtocolVersionsCount()));
    RUN(ctx.getBinaryData().pushBackN(traits::kProtocolVersions, traits::getProtocolVersionsCount()));

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusErrorNotSupportedInterfaceVersion(
      interface_version_t minimumSupportedInterfaceVersion
    , interface_version_t maximumSupportedInterfaceVersion
    , protocol_version_t protocolVersion
    , context::CommonFlags commonFlags
    , S& output) noexcept
{
    messaging::StatusErrorNotSupportedInterfaceVersion statusMessage = {
          .minimumSupportedInterfaceVersion = minimumSupportedInterfaceVersion
        , .maximumSupportedInterfaceVersion = maximumSupportedInterfaceVersion
    };

    RUN(serializeStatus(output, protocolVersion, commonFlags, Status::kErrorNotSupportedInterfaceVersion, statusMessage));

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusErrorNotSupportedInOutInterfaceVersion(
      interface_version_t  inMinimumSupportedInterfaceVersion, interface_version_t inMaximumSupportedInterfaceVersion
    , interface_version_t outMinimumSupportedInterfaceVersion, interface_version_t outMaximumSupportedInterfaceVersion
    , protocol_version_t protocolVersion
    , context::CommonFlags commonFlags
    , S& output) noexcept
{
    messaging::StatusErrorNotSupportedInOutInterfaceVersion statusMessage = {
          .inMinimumSupportedInterfaceVersion  = inMinimumSupportedInterfaceVersion
        , .inMaximumSupportedInterfaceVersion  = inMaximumSupportedInterfaceVersion
        , .outMinimumSupportedInterfaceVersion = outMinimumSupportedInterfaceVersion
        , .outMaximumSupportedInterfaceVersion = outMaximumSupportedInterfaceVersion
    };

    RUN(serializeStatus(output, protocolVersion, commonFlags, Status::kErrorNotSupportedInOutInterfaceVersion, statusMessage));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing