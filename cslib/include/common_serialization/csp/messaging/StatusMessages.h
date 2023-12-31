/**
 * @file cslib/include/common_serialization/csp/messaging/StatusMessages.h
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

#include "common_serialization/csp/processing/Contexts.h"

namespace common_serialization::csp::messaging
{

#pragma pack(push, 1)

/// @brief Data part of CSP status message kErrorNotSupportedProtocolVersion
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

/// @brief Data part of CSP status message kErrorNotSupportedInterfaceVersion
struct StatusErrorNotSupportedInterfaceVersion
{
    interface_version_t minimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t maximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    using SimpleAssignableType = std::true_type;
};

/// @brief Data part of CSP status message kErrorNotSupportedInOutInterfaceVersion
struct StatusErrorNotSupportedInOutInterfaceVersion
{
    interface_version_t inMinimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t inMaximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    interface_version_t outMinimumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };
    interface_version_t outMaximumSupportedInterfaceVersion{ traits::kInterfaceVersionUndefined };

    using SimpleAssignableType = std::true_type;
};

#pragma pack(pop)

} // namespace common_serialization::csp::messaging
