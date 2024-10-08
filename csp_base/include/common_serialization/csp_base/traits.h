/**
 * @file common_serialization/csp_base/traits.h
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

#include <common_serialization/csp_base/types.h>

namespace common_serialization::csp::traits
{

inline constexpr protocol_version_t kProtocolVersions[] = { 1 };
inline constexpr protocol_version_t kProtocolVersionUndefined = 0xff;
inline constexpr interface_version_t kInterfaceVersionUndefined = 0xffffffff;

constexpr [[nodiscard]] protocol_version_t getLatestProtocolVersion() noexcept
{
    return kProtocolVersions[0];
}

constexpr [[nodiscard]] protocol_version_t getProtocolVersionsCount() noexcept
{
    return sizeof(kProtocolVersions);
}

constexpr [[nodiscard]] bool isProtocolVersionSameAsLatestOur(protocol_version_t foreignProtocolVersion) noexcept
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

constexpr [[nodiscard]] bool isProtocolVersionSupported(protocol_version_t foreignProtocolVersion) noexcept
{
    for (protocol_version_t i = 0; i < getProtocolVersionsCount(); ++i)
        if (foreignProtocolVersion == kProtocolVersions[i])
            return true;

    return false;
}

constexpr [[nodiscard]] bool isInterfaceVersionSupported(interface_version_t version, interface_version_t minVersion, interface_version_t maxVersion) noexcept
{
    return maxVersion >= version && version >= minVersion;
}

} // namespace common_serialization::csp::traits
