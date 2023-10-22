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

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
constexpr Status DataProcessor::convertFromOldStruct(context::DData<Walker<uint8_t>, std::unordered_map<size_t, void*>>& ctx
    , uint32_t thisVersionCompat, special_types::SimpleAssignableAlignedToOneSerializable<>& value)
{
    // If value version is the same as thisVersionCompat there is a programmatic error
    assert(value.getThisVersion() != thisVersionCompat);

    if (thisVersionCompat == 0)
    {
        special_types::SimpleAssignableAlignedToOneSerializable_Version0<> compatVersion;
        RUN(deserializeDataLegacy(ctx, compatVersion));

        value.m_x = compatVersion.m_ti.x;
        value.m_y = compatVersion.m_ti.y;
    }
    else if (thisVersionCompat == 1)
    {
        special_types::SimpleAssignableAlignedToOneSerializable_Version1<> compatVersion;
        RUN(deserializeDataLegacy(ctx, compatVersion));

        value.m_x = compatVersion.m_x;
        value.m_y = compatVersion.m_y;
    }

    return Status::kNoFurtherProcessingRequired;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef RUN