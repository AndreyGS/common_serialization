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

#include <thread>

namespace
{

using namespace common_serialization;
using namespace ft_helpers;

csp::service_structs::CspPartySettings<> getServerSettings()
{
    csp::service_structs::CspPartySettings<> serverSettings;

    serverSettings.protocolVersions.pushBackN(csp::traits::kProtocolVersions, csp::traits::getProtocolVersionsCount());

    serverSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ interface_for_test::properties });
    serverSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ descendant_interface::properties });
    serverSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ another_yet_interface::properties });

    return serverSettings;
}

TEST(MessagingTests, InitCommonServerT)
{
    csp::messaging::Server commonServer;
    EXPECT_EQ(commonServer.init<csp::messaging::GenericServerDataHandlerRegistrar>(getServerSettings()), Status::kNoError);
    EXPECT_TRUE(commonServer.isValid());
    EXPECT_EQ(commonServer.init<csp::messaging::GenericServerDataHandlerRegistrar>(getServerSettings()), Status::kErrorAlreadyInited);
    EXPECT_TRUE(commonServer.isValid());
    csp::messaging::Server commonServer2;
    EXPECT_EQ(commonServer2.init<csp::messaging::GenericServerDataHandlerRegistrar>({}), Status::kErrorInvalidArgument);
    EXPECT_FALSE(commonServer2.isValid());
}

TEST(MessagingTests, InitDataClientT)
{
    csp::service_structs::CspPartySettings<> serverSettings;
    serverSettings.protocolVersions.pushBack(2);
    serverSettings.protocolVersions.pushBack(1);
    serverSettings.mandatoryCommonFlags = csp::context::CommonFlags::kBigEndianFormat;
    serverSettings.forbiddenCommonFlags = csp::context::CommonFlags::kBitness32;
    serverSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ another_yet_interface::properties });
    csp::messaging::Server commonServer;
    commonServer.init<csp::messaging::GenericServerDataHandlerRegistrar>(serverSettings);

    interface_for_test::SimplyAssignableAlignedToOne<> dummyInput;
    interface_for_test::SimplyAssignableDescendant<> dummyOutput;

    SimpleSpeaker simpleSpeaker{ commonServer };
    csp::messaging::Client dataClient(simpleSpeaker);

    csp::service_structs::CspPartySettings<> clientSettings;
    clientSettings.protocolVersions.pushBack(1);
    clientSettings.protocolVersions.pushBack(0); // 0 is not a valid CSP version and we add it here only for logic test
    clientSettings.mandatoryCommonFlags = csp::context::CommonFlags::kEndiannessDifference;
    clientSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ interface_for_test::properties });
    clientSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ descendant_interface::properties });
    clientSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ another_yet_interface::properties });
    csp::service_structs::CspPartySettings<> serverSettingsReturned;

    // Valid test 1
    EXPECT_EQ(dataClient.init(clientSettings, serverSettingsReturned), Status::kNoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient.isValid());
    EXPECT_EQ(dataClient.getSettings().protocolVersions[0], clientSettings.protocolVersions[0]);
    EXPECT_EQ(dataClient.getSettings().mandatoryCommonFlags, serverSettings.mandatoryCommonFlags | clientSettings.mandatoryCommonFlags);
    EXPECT_EQ(dataClient.getSettings().forbiddenCommonFlags, serverSettings.forbiddenCommonFlags | clientSettings.forbiddenCommonFlags);
    EXPECT_EQ(dataClient.getSettings().interfaces.size(), 1);
    EXPECT_EQ(dataClient.getSettings().interfaces[0], clientSettings.interfaces[2]);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientSettings.interfaces[2].id), 0);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientSettings.interfaces[1].id), csp::traits::kInterfaceVersionUndefined);
    EXPECT_EQ(dataClient.init(clientSettings, serverSettingsReturned), Status::kErrorAlreadyInited);
    EXPECT_EQ(dataClient.init(clientSettings), Status::kErrorAlreadyInited);

    // Valid test 2
    clientSettings.mandatoryCommonFlags = csp::context::CommonFlags{};
    clientSettings.forbiddenCommonFlags = csp::context::CommonFlags::kEndiannessDifference;
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient2(simpleSpeaker);
    EXPECT_EQ(dataClient2.init(clientSettings, serverSettingsReturned), Status::kNoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient2.isValid());
    EXPECT_EQ(dataClient2.getSettings().forbiddenCommonFlags, serverSettings.forbiddenCommonFlags | clientSettings.forbiddenCommonFlags);

    // No supported interfaces
    clientSettings.interfaces.erase(2);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient3(simpleSpeaker);
    EXPECT_EQ(dataClient3.init(clientSettings, serverSettingsReturned), Status::kErrorNoSupportedInterfaces);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient3.isValid());
    EXPECT_EQ((dataClient3.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::kErrorNotInited);

    // Not compatible common flags settings (between mandatory and forbidden flags)
    // First, reinit dataClient (to ensure that later it will be reset to not valid state)
    clientSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ another_yet_interface::properties });
    csp::messaging::Client dataClient4(simpleSpeaker);
    EXPECT_EQ(dataClient4.init(clientSettings, serverSettingsReturned), Status::kNoError);
    EXPECT_TRUE(dataClient4.isValid());

    clientSettings.mandatoryCommonFlags = serverSettings.forbiddenCommonFlags;
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient5(simpleSpeaker);
    EXPECT_EQ(dataClient5.init(clientSettings, serverSettingsReturned), Status::kErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient5.isValid());
    EXPECT_EQ((dataClient5.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::kErrorNotInited);

    // Not supported protocol version
    // First, reinit dataClient (to ensure that later it will be reset to not valid state)
    clientSettings.mandatoryCommonFlags = csp::context::CommonFlags{};
    csp::messaging::Client dataClient6(simpleSpeaker);
    EXPECT_EQ(dataClient6.init(clientSettings, serverSettingsReturned), Status::kNoError);
    EXPECT_TRUE(dataClient6.isValid());

    clientSettings.protocolVersions.erase(0);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient7(simpleSpeaker);
    EXPECT_EQ(dataClient7.init(clientSettings, serverSettingsReturned), Status::kErrorNotSupportedProtocolVersion);
    EXPECT_EQ(serverSettingsReturned.protocolVersions, serverSettings.protocolVersions);
    EXPECT_FALSE(dataClient7.isValid());
    EXPECT_EQ((dataClient7.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::kErrorNotInited);

    // Test of init when data client speaker is not valid
    clientSettings.protocolVersions.insert(0, 0);
    simpleSpeaker.setValidState(false);
    csp::messaging::Client dataClient8(simpleSpeaker);
    EXPECT_EQ(dataClient8.init(clientSettings, serverSettingsReturned), Status::kErrorNotInited);
    EXPECT_FALSE(dataClient8.isValid());
    EXPECT_EQ((dataClient8.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::kErrorNotInited);
}

TEST(MessagingTests, DataMessageHandling)
{
    csp::messaging::Server commonServer;
    commonServer.init<csp::messaging::GenericServerDataHandlerRegistrar>(getServerSettings());
    FirstCspService* pFirstCspService = new FirstCspService;
    pFirstCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());

    SimpleSpeaker simpleSpeaker{ commonServer };
    csp::messaging::Client dataClient(simpleSpeaker);
    csp::service_structs::CspPartySettings<> clientSettings;
    clientSettings.protocolVersions.pushBack(csp::traits::getLatestProtocolVersion());
    clientSettings.interfaces.pushBack(csp::service_structs::InterfaceVersion{ interface_for_test::properties });

    EXPECT_EQ(dataClient.init(clientSettings), Status::kNoError);

    interface_for_test::SimplyAssignableAlignedToOne<> input;
    fillingStruct(input);
    interface_for_test::SimplyAssignableDescendant<> output;

    EXPECT_EQ((dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output)), Status::kNoError);

    interface_for_test::SimplyAssignableDescendant<> outputReference;
    fillingStruct(outputReference);

    EXPECT_EQ(output, outputReference);

    // Multicast test
    SecondCspService* pSecondCspService = new SecondCspService;
    pSecondCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
    interface_for_test::SimplyAssignable<> input2;
    fillingStruct(input2);

    csp::service_structs::ISerializableDummy<> outputDummy;

    ft_helpers::numberOfMultiEntrances = 0;
    EXPECT_EQ((dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignable<>, csp::service_structs::ISerializableDummy<>>>(input2, outputDummy)), Status::kNoError);
    EXPECT_EQ(ft_helpers::numberOfMultiEntrances, 2);
    
    /// Stress test
    std::thread test1([&]
        {
            interface_for_test::Diamond<> d;
            interface_for_test::DynamicPolymorphic<> d2;


            for (size_t i = 0; i < 10000; ++i)
            {
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignable<>, csp::service_structs::ISerializableDummy<>>>(input2, outputDummy);
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output);
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>(d, d2);
            }
        });

    std::thread test2([&]
        {
            interface_for_test::Diamond<> d;
            interface_for_test::DynamicPolymorphic<> d2;


            for (size_t i = 0; i < 10000; ++i)
            {
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignable<>, csp::service_structs::ISerializableDummy<>>>(input2, outputDummy);
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>(d, d2);
                dataClient.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output);
            }
        });
        
    std::thread test3([&]
        {
            for (size_t i = 0; i < 200; ++i)
            {
                pFirstCspService->unregisterService(*commonServer.getDataHandlersRegistrar());
                delete pFirstCspService;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                pFirstCspService = new FirstCspService;
                pFirstCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->unregisterSimplyAssignableAlignedToOne(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->unregisterSimplyAssignableAlignedToOne(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->unregisterDiamond(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->unregisterSimplyAssignable(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->unregisterService(*commonServer.getDataHandlersRegistrar());
                pFirstCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
            }
        });

    std::thread test4([&]
        {
            for (size_t i = 0; i < 200; ++i)
            {
                pSecondCspService->unregisterService(*commonServer.getDataHandlersRegistrar());
                delete pSecondCspService;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                pSecondCspService = new SecondCspService;
                pSecondCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
                pSecondCspService->unregisterService(*commonServer.getDataHandlersRegistrar());
                pSecondCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
            }
        });

    test1.join();
    test2.join();
    test3.join();
    test4.join();

    // Legacy interface versions
    clientSettings.interfaces[0].version = 1;
    csp::messaging::Client dataClient2(simpleSpeaker);
    dataClient2.init(clientSettings);

    EXPECT_EQ(dataClient2.getInterfaceVersion(clientSettings.interfaces[0].id), 1);

    interface_for_test::SimplyAssignableDescendant<> output2;

    EXPECT_EQ((dataClient2.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output2)), Status::kNoError);
    EXPECT_EQ(output2, outputReference);

    // Struct handler not support interface version
    clientSettings.interfaces[0].version = 0;
    csp::messaging::Client dataClient3(simpleSpeaker);
    dataClient3.init(clientSettings);

    EXPECT_EQ(dataClient3.getInterfaceVersion(clientSettings.interfaces[0].id), 0);

    interface_for_test::SimplyAssignableDescendant<> output3;

    EXPECT_EQ((dataClient3.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output3)), Status::kErrorNotSupportedInterfaceVersion);
    EXPECT_EQ(output3.m_d, 0); // struct wasn't changed
}

} // namespace
