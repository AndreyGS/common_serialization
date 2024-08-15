/**
 * @file UnitTests/serializable_structs/tests_restricted_structs/src/processing/Generated/Serialize.cpp
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

#include <tests_restricted_structs/NotInterface.h>

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::SimplyAssignableAlignedToOne& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.a, ctx));
    CS_RUN(serialize(value.s, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::SimplyAssignableAlignedToOne& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, value.a));
    CS_RUN(deserialize(ctx, value.s));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::SimplyAssignable& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.q, ctx));
    CS_RUN(serialize(value.w, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::SimplyAssignable& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, value.q));
    CS_RUN(deserialize(ctx, value.w));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DynamicPolymorphic& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.m_r, ctx));
    CS_RUN(serialize(value.m_arrR, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DynamicPolymorphic& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, value.m_r));
    CS_RUN(deserialize(ctx, value.m_arrR));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondBase& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.m_d0, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondBase& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, value.m_d0));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondEdge1& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(static_cast<const tests_restricted_structs::DiamondBase&>(value), ctx));
    CS_RUN(serialize(value.m_d1, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondEdge1& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<tests_restricted_structs::DiamondBase&>(value)));
    CS_RUN(deserialize(ctx, value.m_d1));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondEdge2& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(static_cast<const tests_restricted_structs::DiamondBase&>(value), ctx));
    CS_RUN(serialize(value.m_d2, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondEdge2& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, static_cast<tests_restricted_structs::DiamondBase&>(value)));
    CS_RUN(deserialize(ctx, value.m_d2));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::TwoInts& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.x, ctx));
    CS_RUN(serialize(value.y, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::TwoInts& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value);

    CS_RUN(deserialize(ctx, value.x));
    CS_RUN(deserialize(ctx, value.y));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::RecursiveTestSpecial1& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.pAny, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::RecursiveTestSpecial1& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.pAny));

    return Status::NoError;
}

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::RecursiveTestSpecial2& value, context::SData& ctx)
{
    CSP_SERIALIZE_COMMON(value, ctx);

    CS_RUN(serialize(value.pI, ctx));
    CS_RUN(serialize(value.pNext, ctx));
    CS_RUN(serialize(value.pAny, ctx));

    return Status::NoError;
}

template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::RecursiveTestSpecial2& value)
{
    CSP_DESERIALIZE_COMMON(ctx, value);

    CS_RUN(deserialize(ctx, value.pI));
    CS_RUN(deserialize(ctx, value.pNext));
    CS_RUN(deserialize(ctx, value.pAny));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
