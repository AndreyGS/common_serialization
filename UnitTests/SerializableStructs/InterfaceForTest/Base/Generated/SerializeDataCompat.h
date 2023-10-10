/**
 * @file SerializeDataCompat.h
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

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

namespace common_serialization
{

template<>
constexpr Status SerializationProcessor::serializeDataCompat(const special_types::SimpleAssignableAlignedToOneSerializable<>& value, CsProtocolFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, std::unordered_map<const void*, size_t>& pointersMap, Vector<uint8_t>& output)
{
    Status status = convertStructToOldIfNeed(value, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);

    if (status == Status::kNoFurtherProcessingRequired)
        return Status::kNoError;
    else if (!statusSuccess(status))
        return status;

    RUN(serializeDataCompatHelper(value.m_x, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output));
    RUN(serializeDataCompatHelper(value.m_y, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output));

    return Status::kNoError;
}

} // namespace common_serialization

#undef RUN
