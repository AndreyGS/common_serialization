/**
 * @file UnitTests/ForTestsHelpers/include/ft_helpers/SimpleDataClient.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

class SimpleDataClient : public cs::csp::messaging::IDataClient
{
public:
    SimpleDataClient() {}
    SimpleDataClient(cs::csp::protocol_version_t defaultProtocolVersion, cs::csp::context::DataFlags defaultFlags, cs::csp::interface_version_t targetInterfaceVersion)
        : cs::csp::messaging::IDataClient(defaultProtocolVersion, defaultFlags, targetInterfaceVersion)
    {
    }

private:
    // This function must transfer data from client to server.
    // Way by which it will be done is up to concrete client realization.
    // Here we do not need to overcomplicate things and we simply calling csp::messaging::CommonServer::handleMessage.
    cs::Status handleBinData(cs::BinVector& binInput, cs::BinWalker& binOutput) override
    {
        cs::BinWalker input;
        input.init(std::move(binInput));
        return cs::csp::messaging::CommonServer::handleMessage(input, binOutput.getVector());
    }
};

} // namespace ft_helpers

