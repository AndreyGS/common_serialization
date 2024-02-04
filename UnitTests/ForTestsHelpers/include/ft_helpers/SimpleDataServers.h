/**
 * @file UnitTests/ForTestsHelpers/include/ft_helpers/SimpleDataServers.h
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

template<>
void fillingStruct(interface_for_test::SimpleAssignableAlignedToOne<>& output);
template<>
void fillingStruct(not_part_of_interfaces::SimpleAssignable& output);
template<>
void fillingStruct(interface_for_test::SimpleAssignableDescendant<>& output);
template<>
void fillingStruct(interface_for_test::DynamicPolymorphic<>& output);
template<>
void fillingStruct(interface_for_test::Diamond<>& output);
template<>
void fillingStruct(descendant_interface::SimpleStruct<>& output);
template<>
void fillingStruct(descendant_interface::DiamondDescendant<>& output);
template<>
void fillingStruct(another_yet_interface::SimpleStruct<>& output);

namespace cs = common_serialization;

class DataServiceServerTraits
{
public:
    static void fillInterfacesList(cs::Vector<cs::csp::traits::Interface>& list)
    {
        // There can be some algorithm to register different interfaces automatically.

        if (list.size() == 0)
        {
            // Adding interface_for_test
            list.pushBack(interface_for_test::properties);
            // Adding descendant_interface
            list.pushBack(descendant_interface::properties);
            // Adding another_yet_interface
            list.pushBack(another_yet_interface::properties);
        }
    }
};

inline int numberOfMultiEntrances = 0;

template<typename InputStruct, typename OutputStruct>
cs::Status defaultHandle(const InputStruct& input, OutputStruct& output)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return cs::Status::kErrorInternal;

    fillingStruct(output);

    return cs::Status::kNoError;
}

class FirstDataServer 
    : cs::csp::messaging::IDataServer<interface_for_test::SimpleAssignableAlignedToOne<>, interface_for_test::SimpleAssignableDescendant<>, true, false, interface_for_test::SimpleAssignableAlignedToOne<>::getOriginPrivateVersion()>
    , cs::csp::messaging::IDataServer<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>, false>
    , cs::csp::messaging::IDataServer<interface_for_test::SimpleAssignable<>, cs::csp::service_structs::ISerializableDummy<>, false, true>
{
public:
    cs::Status handleData(
          const interface_for_test::SimpleAssignableAlignedToOne<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::BinVector& clientId
        , interface_for_test::SimpleAssignableDescendant<>& output) override
    {
        return defaultHandle(input, output);
    }

    cs::Status handleData(
          const interface_for_test::Diamond<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::BinVector& clientId
        , interface_for_test::DynamicPolymorphic<>& output) override
    {
        return defaultHandle(input, output);
    }

    cs::Status handleData(
          const interface_for_test::SimpleAssignable<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::BinVector& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        ++numberOfMultiEntrances;
        return cs::Status::kNoError;
    }
};

class SecondDataServer
    : cs::csp::messaging::IDataServer<interface_for_test::SimpleAssignable<>, cs::csp::service_structs::ISerializableDummy<>, false, true>
{
public:
    cs::Status handleData(const interface_for_test::SimpleAssignable<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::BinVector& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        ++numberOfMultiEntrances;
        return cs::Status::kNoError;
    }
};

class ThirdDataServer
    : cs::csp::messaging::IDataServer<descendant_interface::DiamondDescendant<>, descendant_interface::SimpleStruct<>, false, true>
{
public:
    cs::Status handleData(
          const descendant_interface::DiamondDescendant<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::BinVector& clientId
        , descendant_interface::SimpleStruct<>& output) override
    {
        return defaultHandle(input, output);
    }
};

class FourthDataServer
    : cs::csp::messaging::IDataServer<another_yet_interface::SimpleStruct<>, cs::csp::service_structs::ISerializableDummy<>, false, true>
{
public:
    cs::Status handleData(
          const another_yet_interface::SimpleStruct<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::BinVector& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        another_yet_interface::SimpleStruct<> test;
        fillingStruct(test);

        if (input != test)
            return cs::Status::kErrorInternal;

        return cs::Status::kNoError;
    }
};

} // namespace ft_helpers
