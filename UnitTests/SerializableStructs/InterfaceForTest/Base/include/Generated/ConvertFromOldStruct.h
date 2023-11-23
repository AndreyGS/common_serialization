/**
 * @file ConvertFromOldStruct.h
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

#include "SpecialTypesSerializable.h"
#include "SpecialTypesSerializableLegacy.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
Status DataProcessor::convertFromOldStruct(context::DData<BinWalker, std::unordered_map<uint64_t, void*>>& ctx
    , uint32_t thisVersionCompat, special_types::SimpleAssignableAlignedToOneSerializable<>& value);
template<>
Status DataProcessor::convertFromOldStruct(context::DData<BinWalker, std::unordered_map<uint64_t, void*>>& ctx
    , uint32_t targetVersion, special_types::SimpleAssignableSerializable<>& value);
template<>
Status DataProcessor::convertFromOldStruct(context::DData<BinWalker, std::unordered_map<uint64_t, void*>>& ctx
    , uint32_t targetVersion, special_types::SimpleAssignableDescendantSerializable<>& value);
template<>
Status DataProcessor::convertFromOldStruct(context::DData<BinWalker, std::unordered_map<uint64_t, void*>>& ctx
    , uint32_t thisVersionCompat, special_types::DForAllModesTests<>& value);

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN