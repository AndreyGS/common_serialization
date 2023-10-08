/**
 * @file DeserializeGenerated.h
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

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !ST_SUCCESS(status))                               \
        return status;                                                          \
}

#include "SpecialTypesSerializable.h"

namespace common_serialization
{

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DynamicPolymorphicNotSerializable& value)
{
    RUN(deserializeDataHelper(input, value.m_r));
    RUN(deserializeDataHelper(input, value.m_arrR));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DynamicPolymorphicSerializable<>& value)
{
    RUN(deserializeDataHelper(input, value.m_o));
    RUN(deserializeDataHelper(input, value.m_dpNS));

    RUN(deserializeDataHelper(input, value.m_arrO));
    RUN(deserializeDataHelper(input, value.m_arrDpNS));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DiamondBaseNotSerializable& value)
{
    RUN(deserializeDataHelper(input, value.m_d0));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DiamondEdge1NotSerializable& value)
{
    RUN(deserializeDataHelper(input, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeDataHelper(input, value.m_d1));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DiamondEdge2NotSerializable& value)
{
    RUN(deserializeDataHelper(input, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeDataHelper(input, value.m_d2));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::DiamondSerializable<>& value)
{
    RUN(deserializeDataHelper(input, static_cast<special_types::DiamondEdge1NotSerializable&>(value)));
    RUN(deserializeDataHelper(input, static_cast<special_types::DiamondEdge2NotSerializable&>(value)));

    return Status::kNoError;
}

template<>
Status SerializationProcessor::deserializeData(Walker<uint8_t>& input, special_types::SpecialProcessingTypeContainSerializable<>& value)
{
    RUN(deserializeDataHelper(input, value.m_vec));

    return Status::kNoError;
}

} // namespace common_serialization

#undef RUN
