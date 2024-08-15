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
void fillingStruct(tests_csp_interface::SimplyAssignableAlignedToOne<>& output);
template<>
void fillingStruct(tests_restricted_structs::SimplyAssignable& output);
template<>
void fillingStruct(tests_csp_interface::SimplyAssignableDescendant<>& output);
template<>
void fillingStruct(tests_csp_interface::DynamicPolymorphic<>& output);
template<>
void fillingStruct(tests_csp_interface::Diamond<>& output);
template<>
void fillingStruct(tests_csp_descendant_interface::SimpleStruct<>& output);
template<>
void fillingStruct(tests_csp_descendant_interface::DiamondDescendant<>& output);
template<>
void fillingStruct(tests_csp_another_interface::SimpleStruct<>& output);

namespace ags_cs = common_serialization;
using namespace ags_cs::csp::messaging::service_structs;

inline int numberOfMultiEntrances = 0;

template<typename InputStruct, typename OutputStruct>
ags_cs::Status defaultHandle(const InputStruct& input, OutputStruct& output)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return ags_cs::Status::ErrorInternal;

    fillingStruct(output);

    return ags_cs::Status::NoError;
}

template<typename InputStruct>
ags_cs::Status multiHandle(const InputStruct& input)
{
    InputStruct test;
    fillingStruct(test);

    if (input != test)
        return ags_cs::Status::ErrorInternal;

    ++numberOfMultiEntrances;

    return ags_cs::Status::NoError;
}

template<typename _T>
using IServerDataHandler = ags_cs::csp::messaging::IServerDataHandler<_T>;

namespace csm = ags_cs::csp::messaging;

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

    ags_cs::Status handleData(
          const tests_csp_interface::SimplyAssignableAlignedToOne<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* pUnmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
        , tests_csp_interface::SimplyAssignableDescendant<>& output) override
    {
        return defaultHandle(input, output);
    }

    ags_cs::Status handleData(
          const tests_csp_interface::Diamond<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* pUnmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
        , tests_csp_interface::DynamicPolymorphic<>& output) override
    {
        return defaultHandle(input, output);
    }

    ags_cs::Status handleData(
          const tests_csp_interface::SimplyAssignable<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* pUnmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
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

    ags_cs::Status handleData(const tests_csp_interface::SimplyAssignable<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* pUnmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        return multiHandle(input);
    }
};

class ThirdCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_descendant_interface::DiamondDescendant<>, ISerializableDummy>>
{
public:
    ThirdCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_descendant_interface::DiamondDescendant<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    ags_cs::Status handleData(
          const tests_csp_descendant_interface::DiamondDescendant<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* unmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        return defaultHandle(input, output);
    }
};

class FourthCspService
    : IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_another_interface::SimpleStruct<>, ISerializableDummy>>
{
public:
    FourthCspService() = default;

    Status registerHandlers(csm::IServerDataHandlerRegistrar& registrar)
    {
        IServerDataHandler<csm::ServerStackMultiHandler<tests_csp_another_interface::SimpleStruct<>, ISerializableDummy>>::registerHandler(registrar, this);
        return Status::NoError;
    }

    void unregisterService(csm::IServerDataHandlerRegistrar& registrar)
    {
        registrar.unregisterService(this);
    }

    ags_cs::Status handleData(
          const tests_csp_another_interface::SimpleStruct<>& input
        , ags_cs::Vector<ags_cs::GenericPointerKeeper>* unmanagedPointers
        , const ags_cs::GenericPointerKeeper& clientId
        , ISerializableDummy& output) override
    {
        tests_csp_another_interface::SimpleStruct<> test;
        fillingStruct(test);

        if (input != test)
            return ags_cs::Status::ErrorInternal;

        return ags_cs::Status::NoError;
    }
};

} // namespace ft_helpers
