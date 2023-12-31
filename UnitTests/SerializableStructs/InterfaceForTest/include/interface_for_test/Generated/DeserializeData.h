/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/include/interface_for_test/Generated/DeserializeData.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

#include "interface_for_test/Structs.h"

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOne<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignable<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableDescendant<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::DynamicPolymorphic<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::Diamond<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SpecialProcessingType<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOneSimilarType1<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableAlignedToOneSimilarType2<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableSimilarType1<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimpleAssignableSimilarType2<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimilarType1<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::SimilarType2<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::RecursiveTestSpecial1& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::RecursiveTestSpecial2& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::ManyPointersType<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<>& ctx, interface_for_test::DForAllModesTests<>& value);

} // namespace common_serialization::csp::processing
