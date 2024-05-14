/**
 * @file UnitTests/ForTestsHelpers/include/ft_helpers/SimpleDataHandlers.h
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
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOne<>& output);
template<>
void fillingStruct(not_part_of_interfaces::SimplyAssignable& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableDescendant<>& output);
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

template<typename InputStruct>
cs::Status multiHandle(const InputStruct& input)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return cs::Status::kErrorInternal;

    ++numberOfMultiEntrances;

    return cs::Status::kNoError;
}

class FirstDataHandler 
    : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>, 1>>
    , cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStack<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>
    , cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<interface_for_test::SimplyAssignable<>, cs::csp::service_structs::ISerializableDummy<>>>
{
public:
    FirstDataHandler(cs::csp::messaging::IServerDataHandlerRegistrar& registrar)
        : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhHeap<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>, 1>>(registrar)
        , cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStack<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>(registrar)
        , cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<interface_for_test::SimplyAssignable<>, cs::csp::service_structs::ISerializableDummy<>>>(registrar)
    { }

    cs::Status handleData(
          const interface_for_test::SimplyAssignableAlignedToOne<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , interface_for_test::SimplyAssignableDescendant<>& output) override
    {
        return defaultHandle(input, output);
    }

    cs::Status handleData(
          const interface_for_test::Diamond<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , interface_for_test::DynamicPolymorphic<>& output) override
    {
        return defaultHandle(input, output);
    }

    cs::Status handleData(
          const interface_for_test::SimplyAssignable<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        
        return multiHandle(input);
    }
};

class SecondDataHandler
    : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<interface_for_test::SimplyAssignable<>, cs::csp::service_structs::ISerializableDummy<>>>
{
public:
    SecondDataHandler(cs::csp::messaging::IServerDataHandlerRegistrar& registrar)
        : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<interface_for_test::SimplyAssignable<>, cs::csp::service_structs::ISerializableDummy<>>>(registrar)
    { }

    cs::Status handleData(const interface_for_test::SimplyAssignable<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        return multiHandle(input);
    }
};

class ThirdDataHandler
    : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<descendant_interface::DiamondDescendant<>, cs::csp::service_structs::ISerializableDummy<>>>
{
public:
    ThirdDataHandler(cs::csp::messaging::IServerDataHandlerRegistrar& registrar)
        : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<descendant_interface::DiamondDescendant<>, cs::csp::service_structs::ISerializableDummy<>>>(registrar)
    { }

    cs::Status handleData(
          const descendant_interface::DiamondDescendant<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , cs::csp::service_structs::ISerializableDummy<>& output) override
    {
        return defaultHandle(input, output);
    }
};

class FourthDataHandler
    : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<another_yet_interface::SimpleStruct<>, cs::csp::service_structs::ISerializableDummy<>>>
{
public:
    FourthDataHandler(cs::csp::messaging::IServerDataHandlerRegistrar& registrar)
        : cs::csp::messaging::IServerDataHandler<cs::csp::messaging::SdhStackMulti<another_yet_interface::SimpleStruct<>, cs::csp::service_structs::ISerializableDummy<>>>(registrar)
    { }

    cs::Status handleData(
          const another_yet_interface::SimpleStruct<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::GenericPointerKeeper& clientId
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
