/**
 * @file UnitTests/serializable_structs/tests_restricted_structs/include/tests_restricted_structs/processing/Generated/Serialize.h
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

#include <common_serialization/tests_restricted_structs/tests_restricted_structs.h>

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::SimplyAssignableAlignedToOne& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::SimplyAssignableAlignedToOne& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::SimplyAssignable& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::SimplyAssignable& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DynamicPolymorphic& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DynamicPolymorphic& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondBase& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondBase& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondEdge1& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondEdge1& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::DiamondEdge2& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::DiamondEdge2& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::TwoInts& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::TwoInts& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::RecursiveTestSpecial1& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::RecursiveTestSpecial1& value);

template<>
Status BodyProcessor::serialize(const tests_restricted_structs::RecursiveTestSpecial2& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_restricted_structs::RecursiveTestSpecial2& value);

} // namespace common_serialization::csp::processing::data