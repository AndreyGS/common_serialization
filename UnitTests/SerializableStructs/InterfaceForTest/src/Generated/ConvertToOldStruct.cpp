/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/Generated/ConvertToOldStruct.cpp
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

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::convertToOldStruct(const interface_for_test::SimpleAssignableAlignedToOne<>& value, uint32_t targetVersion, context::SData<>& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(value.getLatestPrivateVersion() != targetVersion);

    ToVersionConverter<
                        interface_for_test::SimpleAssignableAlignedToOne_Version1<>
                      , interface_for_test::SimpleAssignableAlignedToOne_Version0<>
    > convertTo(targetVersion);

    RUN(convertTo.convert(value, ctx));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertToOldStruct(const interface_for_test::SimpleAssignable<>& value, uint32_t targetVersion, context::SData<>& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(value.getLatestPrivateVersion() != targetVersion);

    ToVersionConverter<
                        interface_for_test::SimpleAssignable_Version0<>
    > convertTo(targetVersion);

    RUN(convertTo.convert(value, ctx));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertToOldStruct(const interface_for_test::SimpleAssignableDescendant<>& value, uint32_t targetVersion, context::SData<>& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(value.getLatestPrivateVersion() != targetVersion);

    ToVersionConverter<
                        interface_for_test::SimpleAssignableDescendant_Version0<>
    > convertTo(targetVersion);

    RUN(convertTo.convert(value, ctx));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertToOldStruct(const interface_for_test::DForAllModesTests<>& value, uint32_t targetVersion, context::SData<>& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(value.getLatestPrivateVersion() != targetVersion);

    ToVersionConverter<
                        interface_for_test::SForAllModesTests_Version2<>
                      , interface_for_test::SForAllModesTests_Version0<>
    > convertTo(targetVersion);

    RUN(convertTo.convert(value, ctx));

    return Status::kNoFurtherProcessingRequired;
}

} // namespace common_serialization::csp::processing

#undef RUN
