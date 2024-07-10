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
#include "Tests/csp/messaging/ClientSpeakerMock.h"
#include "Tests/csp/messaging/Helpers.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

namespace
{

using namespace common_serialization;
using namespace ft_helpers;
using namespace csp::context;
using namespace csp::messaging;
using namespace csp::messaging::service_structs;
using namespace csp::traits;

class MessagingTests : public ::testing::Test
{
protected:
    csp::messaging::Server m_server;
    csp::messaging::Client m_client;
    std::unique_ptr<ClientSpeakerMock> m_clientSpeaker;
};


TEST(MessagingTests, InitCommonServerT)
{
    // Standard initialization
    Server commonServer;
    EXPECT_EQ(commonServer.init<GenericServerDataHandlerRegistrar>(getValidCspPartySettings()), Status::NoError);
    EXPECT_TRUE(commonServer.isValid());

    // Repeated initialization attempt
    EXPECT_EQ(commonServer.init<GenericServerDataHandlerRegistrar>(getValidCspPartySettings()), Status::ErrorAlreadyInited);
    EXPECT_TRUE(commonServer.isValid());

    // Init with invalid settings
    Server commonServer2;
    EXPECT_EQ(commonServer2.init<GenericServerDataHandlerRegistrar>({}), Status::ErrorInvalidArgument);
    EXPECT_FALSE(commonServer2.isValid());
}

TEST(MessagingTests, InitDataClientT)
{
    RawVectorT<protocol_version_t> serverProtocolVersions;
    serverProtocolVersions.pushBack(2);
    serverProtocolVersions.pushBack(1);
    RawVectorT<InterfaceVersion<>> serverInterfaces;
    serverInterfaces.pushBack(InterfaceVersion{ another_yet_interface::properties });

    CspPartySettings serverSettings(serverProtocolVersions, CommonFlags(CommonFlags::kBigEndianFormat), CommonFlags(CommonFlags::kBitness32), serverInterfaces);

    Server commonServer;
    commonServer.init<GenericServerDataHandlerRegistrar>(serverSettings);

    interface_for_test::SimplyAssignableAlignedToOne<> dummyInput;
    interface_for_test::SimplyAssignableDescendant<> dummyOutput;

    SimpleSpeaker simpleSpeaker{ commonServer };
    Client dataClient(simpleSpeaker);

    RawVectorT<protocol_version_t> clientProtocolVersions;
    clientProtocolVersions.pushBack(1);
    clientProtocolVersions.pushBack(0); // 0 is not a valid CSP version and we add it here only for logic test
    RawVectorT<InterfaceVersion<>> clientInterfaces;
    clientInterfaces.pushBack(InterfaceVersion{ interface_for_test::properties });
    clientInterfaces.pushBack(InterfaceVersion{ descendant_interface::properties });
    clientInterfaces.pushBack(InterfaceVersion{ another_yet_interface::properties });

    CspPartySettings clientSettings(clientProtocolVersions, CommonFlags(CommonFlags::kEndiannessDifference), {}, clientInterfaces);

    CspPartySettings serverSettingsReturned;

    // Valid test 1
    EXPECT_EQ(dataClient.init(clientSettings, serverSettingsReturned), Status::NoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient.isValid());
    EXPECT_EQ(dataClient.getSettings().getProtocolVersions()[0], clientSettings.getProtocolVersions()[0]);
    EXPECT_EQ(dataClient.getSettings().getMandatoryCommonFlags(), serverSettings.getMandatoryCommonFlags() | clientSettings.getMandatoryCommonFlags());
    EXPECT_EQ(dataClient.getSettings().getForbiddenCommonFlags(), serverSettings.getForbiddenCommonFlags() | clientSettings.getForbiddenCommonFlags());
    EXPECT_EQ(dataClient.getSettings().getInterfaces().size(), 1);
    EXPECT_EQ(dataClient.getSettings().getInterfaces()[0], clientSettings.getInterfaces()[2]);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientSettings.getInterfaces()[2].id), 0);
    EXPECT_EQ(dataClient.getInterfaceVersion(clientSettings.getInterfaces()[1].id), kInterfaceVersionUndefined);
    EXPECT_EQ(dataClient.init(clientSettings, serverSettingsReturned), Status::ErrorAlreadyInited);
    EXPECT_EQ(dataClient.init(clientSettings), Status::ErrorAlreadyInited);

    // Valid test 2
    clientSettings.init(clientProtocolVersions, {}, CommonFlags(CommonFlags::kEndiannessDifference), clientInterfaces);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient2(simpleSpeaker);
    EXPECT_EQ(dataClient2.init(clientSettings, serverSettingsReturned), Status::NoError);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_TRUE(dataClient2.isValid());
    EXPECT_EQ(dataClient2.getSettings().getForbiddenCommonFlags(), serverSettings.getForbiddenCommonFlags() | clientSettings.getForbiddenCommonFlags());

    // No supported interfaces
    clientInterfaces.erase(2);
    clientSettings.init(clientProtocolVersions, {}, CommonFlags(CommonFlags::kEndiannessDifference), clientInterfaces);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient3(simpleSpeaker);
    EXPECT_EQ(dataClient3.init(clientSettings, serverSettingsReturned), Status::ErrorNoSupportedInterfaces);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient3.isValid());
    EXPECT_EQ((dataClient3.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::ErrorNotInited);

    // Not compatible common flags settings (between mandatory and forbidden flags)
    clientInterfaces.pushBack(InterfaceVersion{ another_yet_interface::properties });
    clientSettings.init(clientProtocolVersions, serverSettings.getForbiddenCommonFlags(), clientSettings.getForbiddenCommonFlags(), clientInterfaces);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient4(simpleSpeaker);
    EXPECT_EQ(dataClient4.init(clientSettings, serverSettingsReturned), Status::ErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(serverSettingsReturned, serverSettings);
    EXPECT_FALSE(dataClient4.isValid());
    EXPECT_EQ((dataClient4.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::ErrorNotInited);

    // Not supported protocol version
    clientProtocolVersions.erase(0);
    clientSettings.init(clientProtocolVersions, {}, clientSettings.getForbiddenCommonFlags(), clientInterfaces);
    serverSettingsReturned.clear();
    csp::messaging::Client dataClient5(simpleSpeaker);
    EXPECT_EQ(dataClient5.init(clientSettings, serverSettingsReturned), Status::ErrorNotSupportedProtocolVersion);
    EXPECT_EQ(serverSettingsReturned.getProtocolVersions(), serverSettings.getProtocolVersions());
    EXPECT_FALSE(dataClient5.isValid());
    EXPECT_EQ((dataClient5.handleData<csp::messaging::CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(dummyInput, dummyOutput)), Status::ErrorNotInited);
}

TEST(MessagingTests, DataMessageHandling)
{
    Server commonServer;
    commonServer.init<GenericServerDataHandlerRegistrar>(getValidCspPartySettings());
    FirstCspService* pFirstCspService = new FirstCspService;
    pFirstCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());

    SimpleSpeaker simpleSpeaker{ commonServer };
    Client dataClient(simpleSpeaker);

    RawVectorT<protocol_version_t> clientProtocolVersions;
    clientProtocolVersions.pushBack(getLatestProtocolVersion());
    RawVectorT<InterfaceVersion<>> clientInterfaces;
    clientInterfaces.pushBack(InterfaceVersion{ interface_for_test::properties });
    CspPartySettings clientSettings(clientProtocolVersions, {}, {}, clientInterfaces);

    dataClient.init(clientSettings);

    interface_for_test::SimplyAssignableAlignedToOne<> input;
    fillingStruct(input);
    interface_for_test::SimplyAssignableDescendant<> output;

    // Unicast test
    EXPECT_EQ((dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output)), Status::NoError);

    interface_for_test::SimplyAssignableDescendant<> outputReference;
    fillingStruct(outputReference);

    EXPECT_EQ(output, outputReference);

    // Multicast test
    SecondCspService* pSecondCspService = new SecondCspService;
    pSecondCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());
    interface_for_test::SimplyAssignable<> input2;
    fillingStruct(input2);

    ISerializableDummy outputDummy;

    ft_helpers::numberOfMultiEntrances = 0;
    EXPECT_EQ((dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignable<>, ISerializableDummy>>(input2, outputDummy)), Status::NoError);
    EXPECT_EQ(ft_helpers::numberOfMultiEntrances, 2);
    
    /// Stress test
    std::thread test1([&]
        {
            interface_for_test::Diamond<> d;
            interface_for_test::DynamicPolymorphic<> d2;


            for (size_t i = 0; i < 10000; ++i)
            {
                dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignable<>, ISerializableDummy>>(input2, outputDummy);
                dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output);
                dataClient.handleData<CdhHeap<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>(d, d2);
            }
        });

    std::thread test2([&]
        {
            interface_for_test::Diamond<> d;
            interface_for_test::DynamicPolymorphic<> d2;


            for (size_t i = 0; i < 10000; ++i)
            {
                dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignable<>, ISerializableDummy>>(input2, outputDummy);
                dataClient.handleData<CdhHeap<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>(d, d2);
                dataClient.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output);
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
    clientInterfaces[0].version = 1;
    clientSettings.init(clientProtocolVersions, {}, {}, clientInterfaces);

    Client dataClient2(simpleSpeaker);
    dataClient2.init(clientSettings);

    EXPECT_EQ(dataClient2.getInterfaceVersion(clientSettings.getInterfaces()[0].id), 1);

    interface_for_test::SimplyAssignableDescendant<> output2;

    EXPECT_EQ((dataClient2.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output2)), Status::NoError);
    EXPECT_EQ(output2, outputReference);

    // Struct handler not support interface version
    clientInterfaces[0].version = 0;
    clientSettings.init(clientProtocolVersions, {}, {}, clientInterfaces);

    csp::messaging::Client dataClient3(simpleSpeaker);
    dataClient3.init(clientSettings);

    EXPECT_EQ(dataClient3.getInterfaceVersion(clientSettings.getInterfaces()[0].id), 0);

    interface_for_test::SimplyAssignableDescendant<> output3;

    EXPECT_EQ((dataClient3.handleData<CdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>>>(input, output3)), Status::ErrorNotSupportedInterfaceVersion);
    EXPECT_EQ(output3.m_d, 0); // struct wasn't changed
}

} // namespace
