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
#include "Serialization/SerializeCommon.h"
#include "Serialization/SerializeSpecial.h"

namespace common_serialization
{

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DynamicPolymorphicNotSerializable& value, S& output)
{
    RUN(serializeThis(value.m_r, output));
    RUN(serializeThis(value.m_arrR, output));

    return Status::kNoError;
}

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DynamicPolymorphicSerializable<T>& value, S& output)
{
    RUN(serializeThis(value.m_o, output));
    RUN(serializeThis(value.m_dpNS, output));

    RUN(serializeThis(value.m_arrO, output));
    RUN(serializeThis(value.m_arrDpNS, output));

    return Status::kNoError;
}

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::TemplateNotSerializable<T>& value, S& output)
{
    RUN(serializeThis(value.m_f, output));
    RUN(serializeThis(value.m_g, output));

    return Status::kNoError;
}

template<typename X, typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::TemplateSerializable<X, T>& value, S& output)
{
    RUN(serializeThis(value.m_c, output));
    RUN(serializeThis(value.m_v, output));
    RUN(serializeThis(value.m_tNS, output));

    return Status::kNoError;
}

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondBaseNotSerializable& value, S& output)
{
    RUN(serializeThis(value.m_d0, output));

    return Status::kNoError;
}

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondEdge1NotSerializable& value, S& output)
{
    RUN(serializeThis(static_cast<const special_types::DiamondBaseNotSerializable&>(value), output));
    RUN(serializeThis(value.m_d1, output));

    return Status::kNoError;
}

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondEdge2NotSerializable& value, S& output)
{
    RUN(serializeThis(static_cast<const special_types::DiamondBaseNotSerializable&>(value), output));
    RUN(serializeThis(value.m_d2, output));

    return Status::kNoError;
}

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondSerializable<T>& value, S& output)
{
    RUN(serializeThis(static_cast<const special_types::DiamondEdge1NotSerializable&>(value), output));
    RUN(serializeThis(static_cast<const special_types::DiamondEdge2NotSerializable&>(value), output));

    return Status::kNoError;
}

template<typename X, typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::SpecialProcessingTypeContainSerializable<X, T>& value, S& output)
{
    RUN(serializeThis(value.m_vec, output));

    return Status::kNoError;
}


} // namespace common_serialization
