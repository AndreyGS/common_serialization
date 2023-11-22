/**
 * @file Traits.h
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

using name_hash_t = uint64_t;
using interface_version_t = uint32_t;
using protocol_version_t = uint8_t;

namespace traits
{

inline constexpr protocol_version_t kProtocolVersions[] = { 1 };
inline constexpr protocol_version_t kProtocolVersionMax = 0xff;
inline constexpr interface_version_t kInterfaceVersionMax = 0xffffffff;

[[nodiscard]] constexpr protocol_version_t getLatestProtocolVersion()
{
    return kProtocolVersions[0];
}

[[nodiscard]] constexpr bool isProtocolVersionSameAsLatestOur(protocol_version_t foreignProtocolVersion)
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

[[nodiscard]] constexpr protocol_version_t getProtocolVersionsCount()
{
    return std::size(kProtocolVersions);
}

[[nodiscard]] constexpr bool isProtocolVersionSupported(protocol_version_t foreignProtocolVersion) noexcept
{
    for (protocol_version_t i = 0; i < getProtocolVersionsCount(); ++i)
        if (foreignProtocolVersion == kProtocolVersions[i])
            return true;

    return false;
}

[[nodiscard]] constexpr bool isInterfaceVersionSupported(interface_version_t version, interface_version_t minVersion, interface_version_t maxVersion) noexcept
{
    return maxVersion >= version && version >= minVersion;
}

template<typename T>
[[nodiscard]] constexpr interface_version_t getThisVersion() noexcept
{
    return T::getThisVersion();
}

[[nodiscard]] constexpr interface_version_t getMinimumInterfaceVersion(const interface_version_t* pVersionsHierarchy, interface_version_t versionsHierarchySize) noexcept
{
    return pVersionsHierarchy[versionsHierarchySize - 1];
}

// Using to find index of version in versions hierarchy of struct to which we must serialize/deserialize
[[nodiscard]] constexpr interface_version_t getBestCompatInterfaceVersion(const interface_version_t* pVersionsHierarchy
    , interface_version_t versionsHierarchySize, interface_version_t compatInterfaceVersion) noexcept
{
    for (interface_version_t i = 0; i < versionsHierarchySize; ++i)
        if (pVersionsHierarchy[i] <= compatInterfaceVersion)
            return pVersionsHierarchy[i];

    return kInterfaceVersionMax;
}

} // namespace traits

} // namespace csp

} // namespace common_serialization
