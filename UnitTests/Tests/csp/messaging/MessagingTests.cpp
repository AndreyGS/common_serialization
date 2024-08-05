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
#include <Tests/csp/messaging/ClientToServerCommunicatorMock.h>
#include <Tests/csp/messaging/Helpers.h>

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
    std::unique_ptr<ClientToServerCommunicatorMock> m_clientToServerCommunicator;
};

TEST(MessagingTests, DataMessageHandling)
{
    Server commonServer;
    commonServer.init<GenericServerDataHandlerRegistrar>(getValidCspPartySettings());
    FirstCspService* pFirstCspService = new FirstCspService;
    pFirstCspService->registerHandlers(*commonServer.getDataHandlersRegistrar());

    SimpleCspClientToServerCommunicator simpleSpeaker{ commonServer };
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
