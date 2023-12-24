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

TEST(MessagingTests, CommonTests)
{
    TunnedDataClient dataClient;
    FourthDataServer dataServer;

    // Test is for IDataClient::getServerProtocolVersions and CommonServer handling
    csp::messaging::SupportedProtocolVersions<> supportedProtocolVersions;
    dataClient.getServerProtocolVersions(supportedProtocolVersions);

    csp::messaging::SupportedProtocolVersions<> supportedProtocolVersionsReference;
    supportedProtocolVersionsReference.list.pushBackN(csp::traits::kProtocolVersions, std::size(csp::traits::kProtocolVersions));

    EXPECT_EQ(supportedProtocolVersionsReference.list, supportedProtocolVersions.list);

    // Test for unsupported protocol version reactions
    auto preOperationFilter0_1 = [](BinWalker& input) -> void
    {
        csp::context::Common<BinWalker> ctx(input);
        csp::processing::deserializeHeaderContext(ctx);
        EXPECT_EQ(ctx.getProtocolVersion(), 1);

        input.seek(0);
        csp::protocol_version_t unsupportedProtocolVersion = 0;
        input.write(&unsupportedProtocolVersion, sizeof(unsupportedProtocolVersion));
        input.seek(0);
    };

    auto postOperationFilter1 = [](BinWalker& output) -> void
    {
        csp::context::Common<BinWalker> ctx(output);

        csp::processing::deserializeHeaderContext(ctx);
        EXPECT_EQ(ctx.getMessageType(), csp::context::Message::kStatus);
        EXPECT_EQ(ctx.getProtocolVersion(), 1);

        Status statusOut = Status::kNoError;
        csp::processing::deserializeStatusGetStatus(ctx.getBinaryData(), statusOut);
        EXPECT_EQ(Status::kErrorNotSupportedProtocolVersion, statusOut);

        csp::protocol_version_t protVersionsCount = 0;
        ctx.getBinaryData().readArithmeticValue(protVersionsCount);
        EXPECT_EQ(protVersionsCount, 1);

        csp::protocol_version_t protVersionOut = 0;
        ctx.getBinaryData().readArithmeticValue(protVersionOut);
        EXPECT_EQ(protVersionOut, 1);

        // Overwritting received protocol version
        csp::protocol_version_t unsupportedProtocolVersion = 0;
        output.seek(output.tell() - sizeof(protVersionOut));
        output.write(&unsupportedProtocolVersion, sizeof(unsupportedProtocolVersion));
        output.seek(0);
    };

    // Before sending to data to server, set protocol version to 0, which is invalid value
    dataClient.setFilterFunction(preOperationFilter0_1, true, 0);
    // Next we must receive Status::kErrorNotSupportedProtocolVersion with versions that server supports
    // And as we not modified server response it will contain valid values
    // Which in turn we also change to 0, like in the first time
    dataClient.setFilterFunction(preOperationFilter0_1, true, 1);
    // Again we will receive Status::kErrorNotSupportedProtocolVersion from server
    // And now we modifying it output by placing invalid version in messaging::StatusErrorNotSupportedProtocolVersion response struct
    dataClient.setFilterFunction(postOperationFilter1, false, 1);

    cs::csp::messaging::ISerializableDummy<> dummy{};

    EXPECT_EQ(dataClient.handleData(another_yet_interface::SimpleStruct<>{}, dummy), Status::kErrorNotSupportedProtocolVersion);
    EXPECT_EQ(dataClient.getLoopCount(), 2);

    // Now test situation when we first response ouput header protocol version is include invalid value
    dataClient.resetLoopCount();
    auto postOperationFilter0 = preOperationFilter0_1;
    dataClient.setFilterFunction(postOperationFilter0, false, 0);

    EXPECT_EQ(dataClient.handleData(another_yet_interface::SimpleStruct<>{}, dummy), Status::kErrorNotSupportedProtocolVersion);
    EXPECT_EQ(dataClient.getLoopCount(), 1);
}

TEST(MessagingTests, DataServiceServerTest)
{
    // Create client (to request on data server)
    SimpleDataClient dataClient;

    // Create DataServiceServer (to response on client requests)
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
}


class TestSubscriber
    : csp::messaging::IMethodDataServer<SimpleAssignableAlignedToOne<>, SimpleAssignableDescendant<>
        , true, false, SimpleAssignableAlignedToOne<>::getOriginPrivateVersion(), 3>
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
