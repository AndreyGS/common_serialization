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
using namespace common_serialization::csp::messaging::service_structs;

CspPartySettings<> getValidCspPartySettings()
{
    CspPartySettings settings;
    settings.protocolVersions.pushBackN(traits::kProtocolVersions, traits::getProtocolVersionsCount());

    settings.interfaces.pushBack(InterfaceVersion{ interface_for_test::properties });
    settings.interfaces.pushBack(InterfaceVersion{ descendant_interface::properties });
    settings.interfaces.pushBack(InterfaceVersion{ another_yet_interface::properties });

    return settings;
}

CspPartySettings<> getInvalidCspPartySettings()
{
    CspPartySettings settings = getValidCspPartySettings();

    settings.mandatoryCommonFlags.addFlags(context::CommonFlags::kBigEndianFormat);
    settings.forbiddenCommonFlags.addFlags(context::CommonFlags::kBigEndianFormat);

    return settings;
}

BinVectorT getNotSupportedProtocolVersionsWithInvalidOutput()
{
    BinVectorT binOutput;
    BinVectorT protocolVersions;
    protocolVersions.pushBack(kNotSupportedCspVersion);
    processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, protocolVersions, {});

    return binOutput;
}

BinVectorT getNotSupportedProtocolVersionsWithValidOutput()
{
    BinVectorT binOutput;
    BinVectorT protocolVersions;
    protocolVersions.pushBackN(csp::traits::kProtocolVersions, csp::traits::getProtocolVersionsCount());
    processing::serializeStatusErrorNotSupportedProtocolVersion(binOutput, protocolVersions, {});

    return binOutput;
}

BinVectorT getSettingsWithBigEndian()
{
    BinVectorT binOutput;
    context::SData ctxOut(binOutput, 1, context::CommonFlags{ context::CommonFlags::kBigEndianFormat }, {}, false, 1);

    CspPartySettings<> settings = getValidCspPartySettings();
    settings.mandatoryCommonFlags.addFlags(context::CommonFlags::kBigEndianFormat);

    settings.serialize(ctxOut);
    return binOutput;
}

} // namespace
