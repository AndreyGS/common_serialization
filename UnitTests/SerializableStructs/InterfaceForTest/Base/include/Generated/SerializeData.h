/**
 * @file SerializeData.h
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

#include "../../../Base/include/SpecialTypesSerializable.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
Status DataProcessor::serializeData(const special_types::TwoInts& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneNotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableNotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableDescendantSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DynamicPolymorphicNotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DynamicPolymorphicSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DiamondBaseNotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DiamondEdge1NotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DiamondEdge2NotSerializable& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DiamondSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SpecialProcessingTypeContainSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSimilarType1Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSimilarType2Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableSimilarType1Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableSimilarType2Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimilarType1Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::SimilarType2Serializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::RecursiveTestSpecial1& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::RecursiveTestSpecial2& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::ManyPointersTypeSerializable<>& value
    , context::SData<>& ctx);

template<>
Status DataProcessor::serializeData(const special_types::DForAllModesTests<>& value
    , context::SData<>& ctx);


} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef SERIALIZE_COMMON

#undef RUN
