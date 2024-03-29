/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/src/processing/Generated/ConvertFromOldStruct.cpp
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

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::SimplyAssignableAlignedToOne<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::SimplyAssignableAlignedToOne<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::SimplyAssignableAlignedToOne_Version0<>
                        , interface_for_test::SimplyAssignableAlignedToOne_Version1<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::SimplyAssignable<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::SimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::SimplyAssignable_Version0<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::SimplyAssignableDescendant<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::SimplyAssignableDescendant<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::SimplyAssignableDescendant_Version0<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::AlwaysSimplyAssignable<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::AlwaysSimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::AlwaysSimplyAssignable_Version0<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::SimplyAssignableFixedSize<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::SimplyAssignableFixedSize<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::SimplyAssignableFixedSize_Version1<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

template<>
Status DataProcessor::convertFromOldStruct(context::DData<>& ctx, uint32_t targetVersion, interface_for_test::DForAllModesTests<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(interface_for_test::DForAllModesTests<>::getLatestPrivateVersion() != targetVersion);

    FromVersionConverter<
                          interface_for_test::SForAllModesTests_Version0<>
                        , interface_for_test::SForAllModesTests_Version2<>
    > convertFrom(targetVersion);

    CS_RUN(convertFrom.convert(ctx, value));

    return Status::kNoFurtherProcessingRequired;
}

} // namespace common_serialization::csp::processing
