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
#include "Tests/csp/messaging/ServerDataHandlerRegistrarMock.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

namespace
{

using namespace ft_helpers;

class ServerTests : public ::testing::Test
{
public:
    Status init(const service_structs::CspPartySettings<>& settings)
    {
        return m_server.init<ServerDataHandlerRegistrarMock>(settings);
    }

protected:
    Server m_server;
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

TEST_F(ServerTests, getDataHandlersRegistrar)
{
    EXPECT_FALSE(m_server.getDataHandlersRegistrar());
    init(getValidCspPartySettings());
    EXPECT_TRUE(m_server.getDataHandlersRegistrar());
}

} // namespace
