/**
 * @file common_serialization/tests_csp_interface/src/csp_processing_data/generated/VersionConverter.cpp
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

#include <common_serialization/csp_base/processing/data/VersionConverter.h>
#include <common_serialization/tests_csp_interface/tests_csp_interface.h>

namespace common_serialization::csp::processing::data
{

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::SimplyAssignableAlignedToOne<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableAlignedToOne<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::SimplyAssignableAlignedToOne_Version1<>
              , tests_csp_interface::SimplyAssignableAlignedToOne_Version0<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::SimplyAssignableAlignedToOne<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableAlignedToOne<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::SimplyAssignableAlignedToOne_Version0<>
              , tests_csp_interface::SimplyAssignableAlignedToOne_Version1<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::SimplyAssignable<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::SimplyAssignable_Version0<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::SimplyAssignable<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::SimplyAssignable_Version0<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::SimplyAssignableDescendant<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableDescendant<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::SimplyAssignableDescendant_Version0<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::SimplyAssignableDescendant<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableDescendant<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::SimplyAssignableDescendant_Version0<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::AlwaysSimplyAssignable<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::AlwaysSimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::AlwaysSimplyAssignable_Version0<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::AlwaysSimplyAssignable<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::AlwaysSimplyAssignable<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::AlwaysSimplyAssignable_Version0<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::SimplyAssignableFixedSize<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableFixedSize<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::SimplyAssignableFixedSize_Version1<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::SimplyAssignableFixedSize<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::SimplyAssignableFixedSize<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::SimplyAssignableFixedSize_Version1<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::toOldStruct(const tests_csp_interface::DForAllModesTests<>& value, uint32_t targetVersion, context::SData& ctx)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::DForAllModesTests<>::getLatestPrivateVersion() != targetVersion);

    ToVersion<
                tests_csp_interface::SForAllModesTests_Version2<>
              , tests_csp_interface::SForAllModesTests_Version0<>
    > convertTo(targetVersion);

    AGS_CS_RUN(convertTo.convert(value, ctx));

    return Status::NoFurtherProcessingRequired;
}

template<>
Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, tests_csp_interface::DForAllModesTests<>& value)
{
    // If value version is the same as targetVersion there is a programmatic error that we are here
    assert(tests_csp_interface::DForAllModesTests<>::getLatestPrivateVersion() != targetVersion);

    FromVersion<
                tests_csp_interface::SForAllModesTests_Version0<>
              , tests_csp_interface::SForAllModesTests_Version2<>
    > convertFrom(targetVersion);

    AGS_CS_RUN(convertFrom.convert(ctx, value));

    return Status::NoFurtherProcessingRequired;
}

} // namespace common_serialization::csp::processing::data
