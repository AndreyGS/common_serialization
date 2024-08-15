/**
 * @file UnitTests/serializable_structs/tests_csp_interface/include/tests_csp_interface/processing/Generated/Serialize.h
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

namespace common_serialization::csp::processing::data
{

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOne<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOne<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignable<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignable<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableDescendant<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableDescendant<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::AlwaysSimplyAssignable<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::AlwaysSimplyAssignable<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableFixedSize<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableFixedSize<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::DynamicPolymorphic<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::DynamicPolymorphic<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::Diamond<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::Diamond<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SpecialProcessingType<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SpecialProcessingType<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOneSimilarType1<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOneSimilarType1<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableAlignedToOneSimilarType2<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableAlignedToOneSimilarType2<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableSimilarType1<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableSimilarType1<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimplyAssignableSimilarType2<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimplyAssignableSimilarType2<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimilarType1<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimilarType1<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::SimilarType2<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::SimilarType2<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ManyPointersType<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ManyPointersType<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::DForAllModesTests<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::DForAllModesTests<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainSimplyAssignableWithoutSerializationFunctions<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainSimplyAssignableWithoutSerializationFunctions<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainBigStructs1<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainBigStructs1<>& value);

template<>
Status BodyProcessor::serialize(const tests_csp_interface::ContainBigStructs2ForAllModes<>& value, context::SData& ctx);
template<>
Status BodyProcessor::deserialize(context::DData& ctx, tests_csp_interface::ContainBigStructs2ForAllModes<>& value);

} // namespace common_serialization::csp::processing::data
