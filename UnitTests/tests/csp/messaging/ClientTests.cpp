/**
 * @file UnitTests/tests/csp/messaging/ClientTests.cpp
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

#include <tests/csp/messaging/Helpers.h>
#include <tests/csp/messaging/ClientToServerCommunicatorMock.h>

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
    ClientTests() : m_clientToServerCommunicator(), m_client(m_clientToServerCommunicator) {}

protected:
    ClientToServerCommunicatorMock m_clientToServerCommunicator;
    Client m_client;
};

TEST_F(ClientTests, Init1InvalidSettings)
{
    EXPECT_EQ(m_client.init(getInvalidCspPartySettings()), Status::ErrorInvalidArgument);
    EXPECT_EQ(m_client.isValid(), false);
}

TEST_F(ClientTests, Init1ValidSettings)
{
    CspPartySettings settings = getValidCspPartySettings();
    EXPECT_EQ(m_client.init(settings), Status::NoError);
    EXPECT_EQ(m_client.isValid(), true);

    // Try to pass valid settings to inited Client
    EXPECT_EQ(m_client.init(settings), Status::ErrorAlreadyInited);
    EXPECT_EQ(m_client.isValid(), true);
}

TEST_F(ClientTests, Init2InvalidSettings)
{
    CspPartySettings serverSettings;
    EXPECT_EQ(m_client.init(getInvalidCspPartySettings(), serverSettings), Status::ErrorInvalidArgument);
    EXPECT_EQ(m_client.isValid(), false);
}

TEST_F(ClientTests, Init2ServerNotSupportClientCspVersion)
{
    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedProtocolVersionsWithInvalidOutput());
            return Status::NoError;
        }
    ));

    CspPartySettings serverSettings;
    EXPECT_EQ(m_client.init(getValidCspPartySettings(), serverSettings), Status::ErrorNotSupportedProtocolVersion);
    EXPECT_EQ(m_client.isValid(), false);
}

TEST_F(ClientTests, Init2ServerCspSettingsAreNotCompatibleWithClientOne)
{
    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedProtocolVersionsWithValidOutput());
            return Status::NoError;
        }
    )).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinSettingsWithMandatoryAnotherEndianness());
            return Status::NoError;
        }
    ));

    CspPartySettings serverSettings;
    EXPECT_EQ(m_client.init(getForbiddenAnotherEndiannessCspPartySettings(), serverSettings), Status::ErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(m_client.isValid(), false);
}

TEST_F(ClientTests, Init2ServerCspSettingsAreCompatibleWithClientOne)
{
    CspPartySettings clientSettings = getValidCspPartySettings();

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedProtocolVersionsWithValidOutput());
            return Status::NoError;
        }
    )).WillOnce(Invoke(
        [&clientSettings](const BinVectorT& input, BinVectorT& output)
        {
            clientSettings.serialize(output);
            return Status::NoError;
        }
    ));

    CspPartySettings serverSettings;
    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::NoError);
    EXPECT_EQ(m_client.isValid(), true);

    // Try to pass valid settings to inited Client
    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorAlreadyInited);
}

TEST_F(ClientTests, GetClientToServerCommunicator)
{
    EXPECT_EQ(&m_clientToServerCommunicator, &m_client.getClientToServerCommunicator());
}

TEST_F(ClientTests, GetServerProtocolVersionsInvalidServerOutput)
{
    RawVectorT<protocol_version_t> protocolList;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedInterfaceVersionValid(ISerializableDummy::getId(), 1));
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerProtocolVersions(protocolList), Status::ErrorDataCorrupted);
}

TEST_F(ClientTests, GetServerProtocolVersionsValidServerOutput)
{
    RawVectorT<protocol_version_t> protocolList;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedProtocolVersionsWithExtraOutput());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerProtocolVersions(protocolList), Status::NoError);
    EXPECT_EQ(protocolList, getExtendedProtocolVersionsList());
}

TEST_F(ClientTests, GetServerSettingsErrorFromSpeaker)
{
    CspPartySettings outputSettings;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {

            return Status::ErrorDataCorrupted;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::ErrorDataCorrupted);
}

TEST_F(ClientTests, GetServerSettingsBadDataFromServer)
{
    CspPartySettings outputSettings;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            output.init(getBinNotSupportedInterfaceVersionValid(ISerializableDummy::getId(), 1));
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::ErrorMismatchOfStructId);
}

TEST_F(ClientTests, GetServerSettingsValidServerOutput)
{
    CspPartySettings outputSettings;
    CspPartySettings serverSettings = getValidCspPartySettings();

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&serverSettings](const BinVectorT& input, BinVectorT& output)
        {
            serverSettings.serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerSettings(kValidProtocolVersion, outputSettings), Status::NoError);
    EXPECT_EQ(outputSettings, serverSettings);
}

TEST_F(ClientTests, GetServerHandlerSettingsClientNotInited)
{
    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    EXPECT_EQ(m_client.getServerHandlerSettings<tests_csp_interface::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNotInited);
}

// Try to get info about struct that is part of interface that isn't present in Client settings
TEST_F(ClientTests, GetServerHandlerSettingsInterfaceNotSupported)
{
    m_client.init(getValidCspPartySettings());

    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    EXPECT_EQ(m_client.getServerHandlerSettings<tests_csp_with_std_interface::OneBigType<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNotSupportedInterface);
}

TEST_F(ClientTests, GetServerHandlerSettingsErrorFromSpeaker)
{
    m_client.init(getValidCspPartySettings());

    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {

            return Status::ErrorDataCorrupted;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<tests_csp_interface::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorDataCorrupted);
}

TEST_F(ClientTests, GetServerHandlerSettingsWrongMessageTypeFromServer)
{
    m_client.init(getValidCspPartySettings());

    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            getValidCspPartySettings().serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.getServerHandlerSettings<tests_csp_interface::Diamond<>>(minimumInterfaceVersion, outputTypeId)), Status::ErrorDataCorrupted);
}

// Server haven't handler for this struct (or return any other error except ErrorNotSupportedInterfaceVersion)
TEST_F(ClientTests, GetServerHandlerSettingsServerDoesNotHaveHandlerForThisStruct)
{
    m_client.init(getValidCspPartySettings());

    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            processing::status::Helpers::serializeFullContext(output, getLatestProtocolVersion(), {}, Status::ErrorNoSuchHandler);
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<tests_csp_interface::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::ErrorNoSuchHandler);
}

TEST_F(ClientTests, GetServerHandlerSettingsValidReturnFromServer)
{
    m_client.init(getValidCspPartySettings());
    
    interface_version_t minimumInterfaceVersion{ 0 };
    Id outputTypeId;

    constexpr interface_version_t expectedMinimumVersion{ 5 };
    constexpr Id expectedOutputTypeId = tests_csp_interface::SpecialProcessingType<>::getId();

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [expectedMinimumVersion, &expectedOutputTypeId](const BinVectorT& input, BinVectorT& output)
        {
            processing::status::Helpers::serializeErrorNotSupportedInterfaceVersion(
                  getLatestProtocolVersion()
                , {}
                , expectedMinimumVersion
                , expectedOutputTypeId
                , output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.getServerHandlerSettings<tests_csp_interface::Diamond<>>(minimumInterfaceVersion, outputTypeId), Status::NoError);
    EXPECT_EQ(minimumInterfaceVersion, expectedMinimumVersion);
    EXPECT_EQ(outputTypeId, expectedOutputTypeId);
}

TEST_F(ClientTests, GetSettingsClientNotInited)
{
    EXPECT_EQ(m_client.getSettings(), CspPartySettings<>());
}

TEST_F(ClientTests, GetSettingsClientInited)
{
    CspPartySettings settings = getValidCspPartySettings();
    m_client.init(settings);
    EXPECT_EQ(m_client.getSettings(), settings);
}

TEST_F(ClientTests, GetInterfaceClientNotInited)
{
    EXPECT_EQ(m_client.getInterfaceVersion(tests_csp_interface::properties.id), kInterfaceVersionUndefined);
}

TEST_F(ClientTests, GetInterfaceClientInited)
{
    m_client.init(getValidCspPartySettings());
    EXPECT_EQ(m_client.getInterfaceVersion(tests_csp_interface::properties.id), tests_csp_interface::properties.version);
}

TEST_F(ClientTests, HandleDataClientNotInited)
{
    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorNotInited);
}

TEST_F(ClientTests, HandleDataClientNotSupportInterfaceVersionStruct)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_with_std_interface::OneBigType<> input;
    service_structs::ISerializableDummy output;

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_with_std_interface::OneBigType<>, service_structs::ISerializableDummy>>(
        input, output)), Status::ErrorNotSupportedInterface);
}

// Try with not supported interface version (too small for Input or Output type - smaller than its origin version)
TEST_F(ClientTests, HandleDataAnyStructHasInterfaceVersionHigherThanSettedUpForClient)
{
    m_client.init(getInterfaceVersionZeroCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;
    tests_csp_interface::SimplyAssignableFixedSize<> unsupportedVersionStruct;

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableFixedSize<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        unsupportedVersionStruct, output)), Status::ErrorNotSupportedInterfaceVersion);
    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableFixedSize<>>>(
        input, unsupportedVersionStruct)), Status::ErrorNotSupportedInterfaceVersion);
}

TEST_F(ClientTests, HandleDataNotCompatibleAdditionalCommonFlags)
{
    m_client.init(getForbiddenAnotherEndiannessCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output, CommonFlags{ CommonFlags::kBigEndianFormat }, {}, nullptr)), Status::ErrorNotCompatibleCommonFlagsSettings);
}

TEST_F(ClientTests, HandleDataUnmanagedPointersContainerNotSuppliedWhenNeed)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output, DataFlags{ DataFlags::kAllowUnmanagedPointers })), Status::ErrorInvalidArgument);
}

TEST_F(ClientTests, HandleDataContextFillingCheck)
{
    CspPartySettings settings = getMandatoryAnotherEndiannessCspPartySettings();
    m_client.init(settings);

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    constexpr CommonFlags additionalCommonFlags{ CommonFlags::kBitness32 };
    constexpr DataFlags additionalDataFlags{ DataFlags::kAlignmentMayBeNotEqual };

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&settings, additionalCommonFlags, effectiveDataFlags = input.getEffectiveMandatoryDataFlags(), additionalDataFlags](const BinVectorT& input, BinVectorT& output)
        {
            BinWalkerT dInput(input);
            DData ctx(dInput);
            tests_csp_interface::SimplyAssignableAlignedToOne<> data;
            EXPECT_EQ(data.deserialize(ctx), Status::NoError);

            EXPECT_EQ(ctx.getProtocolVersion(), settings.getLatestProtocolVersion());
            EXPECT_EQ(ctx.getCommonFlags(), settings.getMandatoryCommonFlags() | additionalCommonFlags);
            EXPECT_EQ(ctx.isHeapUsedForTemp(), true);
            EXPECT_EQ(ctx.getDataFlags(), effectiveDataFlags | additionalDataFlags);

            return Status::ErrorInternal;
        }
    ));

    m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output, additionalCommonFlags, additionalDataFlags);
}

TEST_F(ClientTests, HandleDataErrorFromSpeaker)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {

            return Status::ErrorOverflow;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorOverflow);
}

TEST_F(ClientTests, HandleDataBadCommonContextReceivedFromServer)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            SCommon ctx(output, kProtocolVersionUndefined, Message::Status);
            processing::common::ContextProcessor::serializeNoChecks(ctx);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorNotSupportedProtocolVersion);
}

TEST_F(ClientTests, HandleDataOutputCommonFlagsReceivedFromServerMismatchedWithInputOnes)
{
    m_client.init(getMandatoryAndForbiddenCommonFlagsCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            SCommon ctx(output, getLatestProtocolVersion(), Message::Status);
            processing::common::ContextProcessor::serialize(ctx);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorNotCompatibleCommonFlagsSettings);
}

TEST_F(ClientTests, HandleDataWrongMessageTypeReceivedFromServer)
{
    m_client.init(getMandatoryAnotherEndiannessCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            SCommon ctx(output, getLatestProtocolVersion(), Message::GetSettings, CommonFlags{ getAnotherEndianessPlatformFlags() });
            processing::common::ContextProcessor::serialize(ctx);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorDataCorrupted);
}

TEST_F(ClientTests, HandleDataDataFlagsReceivedFromServerMismatchedWithInputOnes)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&outputStruct = output](const BinVectorT& input, BinVectorT& output)
        {
            outputStruct.serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output, DataFlags{ DataFlags::kSimplyAssignableTagsOptimizationsAreTurnedOff })), Status::ErrorNotCompatibleDataFlagsSettings);
}

TEST_F(ClientTests, HandleDataWrongOutputTypeReceivedFromServer)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&outputStruct = input](const BinVectorT& input, BinVectorT& output)
        {
            outputStruct.serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorMismatchOfStructId);
}

TEST_F(ClientTests, HandleDataWrongStructInterfaceVersionReceivedFromServer)
{
    m_client.init(getInterfaceVersionZeroCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&outputStruct = output](const BinVectorT& input, BinVectorT& output)
        {
            outputStruct.serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorMismatchOfInterfaceVersions);
}

TEST_F(ClientTests, HandleDataGetValidDataFromServer)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&outputStruct = output](const BinVectorT& input, BinVectorT& output)
        {
            fillingStruct(outputStruct);
            outputStruct.serialize(output);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::NoError);
    
    tests_csp_interface::SimplyAssignableDescendant<> reference;
    fillingStruct(reference);
    EXPECT_EQ(reference, output);
}

TEST_F(ClientTests, HandleDataGetStatusFromServer)
{
    m_client.init(getValidCspPartySettings());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [](const BinVectorT& input, BinVectorT& output)
        {
            processing::status::Helpers::serializeFullContext(output, getLatestProtocolVersion(), {}, Status::ErrorNoSuchHandler);
            return Status::NoError;
        }
    ));

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(
        input, output)), Status::ErrorNoSuchHandler);
}

} // namespace
