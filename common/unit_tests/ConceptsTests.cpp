/**
 * @file common_serializaiton/common/unit_tests/ConceptsTests.cpp
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

#include <gtest/gtest.h>
#include <common_serialization/common/concepts.h>

using namespace common_serialization;

namespace
{

Status getRunStatusResult(Status input)
{
    AGS_CS_RUN(input);
    return Status::NoError;
}

Status getRunStatusResult2(Status input)
{
    AGS_CS_RUN(input);
    return Status::ErrorInvalidHash;
}

TEST(StatusTests, AgsCsRun)
{
    EXPECT_EQ(getRunStatusResult(Status::NoError), Status::NoError);
    EXPECT_EQ(getRunStatusResult(Status::NoFurtherProcessingRequired), Status::NoError);
    EXPECT_EQ(getRunStatusResult(Status::ErrorNoMemory), Status::ErrorNoMemory);
    
    EXPECT_EQ(getRunStatusResult2(Status::NoError), Status::ErrorInvalidHash);
}

Status getSetNewErrorStatusResult(Status status, Status input)
{
    AGS_CS_SET_NEW_ERROR(input);
    return status;
}

TEST(StatusTests, AgsCsSetNewError)
{
    Status status = Status::NoError;
    AGS_CS_SET_NEW_ERROR(Status::NoError);

    EXPECT_EQ(status, Status::NoError);

    AGS_CS_SET_NEW_ERROR(Status::NoFurtherProcessingRequired);
    EXPECT_EQ(status, Status::NoError);

    AGS_CS_SET_NEW_ERROR(Status::ErrorNoMemory);
    EXPECT_EQ(status, Status::ErrorNoMemory);

    AGS_CS_SET_NEW_ERROR(Status::NoError);
    EXPECT_EQ(status, Status::ErrorNoMemory);

    AGS_CS_SET_NEW_ERROR(Status::NoFurtherProcessingRequired);
    EXPECT_EQ(status, Status::ErrorNoMemory);

    status = Status::NoFurtherProcessingRequired;

    AGS_CS_SET_NEW_ERROR(Status::NoError);
    EXPECT_EQ(status, Status::NoFurtherProcessingRequired);

    AGS_CS_SET_NEW_ERROR(Status::ErrorNoMemory);
    EXPECT_EQ(status, Status::ErrorNoMemory);
}

TEST(StatusTests, StatusSuccess)
{
    EXPECT_TRUE(statusSuccess(Status::NoError));
    EXPECT_TRUE(statusSuccess(Status::NoFurtherProcessingRequired));
    EXPECT_FALSE(statusSuccess(Status::ErrorNoMemory));
}

} // namespace
