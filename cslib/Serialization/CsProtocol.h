/**
 * @file CsProtocol.h
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

#include "SerializationConcepts.h"
#include "CsProtocolFlags.h"
#include "CsProtocolMessageType.h"

namespace common_serialization
{

struct StructNameHashAndVersion
{
    uint64_t nameHash{ 0 };
    uint32_t thisVersion{ 0 };
};

// Do not add here any member fields or methods
class CsProtocol
{
public:
    static constexpr uint8_t kProtocolVersions[] = { 1 };
    static constexpr uint8_t kProtocolVersionMax = 0xff;
    static constexpr uint32_t kInterfaceVersionMax = 0xffffffff;

    template<serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeHeader(S& output, CsProtocolFlags flags = CsProtocolFlags{}, CsProtocolMessageType messageType = CsProtocolMessageType::kData) noexcept;
    template<serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeHeaderCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat
        , CsProtocolMessageType messageType = CsProtocolMessageType::kData) noexcept;


    template<serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeHeader(D& input, CsProtocolFlags& flags, CsProtocolMessageType& messageType) noexcept;
    template<serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeHeaderCompat(D& input, uint8_t minimumSupportedProtocolVersion, uint8_t& foreignProtocolVersion
        , CsProtocolFlags& flags, CsProtocolMessageType& messageType) noexcept;

    static constexpr bool getLatestProtocolVersion();
    static constexpr bool isProtocolVersionSameAsLatestOur(uint8_t foreignProtocolVersion);
    static constexpr uint8_t getProtocolVersionsCount();
    static constexpr bool isProtocolVersionSupported(uint8_t foreignProtocolVersion) noexcept;
    static constexpr bool isInterfaceVersionCompat(uint32_t ourInterfaceVersion
        , uint32_t minimumSupportedInterfaceVersion, uint32_t compatInterfaceVersion) noexcept;
    // Used to find version of struct to which we shall serialize when we are in compat mode
    static constexpr uint32_t getBestCompatInterfaceVersion(const StructNameHashAndVersion* supportedInterfaceVersions
        , uint32_t supportedInterfaceVersionsSize, uint32_t compatInterfaceVersion) noexcept;
};

template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status CsProtocol::serializeHeader(S& output, CsProtocolFlags flags, CsProtocolMessageType messageType) noexcept
{
    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(getLatestProtocolVersion()) | (static_cast<uint32_t>(flags) << 8)));
    RUN(output.pushBackArithmeticValue(messageType));

    return Status::kNoError;
}

template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status CsProtocol::serializeHeaderCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat
    , CsProtocolMessageType messageType) noexcept
{
    if (!isProtocolVersionSupported(protocolVersionCompat))
        return Status::kErrorNotSupportedSerializationProtocolVersion;

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(protocolVersionCompat) | (static_cast<uint32_t>(flags) << 8)));
    RUN(output.pushBackArithmeticValue(messageType));

    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status CsProtocol::deserializeHeader(D& input, CsProtocolFlags& flags, CsProtocolMessageType& messageType) noexcept
{
    uint32_t versionAndFlags = 0;
    RUN(input.readArithmeticValue(versionAndFlags));

    uint8_t foreignProtocolVersion = versionAndFlags & 0xff;
    if (!isProtocolVersionSameAsLatestOur(foreignProtocolVersion))
        return Status::kErrorMismatchOfSerializationProtocolVersions;

    flags = versionAndFlags >> 8;
    RUN(input.readArithmeticValue(messageType));

    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status CsProtocol::deserializeHeaderCompat(D& input, uint8_t minimumSupportedProtocolVersion, uint8_t& foreignProtocolVersion
    , CsProtocolFlags& flags, CsProtocolMessageType& messageType) noexcept
{
    uint32_t versionAndFlags = 0;
    RUN(input.readArithmeticValue(versionAndFlags));

    foreignProtocolVersion = versionAndFlags & 0xff;
    if (!isProtocolVersionSameAsLatestOur(foreignProtocolVersion))
    {
        if (!isProtocolVersionSupported(foreignProtocolVersion) || foreignProtocolVersion < minimumSupportedProtocolVersion)
            return Status::kErrorNotSupportedSerializationProtocolVersion;

        // here shall be processing for legacy protocol versions
    }
    else
    {
        flags = versionAndFlags >> 8;
        RUN(input.readArithmeticValue(messageType));
    }

    return Status::kNoError;
}

constexpr bool CsProtocol::getLatestProtocolVersion()
{
    return kProtocolVersions[0];
}

constexpr bool CsProtocol::isProtocolVersionSameAsLatestOur(uint8_t foreignProtocolVersion)
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

constexpr uint8_t CsProtocol::getProtocolVersionsCount()
{
    return std::size(kProtocolVersions);
}

constexpr bool CsProtocol::isProtocolVersionSupported(uint8_t foreignProtocolVersion) noexcept
{
    for (uint8_t i = 0; i < getProtocolVersionsCount(); ++i)
        if (foreignProtocolVersion == kProtocolVersions[i])
            return true;

    return false;
}

// Using in deserialize
constexpr bool CsProtocol::isInterfaceVersionCompat(uint32_t ourInterfaceVersion
    , uint32_t minimumSupportedInterfaceVersion, uint32_t foreignInterfaceVersion) noexcept
{
    return foreignInterfaceVersion >= minimumSupportedInterfaceVersion && foreignInterfaceVersion <= ourInterfaceVersion;
}

// Used to find version of struct to which we shall serialize when we are in compat mode
constexpr uint32_t CsProtocol::getBestCompatInterfaceVersion(const StructNameHashAndVersion* supportedInterfaceVersions
    , uint32_t supportedInterfaceVersionsSize, uint32_t compatInterfaceVersion) noexcept
{
    for (uint32_t i = 0; i < supportedInterfaceVersionsSize; ++i)
        if (supportedInterfaceVersions[i].thisVersion <= compatInterfaceVersion)
            return supportedInterfaceVersions[i].thisVersion;

    return kInterfaceVersionMax;
}

} // namespace common_serialization
