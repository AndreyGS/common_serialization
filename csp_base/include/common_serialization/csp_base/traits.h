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

[[nodiscard]] constexpr protocol_version_t getLatestProtocolVersion() noexcept
{
    return kProtocolVersions[0];
}

[[nodiscard]] constexpr bool isProtocolVersionSameAsLatestOur(protocol_version_t foreignProtocolVersion) noexcept
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

[[nodiscard]] constexpr protocol_version_t getProtocolVersionsCount() noexcept
{
    return sizeof(kProtocolVersions);
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

// Using to find index of version in versions hierarchy of struct to which we must serialize/deserialize
template<typename T>
[[nodiscard]] constexpr interface_version_t getBestCompatInterfaceVersion(interface_version_t compatInterfaceVersion) noexcept
{
    const interface_version_t* pPrivateVersions = T::getPrivateVersions();

    for (interface_version_t i = 0; i < T::getPrivateVersionsCount(); ++i)
        if (pPrivateVersions[i] <= compatInterfaceVersion)
            return pPrivateVersions[i];

    return kInterfaceVersionUndefined;
}

template<typename T>
[[nodiscard]] constexpr interface_version_t getBestSupportedInterfaceVersion(
    interface_version_t minForeignVersion, interface_version_t maxForeignVersion, interface_version_t minCurrentVersion
) noexcept
{
    if (minCurrentVersion > minForeignVersion || minForeignVersion > T::getLatestInterfaceVersion())
        return kInterfaceVersionUndefined;

    if (maxForeignVersion >= T::getLatestInterfaceVersion())
        return T::getLatestInterfaceVersion();
    else
        return maxForeignVersion;
}

/// @brief Shortcut to get type interface version in template contexts
/// @tparam _T Type for which interface version is requested
/// @return Latest version of interface if _T implements ISerializable and 0 otherwise
template<typename _T>
consteval interface_version_t getLatestInterfaceVersion()
{
    if constexpr (ISerializableImpl<_T>)
        return _T::getLatestInterfaceVersion();
    else
        return 0;
}

} // namespace common_serialization::csp::traits
