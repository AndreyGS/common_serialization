/**
 * @file MessagingTests.cpp
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

namespace
{

using namespace common_serialization;
using namespace ft_helpers;

const csp::service_structs::CspPartySettings<>& getServerSettings()
{
    static csp::service_structs::CspPartySettings<> serverSettings;

    if (serverSettings.protocolVersions.size() == 0)
    {
        serverSettings.protocolVersions.pushBackN(csp::traits::kProtocolVersions, csp::traits::getProtocolVersionsCount());

        serverSettings.interfaces.pushBack({ interface_for_test::properties.id, interface_for_test::properties.version });
        serverSettings.interfaces.pushBack({ descendant_interface::properties.id, descendant_interface::properties.version });
        serverSettings.interfaces.pushBack({ another_yet_interface::properties.id, another_yet_interface::properties.version });
    }

    return serverSettings;
}

TEST(MessagingTests, InitCommonServerT)
{
    csp::messaging::CommonServer commonServer(getServerSettings());
    EXPECT_TRUE(commonServer.isValid());
    EXPECT_EQ(commonServer.init({}), Status::kErrorInvalidArgument);
    EXPECT_FALSE(commonServer.isValid());
    EXPECT_EQ(commonServer.init(getServerSettings()), Status::kNoError);
    EXPECT_TRUE(commonServer.isValid());
}

TEST(MessagingTests, InitDataClientT)
{
    csp::service_structs::CspPartySettings<> serverSettings;
    serverSettings.protocolVersions.pushBack(2);
    serverSettings.protocolVersions.pushBack(1);
    serverSettings.mandatoryCommonFlags = csp::context::CommonFlags::kBigEndianFormat;
    serverSettings.forbiddenCommonFlags = csp::context::CommonFlags::kBitness32;
    serverSettings.interfaces.pushBack({ another_yet_interface::properties.id, another_yet_interface::properties.version });
    csp::messaging::CommonServer commonServer(serverSettings);

    interface_for_test::SimplyAssignableAlignedToOne<> dummyInput;
    interface_for_test::SimplyAssignableDescendant<> dummyOutput;

    csp::messaging::DataClient dataClient(new SimpleSpeaker{ commonServer });

    EXPECT_TRUE(dataClient.getDataClientSpeaker());

    Vector<csp::protocol_version_t> clientProtocolVersions;
    clientProtocolVersions.pushBack(1);
    clientProtocolVersions.pushBack(0); // 0 is not a valid CSP version and we add it here only for logic test
    csp::context::CommonFlags clientMandatoryCommonFlags{ csp::context::CommonFlags::kEndiannessDifference };
    csp::context::CommonFlags clientForbiddenCommonFlags;
    Vector<csp::service_structs::InterfaceVersion<>> clientInterfaces;
    clientInterfaces.pushBack({ interface_for_test::properties.id, interface_for_test::properties.version });
    clientInterfaces.pushBack({ descendant_interface::properties.id, descendant_interface::properties.version });
    clientInterfaces.pushBack({ another_yet_interface::properties.id, another_yet_interface::properties.version });
    csp::service_structs::CspPartySettings<> serverSettingsReturned;

    // Valid test 1
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kNoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), clientProtocolVersions[0]);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), serverSettings.mandatoryCommonFlags | clientMandatoryCommonFlags);
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), serverSettings.forbiddenCommonFlags | clientForbiddenCommonFlags);
    EXPECT_EQ(dataClient.getInterfaces().size(), 1);
    EXPECT_EQ(dataClient.getInterfaces()[0], clientInterfaces[2]);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientInterfaces[2].id), 0);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientInterfaces[1].id), csp::traits::kInterfaceVersionUndefined);

    // Valid test 2
    clientMandatoryCommonFlags = csp::context::CommonFlags{};
    clientForbiddenCommonFlags = csp::context::CommonFlags::kEndiannessDifference;
    serverSettingsReturned.clear();
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kNoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient.isValid());
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), serverSettings.forbiddenCommonFlags | clientForbiddenCommonFlags);

    // No supported interfaces
    clientInterfaces.erase(2);
    serverSettingsReturned.clear();
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kErrorNoSupportedInterfaces);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), csp::traits::kProtocolVersionUndefined);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getInterfaces().size(), 0);
    EXPECT_EQ(dataClient.handleData(dummyInput, dummyOutput), Status::kErrorNotInited);

    // Not compatible common flags settings (between mandatory and forbidden flags)
    // First, reinit dataClient (to ensure that later it will be reset to not valid state)
    clientInterfaces.pushBack({ another_yet_interface::properties.id, another_yet_interface::properties.version });
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kNoError);
    EXPECT_TRUE(dataClient.isValid());

    clientMandatoryCommonFlags = serverSettings.forbiddenCommonFlags;
    serverSettingsReturned.clear();
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), csp::traits::kProtocolVersionUndefined);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getInterfaces().size(), 0);
    EXPECT_EQ(dataClient.handleData(dummyInput, dummyOutput), Status::kErrorNotInited);

    // Not supported protocol version
    // First, reinit dataClient (to ensure that later it will be reset to not valid state)
    clientMandatoryCommonFlags = csp::context::CommonFlags{};
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kNoError);
    EXPECT_TRUE(dataClient.isValid());

    clientProtocolVersions.erase(0);
    serverSettingsReturned.clear();
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kErrorNotSupportedProtocolVersion);
    EXPECT_EQ(serverSettingsReturned.protocolVersions, serverSettings.protocolVersions);
    EXPECT_FALSE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), csp::traits::kProtocolVersionUndefined);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getInterfaces().size(), 0);
    EXPECT_EQ(dataClient.handleData(dummyInput, dummyOutput), Status::kErrorNotInited);

    // Test of init when data client speaker is not valid
    clientProtocolVersions.insert(0, 0);
    dynamic_cast<SimpleSpeaker*>(dataClient.getDataClientSpeaker().get())->setValidState(false);
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kErrorNotInited);
    EXPECT_FALSE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), csp::traits::kProtocolVersionUndefined);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getInterfaces().size(), 0);
    EXPECT_EQ(dataClient.handleData(dummyInput, dummyOutput), Status::kErrorNotInited);

    // Test of init when DataClient has no speaker
    auto& speaker = dataClient.getDataClientSpeaker();
    delete speaker.release();
    EXPECT_EQ(dataClient.init(clientProtocolVersions, clientMandatoryCommonFlags, clientForbiddenCommonFlags, clientInterfaces, &serverSettingsReturned), Status::kErrorNotInited);
    EXPECT_FALSE(dataClient.isValid());
    EXPECT_EQ(dataClient.getProtocolVersion(), csp::traits::kProtocolVersionUndefined);
    EXPECT_EQ(dataClient.getMandatoryCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getForbiddenCommonFlags(), csp::context::CommonFlags{});
    EXPECT_EQ(dataClient.getInterfaces().size(), 0);
    EXPECT_EQ(dataClient.handleData(dummyInput, dummyOutput), Status::kErrorNotInited);
}

TEST(MessagingTests, DataMessageHandling)
{
    csp::messaging::CommonServer commonServer(getServerSettings());
    FirstDataServer firstDataServer;

    csp::messaging::DataClient dataClient(new SimpleSpeaker{ commonServer });

    // Standard processing
    Vector<csp::service_structs::InterfaceVersion<>> clientInterfaces;
    clientInterfaces.pushBack({ interface_for_test::properties.id, interface_for_test::properties.version });

    EXPECT_EQ(dataClient.init(csp::traits::getLatestProtocolVersion(), {}, {}, clientInterfaces), Status::kNoError);

    interface_for_test::SimplyAssignableAlignedToOne<> input;
    fillingStruct(input);
    interface_for_test::SimplyAssignableDescendant<> output;

    EXPECT_EQ(dataClient.handleData(input, output), Status::kNoError);

    interface_for_test::SimplyAssignableDescendant<> outputReference;
    fillingStruct(outputReference);

    EXPECT_EQ(output, outputReference);

    // Legacy interface versions
    clientInterfaces[0].version = 1;
    dataClient.init(csp::traits::getLatestProtocolVersion(), {}, {}, clientInterfaces);

    EXPECT_EQ(dataClient.getInterfaceVersion(clientInterfaces[0].id), 1);

    interface_for_test::SimplyAssignableDescendant<> output2;

    EXPECT_EQ(dataClient.handleData(input, output2), Status::kNoError);
    EXPECT_EQ(output2, outputReference);

    // Struct handler not support interface version
    clientInterfaces[0].version = 0;
    dataClient.init(csp::traits::getLatestProtocolVersion(), {}, {}, clientInterfaces);

    EXPECT_EQ(dataClient.getInterfaceVersion(clientInterfaces[0].id), 0);

    interface_for_test::SimplyAssignableDescendant<> output3;

    EXPECT_EQ(dataClient.handleData(input, output3), Status::kErrorNotSupportedInterfaceVersion);
    EXPECT_EQ(output3.m_d, 0); // struct wasn't changed
}

} // namespace anonymous
