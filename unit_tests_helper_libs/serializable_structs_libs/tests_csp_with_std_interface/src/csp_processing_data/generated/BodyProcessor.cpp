/**
 * @file common_serialization/tests_csp_with_std_interface/src/csp_processing_data/generated/BodyProcessor.cpp
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

#include <common_serialization/tests_csp_with_std_interface/csp_processing_data/generated/BodyProcessor.h>
#include <common_serialization/csp_base/processing/data/VersionConverter.h>

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::serialize(const tests_csp_with_std_interface::OneBigType<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    AGS_CS_RUN(serialize(value.m_string1, ctx));
    AGS_CS_RUN(serialize(value.m_string2, ctx));
    AGS_CS_RUN(serialize(value.m_vector1, ctx));
    AGS_CS_RUN(serialize(value.m_vector2, ctx));
    AGS_CS_RUN(serialize(value.m_map1, ctx));
    AGS_CS_RUN(serialize(value.m_map2, ctx));
    AGS_CS_RUN(serialize(value.m_tuple1, ctx));
    AGS_CS_RUN(serialize(value.m_tuple2, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_with_std_interface::OneBigType<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    AGS_CS_RUN(deserialize(ctx, value.m_string1));
    AGS_CS_RUN(deserialize(ctx, value.m_string2));
    AGS_CS_RUN(deserialize(ctx, value.m_vector1));
    AGS_CS_RUN(deserialize(ctx, value.m_vector2));
    AGS_CS_RUN(deserialize(ctx, value.m_map1));
    AGS_CS_RUN(deserialize(ctx, value.m_map2));
    AGS_CS_RUN(deserialize(ctx, value.m_tuple1));
    AGS_CS_RUN(deserialize(ctx, value.m_tuple2));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
