/**
 * @file UnitTests/Tests/csp/messaging/Helpers.h
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

namespace
{

using namespace common_serialization;
using namespace common_serialization::csp;
using namespace common_serialization::csp::context;
using namespace common_serialization::csp::messaging;
using namespace common_serialization::csp::messaging::service_structs;
using namespace common_serialization::csp::traits;

constexpr protocol_version_t kValidProtocolVersion = 1;
constexpr protocol_version_t kInvalidProtocolVersion = kProtocolVersions[getProtocolVersionsCount() - 1] + 1;

CspPartySettings<> getValidCspPartySettings()
{
    RawVectorT<protocol_version_t> protocolVersions;
    protocolVersions.pushBackN(traits::kProtocolVersions, traits::getProtocolVersionsCount());
    RawVectorT<InterfaceVersion<>> interfaces;
    interfaces.pushBack(InterfaceVersion{ interface_for_test::properties });
    interfaces.pushBack(InterfaceVersion{ descendant_interface::properties });
    interfaces.pushBack(InterfaceVersion{ another_yet_interface::properties });

    CspPartySettings settings(protocolVersions, {}, {}, interfaces);

    return settings;
}

CspPartySettings<> getInvalidCspPartySettings()
{
    CspPartySettings settings = getValidCspPartySettings();
    settings.init(
          settings.getProtocolVersions()
        , CommonFlags{ CommonFlags::kBigEndianFormat }
        , CommonFlags{ CommonFlags::kBigEndianFormat }
        , settings.getInterfaces());

    return settings;
}

CspPartySettings<> getMandatoryBigEndianCspPartySettings()
{
    CspPartySettings settings = getValidCspPartySettings();
    settings.init(
          settings.getProtocolVersions()
        , CommonFlags{ CommonFlags::kBigEndianFormat }
        , {}
        , settings.getInterfaces());

    return settings;
}

CspPartySettings<> getForbiddenBigEndianCspPartySettings()
{
    CspPartySettings settings = getValidCspPartySettings();
    settings.init(
          settings.getProtocolVersions()
        , {}
        , CommonFlags{ CommonFlags::kBigEndianFormat }
        , settings.getInterfaces());

    return settings;
}

CspPartySettings<> getInterfaceVersionZeroCspPartySettings()
{
    RawVectorT<protocol_version_t> protocolVersions;
    protocolVersions.pushBackN(traits::kProtocolVersions, traits::getProtocolVersionsCount());
    RawVectorT<InterfaceVersion<>> interfaces;
    interfaces.pushBack(InterfaceVersion{ interface_for_test::properties.id, 0 } );
    CspPartySettings settings(protocolVersions, {}, {}, interfaces);

    return settings;
}

RawVectorT<protocol_version_t> getExtendedProtocolVersionsList()
{
    RawVectorT<protocol_version_t> protocolVersions;
    protocolVersions.pushBackN(kProtocolVersions, getProtocolVersionsCount());
    protocolVersions.pushBack(kInvalidProtocolVersion);

    return protocolVersions;
}

BinVectorT getBinNotSupportedProtocolVersionsWithInvalidOutput()
{
    BinVectorT binOutput;
    RawVectorT<protocol_version_t> protocolVersions;
    protocolVersions.pushBack(kInvalidProtocolVersion);
    processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, protocolVersions, {});

    return binOutput;
}

BinVectorT getBinNotSupportedProtocolVersionsWithValidOutput()
{
    BinVectorT binOutput;
    RawVectorT<protocol_version_t> protocolVersions;
    protocolVersions.pushBackN(kProtocolVersions, getProtocolVersionsCount());
    processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, protocolVersions, {});

    return binOutput;
}

BinVectorT getBinNotSupportedProtocolVersionsWithExtraOutput()
{
    BinVectorT binOutput;
    RawVectorT<protocol_version_t> protocolVersions = getExtendedProtocolVersionsList();
    processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, protocolVersions, {});

    return binOutput;
}

BinVectorT getBinSettingsWithMandatoryBigEndian()
{
    BinVectorT binOutput;
    SData ctxOut(binOutput, 1, CommonFlags{ CommonFlags::kBigEndianFormat }, {}, false, 1);

    CspPartySettings<> settings = getMandatoryBigEndianCspPartySettings();
    settings.serialize(ctxOut);
    return binOutput;
}

BinVectorT getBinSettingsWithForbiddenBigEndian()
{
    BinVectorT binOutput;
    SData ctxOut(binOutput, 1, CommonFlags{ CommonFlags::kBigEndianFormat }, {}, false, 1);

    CspPartySettings<> settings = getForbiddenBigEndianCspPartySettings();
    settings.serialize(ctxOut);
    return binOutput;
}

BinVectorT getBinNotSupportedInterfaceVersionValid(const Id& id, interface_version_t minimumInterfaceVersion)
{
    BinVectorT binOutput;
    processing::serializeStatusErrorNotSupportedInterfaceVersion(getLatestProtocolVersion(), {}, minimumInterfaceVersion, id, binOutput);

    return binOutput;
}

} // namespace
