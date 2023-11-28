/**
 * @file cslib/include/common_serialization/CSP/ContextMessage.h
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

#include "common_serialization/CSP/Processing.h"

namespace common_serialization::csp
{

#pragma pack(push, 1)

struct StatusErrorNotSupportedProtocolVersion
{
    protocol_version_t supportedProtocolsVersionsSize{ helpers::countof(traits::kProtocolVersions) };
    protocol_version_t supportedProtocolsVersions[helpers::countof(traits::kProtocolVersions)];

    StatusErrorNotSupportedProtocolVersion()
    {
        memcpy(supportedProtocolsVersions, traits::kProtocolVersions, supportedProtocolsVersionsSize * sizeof(protocol_version_t));
    }

    using SimpleAssignableType = std::true_type;
};

struct StatusErrorNotSupportedInterfaceVersion
{
    interface_version_t minimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t maximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    using SimpleAssignableType = std::true_type;
};

struct StatusErrorNotSupportedInterfaceVersionInOut
{
    interface_version_t inMinimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t inMaximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    interface_version_t outMinimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t outMaximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    using SimpleAssignableType = std::true_type;
};

#pragma pack(pop)

namespace processing
{

template<ISerializationCapableContainer S>
constexpr Status serializeStatusErrorNotSupportedProtocolVersion(S& output) noexcept
{
    context::Common<S> ctx(output, traits::getLatestProtocolVersion(), context::Message::kStatus);
    RUN(serializeHeaderContext(ctx));

    StatusErrorNotSupportedProtocolVersion statusMessage;

    RUN(serializeStatus(output, Status::kErrorNotSupportedProtocolVersion, statusMessage));

    return Status::kNoError;
}

template<ISerializationCapableContainer S>
constexpr Status serializeStatusErrorNotSupportedInterfaceVersionInOut(
      interface_version_t  inMinimumSupportedInterfaceVersion, interface_version_t inMaximumSupportedInterfaceVersion
    , interface_version_t outMinimumSupportedInterfaceVersion, interface_version_t outMaximumSupportedInterfaceVersion
    , S& output) noexcept
{
    context::Common<S> ctx(output, traits::getLatestProtocolVersion(), context::Message::kStatus);
    RUN(serializeHeaderContext(ctx));

    StatusErrorNotSupportedInterfaceVersionInOut statusMessage = { 
          .inMinimumSupportedInterfaceVersion  = inMinimumSupportedInterfaceVersion,  .inMaximumSupportedInterfaceVersion  = inMaximumSupportedInterfaceVersion
        , .outMinimumSupportedInterfaceVersion = outMinimumSupportedInterfaceVersion, .outMaximumSupportedInterfaceVersion = outMaximumSupportedInterfaceVersion
    };

    RUN(serializeStatus(output, Status::kErrorNotSupportedProtocolVersion, statusMessage));

    return Status::kNoError;
}

} // namespace processing

} // namespace common_serialization::csp
