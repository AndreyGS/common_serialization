/**
 * @file common_serializaiton/csp_messaging/unit_tests/ServerTests.cpp
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <common_serialization/csp_messaging/service_structs/service_structs.h>
#include <common_serialization/csp_messaging/Server.h>
#include <common_serialization/tests_csp_another_interface/tests_csp_another_interface.h>
#include <common_serialization/tests_csp_descendant_interface/tests_csp_descendant_interface.h>
#include <common_serialization/tests_csp_interface/tests_csp_interface.h>
#include <common_serialization/tests_restricted_structs/tests_restricted_structs.h>
#include "helpers.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

namespace
{

class ServerDataHandlerRegistrarMock : public csp::messaging::IServerDataHandlerRegistrar
{
public:
    MOCK_METHOD(Status, registerHandler, (const Id&, bool, Service*, IServerDataHandlerBase&), (override));
    MOCK_METHOD(void, unregisterHandler, (const Id&, IServerDataHandlerBase&), (noexcept, override));
    MOCK_METHOD(void, unregisterService, (Service*), (noexcept, override));
    MOCK_METHOD(Status, aquireHandlers, (const Id&, RawVectorT<IServerDataHandlerBase*>&), (override));
    MOCK_METHOD(Status, aquireHandler, (const Id&, IServerDataHandlerBase*&), (noexcept, override));
    MOCK_METHOD(void, releaseHandler, (IServerDataHandlerBase*), (noexcept, override));
};

class ServerDataHandlerBaseMock : public csp::messaging::IServerDataHandlerBase
{
public:
    MOCK_METHOD(Status, handleDataCommon, (DData&, const GenericPointerKeeperT&, BinVectorT&), (override));
};

class ServerTests : public ::testing::Test
{
public:
    Status init(const service_structs::CspPartySettings<>& settings)
    {
        return m_server.init<ServerDataHandlerRegistrarMock>(settings);
    }

protected:
    Server m_server;
    GenericPointerKeeperT m_clientId;
};

TEST_F(ServerTests, ConstructorInvalidSettings)
{
    Server server(getInvalidCspPartySettings(), makeUnique<ServerDataHandlerRegistrarMock>());
    EXPECT_FALSE(server.isValid());
}

TEST_F(ServerTests, ConstructorInvalidRegistrar)
{
    Server server(getValidCspPartySettings(), UniquePtrT<ServerDataHandlerRegistrarMock>{});
    EXPECT_FALSE(server.isValid());
}

TEST_F(ServerTests, ConstructorValidSettings)
{
    Server server(getValidCspPartySettings(), makeUnique<ServerDataHandlerRegistrarMock>());
    EXPECT_TRUE(server.isValid());
}

TEST_F(ServerTests, InitInvalidSettings)
{
    EXPECT_EQ(init(getInvalidCspPartySettings()), Status::ErrorInvalidArgument);
    EXPECT_FALSE(m_server.isValid());
}

TEST_F(ServerTests, InitAlreadyInited)
{
    init(getValidCspPartySettings());
    EXPECT_EQ(init(getValidCspPartySettings()), Status::ErrorAlreadyInited);
}

TEST_F(ServerTests, InitValidSettings)
{
    EXPECT_EQ(init(getValidCspPartySettings()), Status::NoError);
    EXPECT_TRUE(m_server.isValid());
}

TEST_F(ServerTests, GetDataHandlersRegistrar)
{
    EXPECT_FALSE(m_server.getDataHandlersRegistrar());
    init(getValidCspPartySettings());
    EXPECT_TRUE(m_server.getDataHandlersRegistrar());
}

TEST_F(ServerTests, GetSettingsServerNotInited)
{
    EXPECT_EQ(m_server.getSettings(), CspPartySettings<>());
}

TEST_F(ServerTests, GetSettingsServerInited)
{
    CspPartySettings settings = getValidCspPartySettings();
    init(getValidCspPartySettings());
    EXPECT_EQ(m_server.getSettings(), settings);
}

TEST_F(ServerTests, HandleMessageNotInited)
{
    BinWalkerT binInput;
    BinVectorT binOutput;
    EXPECT_EQ(m_server.handleMessage(binInput, m_clientId, binOutput), Status::ErrorNotInited);
}

TEST_F(ServerTests, HandleMessageErrorInCommonContextDeserialize)
{
    init(getValidCspPartySettings());
    BinWalkerT binInput;
    BinWalkerT binOutput;
    EXPECT_EQ(m_server.handleMessage(binInput, m_clientId, binOutput.getVector()), Status::ErrorOverflow);

    DData ctxOut(binOutput);
    Status statusOut{ Status::NoError };
    EXPECT_EQ(processing::common::ContextProcessor::deserialize(ctxOut), Status::NoError);
    EXPECT_EQ(ctxOut.getMessageType(), Message::Status);
    
    EXPECT_EQ(processing::status::ContextProcessor::deserialize(ctxOut, statusOut), Status::NoError);
    EXPECT_EQ(statusOut, Status::ErrorOverflow);
}

TEST_F(ServerTests, HandleMessageProcessErrorNotSupportedProtocolVersion)
{
    init(getValidCspPartySettings());
    BinWalkerT binInput;
    SCommon ctxIn(binInput.getVector(), traits::kProtocolVersionUndefined);
    processing::common::ContextProcessor::serializeNoChecks(ctxIn);
    BinWalkerT binOutput;

    EXPECT_EQ(m_server.handleMessage(binInput, m_clientId, binOutput.getVector()), Status::NoError);

    DData ctxOut(binOutput);
    Status statusOut{ Status::NoError };
    EXPECT_EQ(processing::common::ContextProcessor::deserializeNoChecks(ctxOut), Status::NoError);
    EXPECT_EQ(ctxOut.getMessageType(), Message::Status);

    EXPECT_EQ(processing::status::ContextProcessor::deserialize(ctxOut, statusOut), Status::NoError);
    EXPECT_EQ(statusOut, Status::ErrorNotSupportedProtocolVersion);

    RawVectorT<protocol_version_t> protocolVersions;
    EXPECT_EQ(processing::status::BodyProcessor::deserializeErrorNotSupportedProtocolVersion(ctxOut, protocolVersions), Status::NoError);
    EXPECT_EQ(protocolVersions, m_server.getSettings().getProtocolVersions());
}

TEST_F(ServerTests, HandleMessageGetSettings)
{
    init(getValidCspPartySettings());
    BinWalkerT binInput;
    SCommon ctxIn(binInput.getVector(), getLatestProtocolVersion(), context::Message::GetSettings, {});
    processing::common::ContextProcessor::serialize(ctxIn);
    BinWalkerT binOutput;

    EXPECT_EQ(m_server.handleMessage(binInput, m_clientId, binOutput.getVector()), Status::NoError);

    service_structs::CspPartySettings<> receivedSettings;
    EXPECT_EQ(receivedSettings.deserialize(binOutput), Status::NoError);
    EXPECT_EQ(receivedSettings, m_server.getSettings());
}

TEST_F(ServerTests, HandleMessageNotSupportedOne)
{
    init(getValidCspPartySettings());
    BinWalkerT binInput;
    processing::status::Helpers::serializeFullContext(binInput.getVector(), m_server.getSettings().getLatestProtocolVersion(), m_server.getSettings().getMandatoryCommonFlags(), Status::NoError);
    BinWalkerT binOutput;

    EXPECT_EQ(m_server.handleMessage(binInput, m_clientId, binOutput.getVector()), Status::ErrorDataCorrupted);

    DData ctxOut(binOutput);
    processing::common::ContextProcessor::deserialize(ctxOut);
    Status statusOut{ Status::NoError };
    processing::status::ContextProcessor::deserialize(ctxOut, statusOut);
    EXPECT_EQ(statusOut, Status::ErrorDataCorrupted);
}

} // namespace
