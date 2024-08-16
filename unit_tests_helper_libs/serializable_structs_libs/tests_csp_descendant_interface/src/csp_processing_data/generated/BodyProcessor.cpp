/**
 * @file common_serialization/tests_csp_descendant_interface/src/csp_processing_data/generated/BodyProcessor.cpp
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

#include <tests_csp_descendant_interface/Interface.h>

namespace common_serialization::csp::processing::data
{


template<>
Status BodyProcessor::serialize(const tests_csp_descendant_interface::SimpleStruct<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.m_i, ctx));


    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_descendant_interface::SimpleStruct<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.m_i));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_csp_descendant_interface::DiamondDescendant<>& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(static_cast<const tests_csp_interface::Diamond<>&>(value), ctx));

    CS_RUN(serialize(value.m_sSt, ctx));
    CS_RUN(serialize(value.m_ldouble, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_descendant_interface::DiamondDescendant<>& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<tests_csp_interface::Diamond<>&>(value)));

    CS_RUN(deserialize(ctx, value.m_sSt));
    CS_RUN(deserialize(ctx, value.m_ldouble));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
