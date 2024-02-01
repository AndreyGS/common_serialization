/**
 * @file MessagingTests.cpp
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

namespace
{

using namespace common_serialization;
using namespace ft_helpers;

csp::service_structs::CspPartySettings<>& getServerSettings()
{
    static csp::service_structs::CspPartySettings<> serverSettings;
    if (serverSettings.supportedCspVersions.size() == 0)
    {
        serverSettings.supportedCspVersions.pushBackN(csp::traits::kProtocolVersions, csp::traits::getProtocolVersionsCount());

        serverSettings.interfaces.pushBack({ interface_for_test::properties.id, interface_for_test::properties.version });
        serverSettings.interfaces.pushBack({ descendant_interface::properties.id, descendant_interface::properties.version });
        serverSettings.interfaces.pushBack({ another_yet_interface::properties.id, another_yet_interface::properties.version });
    }

    return serverSettings;
}

TEST(MessagingTests, DataServiceServerTest)
{
    // Create client (to request on data server)
    csp::messaging::DataClient dataClient(new SimpleDataClient);

    // Create DataServiceServer (to response on client requests)
    csp::messaging::DataServiceServer<ft_helpers::DataServiceServerTraits> serviceServer;

    // Test of getting all availible interfaces on server
    csp::service_structs::InterfacesList outInterfacesList;
    EXPECT_EQ(dataClient.handleData(csp::service_structs::GetInterfacesList<>{}, outInterfacesList), Status::kNoError);

    csp::service_structs::InterfacesList interfacesListReference;
    DataServiceServerTraits::fillInterfacesList(interfacesListReference.list);

    EXPECT_EQ(outInterfacesList, interfacesListReference);

    // Test of getting properties of single interface on server
    csp::service_structs::GetInterface getInterfaceProps;
    getInterfaceProps.id = outInterfacesList.list[outInterfacesList.list.size()-1].id;
    csp::service_structs::OutGetInterface outGetInterfaceProps;

    EXPECT_EQ(dataClient.handleData(getInterfaceProps, outGetInterfaceProps), Status::kNoError);
    EXPECT_EQ(outGetInterfaceProps.properties, outInterfacesList.list[outInterfacesList.list.size() - 1]);

    // Test of getting properties of single unknown/invalid_id interface on server
    getInterfaceProps.id = getInterfaceProps.getId();

    EXPECT_EQ(dataClient.handleData(getInterfaceProps, outGetInterfaceProps), Status::kNoError);

    csp::traits::Interface interfacePropsReference;
    interfacePropsReference.id = getInterfaceProps.id;
    interfacePropsReference.version = csp::traits::kInterfaceVersionUndefined;

    EXPECT_EQ(outGetInterfaceProps.properties, interfacePropsReference);
}
/*
TEST(MessagingTests, MainTest)
{
    SimpleDataClient dataClient;

    FirstDataServer dataServer1;
    SecondDataServer dataServer2;
    ThirdDataServer dataServer3;
    FourthDataServer dataServer4;

    interface_for_test::SimpleAssignableAlignedToOne<> saaTo;
    fillingStruct(saaTo);

    interface_for_test::SimpleAssignableDescendant<> sad;

    // Send with parameters by default
    EXPECT_EQ(dataClient.handleData(saaTo, sad), Status::kNoError);

    interface_for_test::SimpleAssignableDescendant<> sadReference;
    fillingStruct(sadReference);

    EXPECT_EQ(sad, sadReference);

    csp::context::DataFlags dataFlags;
    dataFlags.alignmentMayBeNotEqual = true;
    dataFlags.sizeOfArithmeticTypesMayBeNotEqual = true;
    dataFlags.checkRecursivePointers = true;

    interface_for_test::SimpleAssignableDescendant<> sad2;

    Vector<GenericPointerKeeper> unmanagedPointers;

    // Test when we send unsupported struct version, and then resend with supported version
    EXPECT_EQ(dataClient.handleData(saaTo, sad2, dataFlags, 1, 0, 0, 0, csp::traits::getLatestProtocolVersion(), &unmanagedPointers), Status::kNoError);

    EXPECT_EQ(sad2, sadReference);

    // Test of no output struct
    interface_for_test::SimpleAssignable<> sa;
    fillingStruct(sa);

    csp::messaging::ISerializableDummy<> dummy{};

    // Send with parameters by default
    EXPECT_EQ(dataClient.handleData(sa, dummy), Status::kNoError);
}


class TestSubscriber
    : csp::messaging::IMethodDataServer<SimpleAssignableAlignedToOne<>, SimpleAssignableDescendant<>
        , true, false, SimpleAssignableAlignedToOne<>::getOriginPrivateVersion(), 3>
    , csp::messaging::IStaticDataServer<TestSubscriber, Diamond<>, DynamicPolymorphic<>, false>
{
public:
    Status handleData(const SimpleAssignableAlignedToOne<>& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, SimpleAssignableDescendant<>& output) override
    {
        return Status::kNoError;
    }

    static Status handleDataStatic(const Diamond<>& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, DynamicPolymorphic<>& output)
    {
        return Status::kNoError;
    }
};

TEST(MessagingTests, Temp)
{
    TestSubscriber testSubs;

    SimpleAssignableAlignedToOne testInput;
    BinWalker bin;
    csp::context::SInOutData<> ctxIn(bin.getVector());
    ctxIn.setMessageType(csp::context::Message::kInOutData);
    ctxIn.setInterfaceVersion(1);
    ctxIn.setOutputInterfaceVersion(2);

    csp::processing::serializeCommonContext(ctxIn);
    csp::processing::serializeInOutDataContext<SimpleAssignableAlignedToOne<>>(ctxIn);
    csp::processing::DataProcessor::serializeData(testInput, ctxIn);

    BinVector binOut;
    csp::messaging::CommonServer::handleMessage(bin, cs::BinVector{}, binOut);

    SimpleAssignableDescendant testOutput;

    BinWalker binIn;
    binIn.pushBack(1);
    
    Vector<csp::messaging::IDataServerBase*, RawGenericAllocatorHelper<csp::messaging::IDataServerBase*>> subscribers;
    csp::messaging::GetDataServersKeeper().findServers(testInput.getId(), subscribers);
    //subscribers[0]->handleDataConcrete(binIn, binOut);

    testSubs.~TestSubscriber();

    Diamond testInput2;
    DynamicPolymorphic testOutput2;

    csp::messaging::GetDataServersKeeper().findServers(testInput2.getId(), subscribers);
    
    //if (subscribers.size())
        //subscribers[0]->handleDataConcrete(binIn, binOut);

    csp::messaging::DataServiceServer<ft_helpers::DataServiceServerTraits> serviceServer;
    csp::messaging::InterfacesList list;
    serviceServer.handleDataStatic(csp::messaging::GetInterfacesList<>{}, nullptr, list);
}
*/
} // namespace anonymous
