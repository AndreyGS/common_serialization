/**
 * @file ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff.cpp
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
using namespace interface_for_test;
using namespace ft_helpers;

template<typename T>
void mainTest()
{
    T input;
    fillingStruct(input);

    BinWalker bin;
    csp::context::SData<> ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kSimplyAssignableTagsOptimizationsAreTurnedOff));
    EXPECT_EQ(input.serialize(ctxIn), Status::kNoError);

    T output;
    EXPECT_EQ(output.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);
}

TEST(ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff, SimplyAssignableAlignedToOneT)
{
    mainTest<SimplyAssignableAlignedToOne<>>();
}

TEST(ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff, SimplyAssignableT)
{
    mainTest<SimplyAssignable<>>();
}

TEST(ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff, AlwaysSimplyAssignableT)
{
    mainTest<AlwaysSimplyAssignable<>>();
}

TEST(ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff, SimplyAssignableFixedSizeT)
{
    mainTest<SimplyAssignableFixedSize<>>();
}


TEST(ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff, ContainSimplyAssignableWithoutSerializationFunctionsT)
{
    ContainSimplyAssignableWithoutSerializationFunctions<> input;
    fillingStruct(input);

    BinWalker bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::kNoError);

    ContainSimplyAssignableWithoutSerializationFunctions<> output;
    EXPECT_EQ(output.deserialize(bin), Status::kNoError);
    EXPECT_EQ(bin.tell(), bin.size());

    EXPECT_EQ(input, output);

    bin.clear();
    csp::context::SData<> ctxIn(bin.getVector());
    ctxIn.setDataFlags(csp::context::DataFlags(csp::context::DataFlags::kSimplyAssignableTagsOptimizationsAreTurnedOff));
    EXPECT_EQ(input.serialize(ctxIn), Status::kErrorNotSupportedSerializationSettingsForStruct);
}


} // namespace
