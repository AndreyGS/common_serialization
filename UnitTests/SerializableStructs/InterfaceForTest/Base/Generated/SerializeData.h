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

#include "../SpecialTypesSerializable.h"

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !statusSuccess(status))                            \
        return status;                                                          \
}

#define CONVERT_TO_OLD_IF_NEED(value, context)                                  \
{                                                                               \
    Status status = convertStructToOldIfNeed((value), (context));               \
                                                                                \
    if (status == Status::kNoFurtherProcessingRequired)                         \
        return Status::kNoError;                                                \
    else if (!statusSuccess(status))                                            \
        return status;                                                          \
}

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<>
constexpr Status DataProcessor::serializeData(const special_types::SimpleAssignableAlignedToOneSerializable<>& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    CONVERT_TO_OLD_IF_NEED(value, context);

    RUN(serializeDataHelper(value.m_x, context));
    RUN(serializeDataHelper(value.m_y, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DynamicPolymorphicNotSerializable& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(value.m_r, context));
    RUN(serializeDataHelper(value.m_arrR, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DynamicPolymorphicSerializable<>& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(value.m_o, context));
    RUN(serializeDataHelper(value.m_dpNS, context));

    RUN(serializeDataHelper(value.m_arrO, context));
    RUN(serializeDataHelper(value.m_arrDpNS, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondBaseNotSerializable& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(value.m_d0, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondEdge1NotSerializable& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(static_cast<const special_types::DiamondBaseNotSerializable&>(value), context));
    RUN(serializeDataHelper(value.m_d1, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondEdge2NotSerializable& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(static_cast<const special_types::DiamondBaseNotSerializable&>(value), context));
    RUN(serializeDataHelper(value.m_d2, context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::DiamondSerializable<>& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(static_cast<const special_types::DiamondEdge1NotSerializable&>(value), context));
    RUN(serializeDataHelper(static_cast<const special_types::DiamondEdge2NotSerializable&>(value), context));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const special_types::SpecialProcessingTypeContainSerializable<>& value
    , context::Data<Vector<uint8_t>, std::unordered_map<const void*, size_t>>& context)
{
    RUN(serializeDataHelper(value.m_vec, context));

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization

#undef CONVERT_TO_OLD_IF_NEED

#undef RUN
