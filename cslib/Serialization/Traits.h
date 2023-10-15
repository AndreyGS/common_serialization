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

namespace traits
{

struct NameHashAndVersion
{
    uint64_t nameHash{ 0 };
    uint32_t thisVersion{ 0 };
};

inline constexpr uint8_t kProtocolVersions[] = { 1 };
inline constexpr uint8_t kProtocolVersionMax = 0xff;
inline constexpr uint32_t kInterfaceVersionMax = 0xffffffff;

constexpr uint8_t getLatestProtocolVersion()
{
    return kProtocolVersions[0];
}

constexpr bool isProtocolVersionSameAsLatestOur(uint8_t foreignProtocolVersion)
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

constexpr uint8_t getProtocolVersionsCount()
{
    return std::size(kProtocolVersions);
}

constexpr bool isProtocolVersionSupported(uint8_t foreignProtocolVersion) noexcept
{
    for (uint8_t i = 0; i < getProtocolVersionsCount(); ++i)
        if (foreignProtocolVersion == kProtocolVersions[i])
            return true;

    return false;
}

constexpr bool isInterfaceVersionSupported(uint32_t version, uint32_t minVersion, uint32_t maxVersion) noexcept
{
    return maxVersion >= version && version <= minVersion;
}

// Used to find version of struct to which we shall serialize when we are in compat mode
constexpr uint32_t getBestCompatInterfaceVersion(const NameHashAndVersion* supportedInterfaceVersions
    , uint32_t supportedInterfaceVersionsSize, uint32_t compatInterfaceVersion) noexcept
{
    for (uint32_t i = 0; i < supportedInterfaceVersionsSize; ++i)
        if (supportedInterfaceVersions[i].thisVersion <= compatInterfaceVersion)
            return supportedInterfaceVersions[i].thisVersion;

    return kInterfaceVersionMax;
}

} // namespace traits

} // namespace csp

} // namespace common_serialization
