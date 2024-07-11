/**
 * @file UnitTests/Tests/csp/messaging/ClientTests.cpp
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

#include "Tests/csp/messaging/Helpers.h"
#include "Tests/csp/messaging/ClientSpeakerMock.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

namespace
{

using namespace ft_helpers;

class ClientTests : public ::testing::Test
{
public:
    ClientTests() : m_speaker(), m_client(m_speaker) {}

    void init() {}

protected:
    ClientSpeakerMock m_speaker;
    Client m_client;
    ISerializableDummy m_dummy;
};

TEST_F(ClientTests, Init1)
{
    
    // Try to pass invalid settings to Client
    CspPartySettings settings = getInvalidCspPartySettings();
    EXPECT_EQ(m_client.init(settings), Status::ErrorInvalidArgument);
    EXPECT_EQ(m_client.isValid(), false);

    // Try to pass valid settings to Client
    settings = getValidCspPartySettings();
    EXPECT_EQ(m_client.init(settings), Status::NoError);

    // Try to pass valid settings to inited Client
    EXPECT_EQ(m_client.init(settings), Status::ErrorAlreadyInited);
}

TEST_F(ClientTests, Init2)
{
    // Try to pass invalid settings to Client
    CspPartySettings clientSettings = getInvalidCspPartySettings();
    CspPartySettings serverSettings;
    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorInvalidArgument);
    EXPECT_EQ(m_client.isValid(), false);

    // Try to pass valid settings to Client
    clientSettings = getForbiddenBigEndianCspPartySettings();
    
        // Test 1: Server is not supported CSP version used by Client
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedProtocolVersionsWithInvalidOutput());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorNotSupportedProtocolVersion);
    EXPECT_EQ(m_client.isValid(), false);

        // Test 2: Server settings are not compatible with Client one

    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedProtocolVersionsWithValidOutput());
            return Status::NoError;
        }
                                )).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinSettingsWithMandatoryBigEndian());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(m_client.isValid(), false);

    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedProtocolVersionsWithValidOutput());
            return Status::NoError;
        }
                                )).WillOnce(Invoke(
        [&clientSettings](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            clientSettings.serialize(binOutput.getVector());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::NoError);
    EXPECT_EQ(m_client.isValid(), true);

    // Try to pass valid settings to inited Client
    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorAlreadyInited);
}

TEST_F(ClientTests, GetClientSpeaker)
{
    EXPECT_EQ(&m_speaker, &m_client.getClientSpeaker());
}

TEST_F(ClientTests, GetServerProtocolVersions)
{
    RawVectorT<protocol_version_t> protocolList;

    // Test valid output from Server
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedProtocolVersionsWithExtraOutput());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerProtocolVersions(protocolList), Status::NoError);
    EXPECT_EQ(protocolList, getExtendedProtocolVersionsList());

    // Test invalid output from Server
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedInterfaceVersionValid(ISerializableDummy::getId(), 1));
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerProtocolVersions(protocolList), Status::ErrorDataCorrupted);
}

TEST_F(ClientTests, GetServerSettings)
{
    CspPartySettings outputSettings;
    CspPartySettings serverSettings = getValidCspPartySettings();

    // Test valid output from Server
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [&serverSettings](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            serverSettings.serialize(binOutput.getVector());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::NoError);
    EXPECT_EQ(outputSettings, serverSettings);

    // Test error from speaker
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {

            return Status::ErrorDataCorrupted;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::ErrorDataCorrupted);

    // Test bad data from Server
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getBinNotSupportedInterfaceVersionValid(ISerializableDummy::getId(), 1));
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::ErrorMismatchOfStructId);
}

TEST_F(ClientTests, GetServerHandlerSettings)
{
    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    // Client not inited yet test
    EXPECT_EQ(m_client.getServerHandlerSettings<interface_for_test::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNotInited);

    m_client.init(getValidCspPartySettings());

    // Try to get info about struct that is part of interface that isn't present in Client settings
    EXPECT_EQ(m_client.getServerHandlerSettings<with_std_included_interface::OneBigType<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNotSupportedInterface);

    // Server return wrong message
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            getValidCspPartySettings().serialize(binOutput.getVector());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<interface_for_test::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorDataCorrupted);

    // Server haven't handler for this struct (or return any other error except ErrorNotSupportedInterfaceVersion)
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            processing::serializeStatusFullContext(binOutput.getVector(), getLatestProtocolVersion(), {}, Status::ErrorNoSuchHandler);
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<interface_for_test::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNoSuchHandler);
    
    // Server have handler and returns its attributes
    constexpr interface_version_t expectedMinimumVersion{ 5 };
    constexpr Id expectedOutputTypeId = interface_for_test::SpecialProcessingType<>::getId();

    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [expectedMinimumVersion, &expectedOutputTypeId](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            processing::serializeStatusErrorNotSupportedInterfaceVersion(
                  getLatestProtocolVersion()
                , {}
                , expectedMinimumVersion
                , expectedOutputTypeId
                , binOutput.getVector());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<interface_for_test::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::NoError);
    EXPECT_EQ(minimumInterfaceVersion, expectedMinimumVersion);
    EXPECT_EQ(outputTypeId, expectedOutputTypeId);
}

TEST_F(ClientTests, GetSettings)
{
    // Try with not initialized Client
    EXPECT_EQ(m_client.getSettings(), CspPartySettings<>());

    CspPartySettings settings = getValidCspPartySettings();

    m_client.init(settings);

    // After initialize
    EXPECT_EQ(m_client.getSettings(), settings);
}

TEST_F(ClientTests, GetInterfaceVersion)
{
    // Try with not initialized Client
    EXPECT_EQ(m_client.getInterfaceVersion(interface_for_test::properties.id), kInterfaceVersionUndefined);

    CspPartySettings settings = getValidCspPartySettings();

    m_client.init(settings);

    // After initialize
    EXPECT_EQ(m_client.getInterfaceVersion(interface_for_test::properties.id), interface_for_test::properties.version);
}

TEST_F(ClientTests, HandleData)
{
    interface_for_test::SimplyAssignableAlignedToOne<> input;
    fillingStruct(input);
    interface_for_test::SimplyAssignableDescendant<> output;

    // Try with not initialized Client
    EXPECT_EQ((m_client.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output)), Status::ErrorNotInited);

}

} // namespace