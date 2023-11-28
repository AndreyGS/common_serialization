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

using namespace special_types;


class TestSubscriber
    : csp::messaging::IMethodDataServer<SimpleAssignableAlignedToOneSerializable<>, SimpleAssignableDescendantSerializable<>
        , SimpleAssignableAlignedToOneSerializable<>::getMinimumInterfaceVersion(), 3, true, false>
    , csp::messaging::IStaticDataServer<TestSubscriber, DiamondSerializable<>, DynamicPolymorphicSerializable<>
        , DiamondSerializable<>::getMinimumInterfaceVersion(), DynamicPolymorphicSerializable<>::getMinimumInterfaceVersion(), true>

{
public:
    Status handleData(const SimpleAssignableAlignedToOneSerializable<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, SimpleAssignableDescendantSerializable<>& output) override
    {
        return Status::kNoError;
    }

    static Status handleDataStatic(const DiamondSerializable<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, DynamicPolymorphicSerializable<>& output)
    {
        return Status::kNoError;
    }
};

TEST(MessagingTests, Temp)
{
    TestSubscriber testSubs;

    SimpleAssignableAlignedToOneSerializable testInput;
    BinWalker bin;
    csp::context::SInOutData<> ctxIn(bin.getVector());
    ctxIn.setMessageType(csp::context::Message::kInOutData);
    ctxIn.setInterfaceVersion(1);
    ctxIn.setOutputInterfaceVersion(2);

    csp::processing::serializeHeaderContext(ctxIn);
    csp::processing::serializeInOutDataContext<SimpleAssignableAlignedToOneSerializable<>>(ctxIn);
    csp::processing::DataProcessor::serializeData(testInput, ctxIn);

    BinVector binOut;
    csp::messaging::CommonServer::handleMessage(bin, binOut);


    SimpleAssignableDescendantSerializable testOutput;

    BinWalker binIn;
    binIn.pushBack(1);
    

    Vector<csp::messaging::IDataServerBase*, RawGenericAllocatorHelper<csp::messaging::IDataServerBase*>> subscribers;
    csp::messaging::GetDataServersKeeper().findServers(testInput.getNameHash(), subscribers);
    //subscribers[0]->handleDataConcrete(binIn, binOut);

    testSubs.~TestSubscriber();

    DiamondSerializable testInput2;
    DynamicPolymorphicSerializable testOutput2;

    csp::messaging::GetDataServersKeeper().findServers(testInput2.getNameHash(), subscribers);
    
    //if (subscribers.size())
        //subscribers[0]->handleDataConcrete(binIn, binOut);

}

} // namespace anonymous
