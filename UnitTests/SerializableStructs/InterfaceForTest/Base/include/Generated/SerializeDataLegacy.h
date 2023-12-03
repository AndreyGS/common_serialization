/**
 * @file SerializeDataLegacy.h
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

#include "../../../Base/include/SpecialTypesSerializableLegacy.h"

namespace common_serialization::csp::processing
{

template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSerializable_Version0<>& value
    , context::SData<>& ctx);
template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableSerializable_Version0<>& value
    , context::SData<>& ctx);
template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableDescendantSerializable_Version0<>& value
    , context::SData<>& ctx);
template<>
Status DataProcessor::serializeData(const special_types::SForAllModesTests_Version0<>& value
    , context::SData<>& ctx);
template<>
Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSerializable_Version1<>& value
    , context::SData<>& ctx);
template<>
Status DataProcessor::serializeData(const special_types::SForAllModesTests_Version2<>& value
    , context::SData<>& ctx);

} // namespace common_serialization::csp::processing

#undef RUN
