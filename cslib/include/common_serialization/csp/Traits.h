/**
 * @file cslib/include/common_serialization/csp/Traits.h
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

#include "common_serialization/csp/context/DataFlags.h"

namespace common_serialization::csp
{

using Id = Uuid;
using interface_version_t = uint32_t;
using protocol_version_t = uint8_t;
#ifdef X32
using csp_size_t = uint32_t;
#else
using csp_size_t = uint64_t;
#endif // #ifdef X32

namespace traits
{

inline constexpr protocol_version_t kProtocolVersions[] = { 1 };
inline constexpr protocol_version_t kProtocolVersionUndefined = 0xff;           // it always must be kind of UINTMAX
inline constexpr interface_version_t kInterfaceVersionUndefined = 0xffffffff;   // it always must be kind of UINTMAX

} // namespace traits

struct Interface
{
    using simply_assignable_fixed_size_tag = std::true_type;

    Id id{ kNullUuid };

    /// @brief The only field that allowed to change since interface publication
    interface_version_t version{ traits::kInterfaceVersionUndefined };

    context::DataFlags mandatoryDataFlags;
    context::DataFlags forbiddenDataFlags;

    constexpr Interface() {}
    constexpr Interface(const Id& id, interface_version_t version, context::DataFlags mandatoryDataFlags, context::DataFlags forbiddenDataFlags)
        : id(id), version(version), mandatoryDataFlags(mandatoryDataFlags), forbiddenDataFlags(forbiddenDataFlags)
    { }

    bool operator==(const Interface& rhs) const noexcept
    {
        return id == rhs.id && version == rhs.version
            && mandatoryDataFlags == rhs.mandatoryDataFlags && forbiddenDataFlags == rhs.forbiddenDataFlags;
    }
};

namespace traits
{

constexpr Interface kUndefinedInterface{ kNullUuid, kInterfaceVersionUndefined, context::DataFlags{}, context::DataFlags{} };

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

} // namespace traits

} // namespace common_serialization::csp
