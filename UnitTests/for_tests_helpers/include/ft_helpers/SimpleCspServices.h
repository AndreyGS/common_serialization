/**
 * @file UnitTests/for_tests_helpers/include/ft_helpers/SimpleDataServices.h
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
void fillingStruct(restricted_structs::SimplyAssignable& output);
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
using namespace cs::csp::messaging::service_structs;

inline int numberOfMultiEntrances = 0;

template<typename InputStruct, typename OutputStruct>
cs::Status defaultHandle(const InputStruct& input, OutputStruct& output)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return cs::Status::ErrorInternal;

    fillingStruct(output);

    return cs::Status::NoError;
}

template<typename InputStruct>
cs::Status multiHandle(const InputStruct& input)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return cs::Status::ErrorInternal;

    ++numberOfMultiEntrances;

    return cs::Status::NoError;
}

template<typename _T>
using IServerDataHandler = cs::csp::messaging::IServerDataHandler<_T>;

namespace csm = cs::csp::messaging;

class FirstCspService
    : IServerDataHandler<csm::ServerHeapHandler<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>, 1>>
    , IServerDataHandler<csm::ServerStackHandler<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>
    , IServerDataHandler<csm::ServerStackMultiHandler<interface_for_test::SimplyAssignable<>, ISerializableDummy>>
{
public:
    FirstCspService() = default;
    
    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerHeapHandler<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>, 1>>::registerHandler(registrar, this);
        IServerDataHandler<csm::ServerStackHandler<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>::registerHandler(registrar, this);
        IServerDataHandler<csm::ServerStackMultiHandler<interface_for_test::SimplyAssignable<>, ISerializableDummy>>::registerHandler(registrar, this);

        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    void unregisterSimplyAssignableAlignedToOne(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerHeapHandler<interface_for_test::SimplyAssignableAlignedToOne<>, interface_for_test::SimplyAssignableDescendant<>, 1>>::unregisterHandler(registrar);
    }

    void unregisterDiamond(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackHandler<interface_for_test::Diamond<>, interface_for_test::DynamicPolymorphic<>>>::unregisterHandler(registrar);
    }

    void unregisterSimplyAssignable(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<interface_for_test::SimplyAssignable<>, ISerializableDummy>>::unregisterHandler(registrar);
    }

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
        , ISerializableDummy& output) override
    {
        
        return multiHandle(input);
    }
};

class SecondCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<interface_for_test::SimplyAssignable<>, ISerializableDummy>>
{
public:
    SecondCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<interface_for_test::SimplyAssignable<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    cs::Status handleData(const interface_for_test::SimplyAssignable<>& input
        , cs::Vector<cs::GenericPointerKeeper>* pUnmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        return multiHandle(input);
    }
};

class ThirdCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<descendant_interface::DiamondDescendant<>, ISerializableDummy>>
{
public:
    ThirdCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<descendant_interface::DiamondDescendant<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    cs::Status handleData(
          const descendant_interface::DiamondDescendant<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        return defaultHandle(input, output);
    }
};

class FourthCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<another_yet_interface::SimpleStruct<>, ISerializableDummy>>
{
public:
    FourthCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<another_yet_interface::SimpleStruct<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    cs::Status handleData(
          const another_yet_interface::SimpleStruct<>& input
        , cs::Vector<cs::GenericPointerKeeper>* unmanagedPointers
        , const cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        another_yet_interface::SimpleStruct<> test;
        fillingStruct(test);

        if (input != test)
            return cs::Status::ErrorInternal;

        return cs::Status::NoError;
    }
};

} // namespace ft_helpers
