/**
 * @file common_serializaiton/csp_messaging/unit_tests/ComplexTests.cpp
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <common_serialization/csp_messaging/csp_messaging.h>
#include <common_serialization/tests_csp_another_interface/tests_csp_another_interface.h>
#include <common_serialization/tests_csp_descendant_interface/tests_csp_descendant_interface.h>
#include <common_serialization/tests_csp_interface/tests_csp_interface.h>
#include <common_serialization/tests_restricted_structs/tests_restricted_structs.h>
#include "Helpers.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

namespace
{

using namespace common_serialization;
using namespace csp::context;
using namespace csp::messaging;
using namespace csp::messaging::service_structs;
using namespace csp::traits;

class ClientToServerCommunicatorMock : public csp::messaging::IClientToServerCommunicator
{
public:
    MOCK_METHOD(Status, process, (const BinVectorT& input, BinVectorT& output), (override));
};

inline int g_numberOfMultiEntrances = 0;

template<typename InputStruct, typename OutputStruct>
Status defaultHandle(const InputStruct& input, OutputStruct& output)
{
    InputStruct test;
    test.fill();

    EXPECT_EQ(input, test);

    output.fill();

    return Status::NoError;
}

template<typename InputStruct>
Status multiHandle(const InputStruct& input)
{
    InputStruct test;
    test.fill();

    EXPECT_EQ(input, test);

    ++g_numberOfMultiEntrances;

    return Status::NoError;
}

template<typename T>
using IServerDataHandler = csp::messaging::IServerDataHandler<T>;

namespace csm = csp::messaging;

class FirstCspService
    : IServerDataHandler<csm::ServerHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>, 1>>
    , IServerDataHandler<csm::ServerStackHandler<tests_csp_interface::Diamond<>, tests_csp_interface::DynamicPolymorphic<>>>
    , IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>
{
public:
    FirstCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>, 1>>::registerHandler(registrar, this);
        IServerDataHandler<csm::ServerStackHandler<tests_csp_interface::Diamond<>, tests_csp_interface::DynamicPolymorphic<>>>::registerHandler(registrar, this);
        IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>::registerHandler(registrar, this);

        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    void unregisterSimplyAssignableAlignedToOne(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>, 1>>::unregisterHandler(registrar);
    }

    void unregisterDiamond(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackHandler<tests_csp_interface::Diamond<>, tests_csp_interface::DynamicPolymorphic<>>>::unregisterHandler(registrar);
    }

    void unregisterSimplyAssignable(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>::unregisterHandler(registrar);
    }

    Status handleData(
        const tests_csp_interface::SimplyAssignableAlignedToOne<>& input
        , Vector<GenericPointerKeeper>* pUnmanagedPointers
        , const GenericPointerKeeper& clientId
        , tests_csp_interface::SimplyAssignableDescendant<>& output) override
    {
        return defaultHandle(input, output);
    }

    Status handleData(
        const tests_csp_interface::Diamond<>& input
        , Vector<GenericPointerKeeper>* pUnmanagedPointers
        , const GenericPointerKeeper& clientId
        , tests_csp_interface::DynamicPolymorphic<>& output) override
    {
        return defaultHandle(input, output);
    }

    Status handleData(
        const tests_csp_interface::SimplyAssignable<>& input
        , Vector<GenericPointerKeeper>* pUnmanagedPointers
        , const GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {

        return multiHandle(input);
    }
};

class SecondCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>
{
public:
    SecondCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    Status handleData(const tests_csp_interface::SimplyAssignable<>& input
        , Vector<GenericPointerKeeper>* pUnmanagedPointers
        , const GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        return multiHandle(input);
    }
};

class ComplexTests : public ::testing::Test
{
public:
    ComplexTests()
        : m_clientToServerCommunicator(), m_client(m_clientToServerCommunicator)
    {
        m_server.init<GenericServerDataHandlerRegistrar>(getValidCspPartySettings());
        m_client.init(getValidCspPartySettings());
    }

protected:
    csp::messaging::Server m_server;
    csp::messaging::Client m_client;
    ClientToServerCommunicatorMock m_clientToServerCommunicator;
};

TEST_F(ComplexTests, SimpleUnicastTest)
{
    FirstCspService firstCspService;
    firstCspService.registerHandlers(*m_server.getDataHandlersRegistrar());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    input.fill();
    tests_csp_interface::SimplyAssignableDescendant<> output;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&server = this->m_server](const BinVectorT& input, BinVectorT& output)
        {
            BinWalkerT inputW;
            inputW.init(input);

            return server.handleMessage(inputW, GenericPointerKeeper{}, output);
        })
    );

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(input, output)), Status::NoError);

    tests_csp_interface::SimplyAssignableDescendant<> outputReference;
    outputReference.fill();

    EXPECT_EQ(output, outputReference);
}

TEST_F(ComplexTests, SimpleMulticastTest)
{
    FirstCspService firstCspService;
    firstCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
    SecondCspService secondCspService;
    secondCspService.registerHandlers(*m_server.getDataHandlersRegistrar());

    tests_csp_interface::SimplyAssignable<> input;
    input.fill();
    ISerializableDummy outputDummy;

    g_numberOfMultiEntrances = 0;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillOnce(Invoke(
        [&server = this->m_server](const BinVectorT& input, BinVectorT& output)
        {
            BinWalkerT inputW;
            inputW.init(input);

            return server.handleMessage(inputW, GenericPointerKeeper{}, output);
        })
    );

    EXPECT_EQ((m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>(input, outputDummy)), Status::NoError);
    EXPECT_EQ(g_numberOfMultiEntrances, 2);
}

TEST_F(ComplexTests, StressTest)
{   
    FirstCspService firstCspService;
    firstCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
    SecondCspService secondCspService;
    secondCspService.registerHandlers(*m_server.getDataHandlersRegistrar());

    tests_csp_interface::SimplyAssignableAlignedToOne<> input;
    input.fill();
    tests_csp_interface::SimplyAssignableDescendant<> output;

    tests_csp_interface::SimplyAssignable<> input2;
    input2.fill();
    ISerializableDummy outputDummy;

    tests_csp_interface::Diamond<> input3;
    input3.fill();
    tests_csp_interface::DynamicPolymorphic<> output3;

    EXPECT_CALL(m_clientToServerCommunicator, process).WillRepeatedly(Invoke(
        [&server = this->m_server](const BinVectorT& input, BinVectorT& output)
        {
            BinWalkerT inputW;
            inputW.init(input);

            return server.handleMessage(inputW, GenericPointerKeeper{}, output);
        })
    );

    std::thread test1([&]
        {
            for (size_t i = 0; i < 10000; ++i)
            {
                m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>(input2, outputDummy);
                m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(input, output);
                m_client.handleData<ClientHeapHandler<tests_csp_interface::Diamond<>, tests_csp_interface::DynamicPolymorphic<>>>(input3, output3);
            }
        });

    std::thread test2([&]
        {
            for (size_t i = 0; i < 10000; ++i)
            {
                m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignable<>, ISerializableDummy>>(input2, outputDummy);
                m_client.handleData<ClientHeapHandler<tests_csp_interface::Diamond<>, tests_csp_interface::DynamicPolymorphic<>>>(input3, output3);
                m_client.handleData<ClientHeapHandler<tests_csp_interface::SimplyAssignableAlignedToOne<>, tests_csp_interface::SimplyAssignableDescendant<>>>(input, output);
            }
        });
        
    std::thread test3([&]
        {
            for (size_t i = 0; i < 200; ++i)
            {
                firstCspService.unregisterService(*m_server.getDataHandlersRegistrar());
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                firstCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
                firstCspService.unregisterSimplyAssignableAlignedToOne(*m_server.getDataHandlersRegistrar());
                firstCspService.unregisterSimplyAssignableAlignedToOne(*m_server.getDataHandlersRegistrar());
                firstCspService.unregisterDiamond(*m_server.getDataHandlersRegistrar());
                firstCspService.unregisterSimplyAssignable(*m_server.getDataHandlersRegistrar());
                firstCspService.unregisterService(*m_server.getDataHandlersRegistrar());
                firstCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
            }
        });

    std::thread test4([&]
        {
            for (size_t i = 0; i < 200; ++i)
            {
                secondCspService.unregisterService(*m_server.getDataHandlersRegistrar());
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                secondCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
                secondCspService.unregisterService(*m_server.getDataHandlersRegistrar());
                secondCspService.registerHandlers(*m_server.getDataHandlersRegistrar());
            }
        });

    test1.join();
    test2.join();
    test3.join();
    test4.join();
}

} // namespace
