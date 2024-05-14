/**
 * @file StdIncludedTests.cpp
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

// In most of this tests we are simulating difference in arithmetic sizes by using
// distinct structs for serialization and deserialization
// (to accomplish that we set their name hashes to the same value)

namespace
{

using namespace common_serialization;
using namespace with_std_included_interface;
using namespace ft_helpers;

TEST(StdIncludedTests, MainT)
{
    OneBigType<> input;
    fillingStruct(input);

    BinWalkerT bin;
    EXPECT_EQ(input.serialize(bin.getVector()), Status::kNoError);

    OneBigType<> output;
    EXPECT_EQ(output.deserialize(bin), Status::kNoError);

    EXPECT_EQ(input, output);

    cleanAfterStruct(input);
}

} // namespace
