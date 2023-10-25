/**
 * @file DeserializeData.h
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
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneNotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSerializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableNotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableSerializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DynamicPolymorphicNotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DynamicPolymorphicSerializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DiamondBaseNotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DiamondEdge1NotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DiamondEdge2NotSerializable& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::DiamondSerializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SpecialProcessingTypeContainSerializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSimilarType1Serializable<>& value);
template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableAlignedToOneSimilarType2Serializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableSimilarType1Serializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimpleAssignableSimilarType2Serializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimilarType1Serializable<>& value);

template<>
Status DataProcessor::deserializeData(context::DData<Walker<uint8_t>, std::unordered_map<uint64_t, void*>>& ctx
    , special_types::SimilarType2Serializable<>& value);

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN