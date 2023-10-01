/**
 * @file SerializeGenerated.h
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
#include "Serialization/DeserializeCommon.h"
#include "Serialization/DeserializeSpecial.h"

namespace common_serialization
{

template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DynamicPolymorphicNotSerializable& value)
{
    RUN(deserializeThis(input, value.m_r));
    RUN(deserializeThis(input, value.m_arrR));

    return Status::kNoError;
}

template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DynamicPolymorphicSerializable<T>& value)
{
    RUN(deserializeThis(input, value.m_o));
    RUN(deserializeThis(input, value.m_dpNS));

    RUN(deserializeThis(input, value.m_arrO));
    RUN(deserializeThis(input, value.m_arrDpNS));

    return Status::kNoError;
}

template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::TemplateNotSerializable<T>& value)
{
    RUN(deserializeThis(input, value.m_f));
    RUN(deserializeThis(input, value.m_g));

    return Status::kNoError;
}

template<typename X, typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::TemplateSerializable<X, T>& value)
{
    RUN(deserializeThis(input, value.m_c));
    RUN(deserializeThis(input, value.m_v));
    RUN(deserializeThis(input, value.m_tNS));

    return Status::kNoError;
}

template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondBaseNotSerializable& value)
{
    RUN(deserializeThis(input, value.m_d0));

    return Status::kNoError;
}

template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondEdge1NotSerializable& value)
{
    RUN(deserializeThis(input, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeThis(input, value.m_d1));

    return Status::kNoError;
}

template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondEdge2NotSerializable& value)
{
    RUN(deserializeThis(input, static_cast<special_types::DiamondBaseNotSerializable&>(value)));
    RUN(deserializeThis(input, value.m_d2));

    return Status::kNoError;
}

template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondSerializable<T>& value)
{
    RUN(deserializeThis(input, static_cast<special_types::DiamondEdge1NotSerializable&>(value)));
    RUN(deserializeThis(input, static_cast<special_types::DiamondEdge2NotSerializable&>(value)));

    return Status::kNoError;
}

template<typename X, typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::SpecialProcessingTypeContainSerializable<X, T>& value)
{
    RUN(deserializeThis(input, value.m_vec));

    return Status::kNoError;
}

} // namespace common_serialization
