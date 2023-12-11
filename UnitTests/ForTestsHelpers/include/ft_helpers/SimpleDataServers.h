/**
 * @file UnitTests/ForTestsHelpers/include/ft_helpers/SimpleDataServers.h
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
/*
class CspServiceServer
    : cs::csp::messaging::IStaticDataServer<CspServiceServer, cs::csp::messaging::GetInterfaceVersion<>, cs::csp::messaging::InterfaceVersion<>, false>
    , cs::csp::messaging::IStaticDataServer<CspServiceServer, cs::csp::messaging::GetInterfacesList<>, cs::csp::messaging::InterfacesList<>, false>
{
    static cs::Status handleDataStatic(const cs::csp::messaging::GetInterfacesList<>& input, cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers, cs::csp::messaging::InterfacesList<>& output)
    {


        return cs::Status::kNoError;
    }
};


class FirstDataServer 
    : cs::csp::messaging::IMethodDataServer<interface_for_test::SimpleAssignableAlignedToOne<>, interface_for_test::SimpleAssignableDescendant<>, true, false, interface_for_test::SimpleAssignableAlignedToOne<>::getMinimumInterfaceVersion(), 3>
    , cs::csp::messaging::IStaticDataServer<FirstDataServer, interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>, false>
    , cs::csp::messaging::IStaticDataServer<FirstDataServer, cs::csp::messaging::GetInterfaceVersion<>, cs::csp::messaging::InterfaceVersion<>, false>
{};

class SecondDataServer
{};

class ThirdDataServer
{};
*/
} // namespace ft_helpers