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

#include "Tests/csp/Helpers.h"
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

using namespace common_serialization;
using namespace common_serialization::csp;
using namespace common_serialization::csp::messaging;
using namespace common_serialization::csp::messaging::service_structs;


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
    clientSettings = getValidCspPartySettings();
    clientSettings.forbiddenCommonFlags.addFlags(context::CommonFlags::kBigEndianFormat);
    
        // Test 1: Server is not supported CSP version used by Client
    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getNotSupportedProtocolVersionsWithInvalidOutput());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorNotSupportedProtocolVersion);
    EXPECT_EQ(m_client.isValid(), false);

        // Test 2: Server settings are not compatible with used by Client

    EXPECT_CALL(m_speaker, speak).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getNotSupportedProtocolVersionsWithValidOutput());
            return Status::NoError;
        }
                                )).WillOnce(Invoke(
        [](const BinVectorT& binInput, BinWalkerT& binOutput)
        {
            binOutput.init(getSettingsWithBigEndian());
            return Status::NoError;
        }
    ));

    EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorNotCompatibleCommonFlagsSettings);
    EXPECT_EQ(m_client.isValid(), false);


    BinVectorT vector;
    vector.size();
    // Try to pass valid settings to inited Client
   // EXPECT_EQ(m_client.init(clientSettings, serverSettings), Status::ErrorAlreadyInited);
}

} // namespace