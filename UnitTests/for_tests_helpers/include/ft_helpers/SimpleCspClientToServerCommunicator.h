/**
 * @file UnitTests/for_tests_helpers/include/ft_helpers/SimpleCspClientToServerCommunicator.h
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

#pragma once

namespace ft_helpers
{

namespace cs = common_serialization;

class SimpleCspClientToServerCommunicator : public cs::csp::messaging::IClientToServerCommunicator
{
public:
    SimpleCspClientToServerCommunicator(cs::csp::messaging::Server& server) : m_server(server) {}

    void setValidState(bool isValid)
    {
        m_isValid = isValid;
    }

private:
    // This function must transfer data from client to server.
    // Way by which it will be done is up to concrete client realization.
    // Here we do not need to overcomplicate things and we simply calling csp::messaging::Server::handleMessage.
    cs::Status process(const cs::BinVectorT& input, cs::BinVectorT& output) override
    {
        cs::BinWalkerT inputW;
        inputW.init(input);

        return m_server.handleMessage(inputW, cs::GenericPointerKeeper{}, output);
    }

    cs::csp::messaging::Server& m_server;
    bool m_isValid{ true };
};

} // namespace ft_helpers

