/**
 * @file MessagingTests.cpp
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

namespace
{

using namespace common_serialization;
using namespace interface_for_test;
using namespace ft_helpers;

TEST(MessagingTests, CommonServerTest)
{
    TunnedDataClient dataClient;
    FourthDataServer dataServer;

    auto preOperationFilter0_1 = [](BinWalker& input)
    {
        csp::context::Common<BinWalker> ctx(input);
        csp::processing::deserializeHeaderContext(ctx);
        EXPECT_EQ(ctx.getProtocolVersion(), 1);

        input.seek(0);
        csp::protocol_version_t unsupportedProtocolVersion = 2;
        input.write(&unsupportedProtocolVersion, sizeof(unsupportedProtocolVersion));
        input.seek(0);
    };

    auto postOperationFilter1 = [](BinWalker& output)
    {
        csp::context::Common<BinWalker> ctx(output);

        csp::processing::deserializeHeaderContext(ctx);
        EXPECT_EQ(ctx.getMessageType(), csp::context::Message::kStatus);

        Status statusOut = Status::kNoError;
        csp::processing::deserializeStatusGetStatus(ctx.getBinaryData(), statusOut);
        EXPECT_EQ(Status::kErrorNotSupportedProtocolVersion, statusOut);

        csp::protocol_version_t protVersionsCount = 0;
        ctx.getBinaryData().readArithmeticValue(protVersionsCount);
        EXPECT_EQ(protVersionsCount, 1);

        csp::protocol_version_t protVersionOut = 0;
        ctx.getBinaryData().readArithmeticValue(protVersionOut);
        EXPECT_EQ(protVersionOut, 1);

        csp::protocol_version_t unsupportedProtocolVersion = 2;
        output.seek(0);
        output.write(&unsupportedProtocolVersion, sizeof(unsupportedProtocolVersion));
        output.seek(0);
    };

    dataClient.setFilterFunction(preOperationFilter0_1, true, 0);
    dataClient.setFilterFunction(preOperationFilter0_1, true, 1);
    dataClient.setFilterFunction(postOperationFilter1, false, 1);

    cs::csp::messaging::ISerializableDummy<> dummy{};

    EXPECT_EQ(dataClient.handleData(another_yet_interface::SimpleStruct<>{}, dummy), Status::kErrorNotSupportedProtocolVersion);
    EXPECT_EQ(dataClient.getLoopCount(), 2);
}

TEST(MessagingTests, DataServiceServerTest)
{
    SimpleDataClient dataClient;
    csp::messaging::DataServiceServer<ft_helpers::DataServiceServerBase> serviceServer;

    // Test of getting all availible interfaces on server
    csp::messaging::InterfacesList outInterfacesList;
    EXPECT_EQ(dataClient.handleData(csp::messaging::GetInterfacesList<>{}, outInterfacesList), Status::kNoError);

    csp::messaging::InterfacesList interfacesListReference;
    DataServiceServerBase::fillInterfacesList(interfacesListReference.list);

    EXPECT_EQ(outInterfacesList, interfacesListReference);

    // Test of getting properties of single interface on server
    csp::messaging::GetInterfaceProperties getInterfaceProps;
    getInterfaceProps.id = outInterfacesList.list[outInterfacesList.list.size()-1].id;
    csp::messaging::OurGetInterfaceProperties outGetInterfaceProps;

    EXPECT_EQ(dataClient.handleData(getInterfaceProps, outGetInterfaceProps), Status::kNoError);
    EXPECT_EQ(outGetInterfaceProps.properties, outInterfacesList.list[outInterfacesList.list.size() - 1]);

    // Test of getting properties of single unknown/invalid_id interface on server
    getInterfaceProps.id = getInterfaceProps.getId();

    EXPECT_EQ(dataClient.handleData(getInterfaceProps, outGetInterfaceProps), Status::kNoError);

    csp::traits::InterfaceProperties interfacePropsReference;
    interfacePropsReference.id = getInterfaceProps.id;
    interfacePropsReference.version = csp::traits::kInterfaceVersionUndefined;

    EXPECT_EQ(outGetInterfaceProps.properties, interfacePropsReference);
}


class TestSubscriber
    : csp::messaging::IMethodDataServer<SimpleAssignableAlignedToOne<>, SimpleAssignableDescendant<>
        , true, false, SimpleAssignableAlignedToOne<>::getMinimumInterfaceVersion(), 3>
    , csp::messaging::IStaticDataServer<TestSubscriber, Diamond<>, DynamicPolymorphic<>, false>
{
public:
    Status handleData(const SimpleAssignableAlignedToOne<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, SimpleAssignableDescendant<>& output) override
    {
        return Status::kNoError;
    }

    static Status handleDataStatic(const Diamond<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, DynamicPolymorphic<>& output)
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

    csp::processing::serializeHeaderContext(ctxIn);
    csp::processing::serializeInOutDataContext<SimpleAssignableAlignedToOne<>>(ctxIn);
    csp::processing::DataProcessor::serializeData(testInput, ctxIn);

    BinVector binOut;
    csp::messaging::CommonServer::handleMessage(bin, binOut);


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

    csp::messaging::DataServiceServer<ft_helpers::DataServiceServerBase> serviceServer;
    csp::messaging::InterfacesList list;
    serviceServer.handleDataStatic(csp::messaging::GetInterfacesList<>{}, nullptr, list);
}

} // namespace anonymous
