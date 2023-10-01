/**
 * @file SerializationForwardDeclarationsGenerated.h
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

#include "Status.h"
#include "Serialization/ISerializable.h"

namespace special_types 
{

class DynamicPolymorphicNotSerializable;
template<typename> class DynamicPolymorphicSerializable;

template<typename> struct TemplateNotSerializable;
template<typename, typename> class TemplateSerializable;

struct DiamondBaseNotSerializable;
struct DiamondEdge1NotSerializable;
struct DiamondEdge2NotSerializable;
template<typename> class DiamondSerializable;

template<typename, typename> class SpecialProcessingTypeContainSerializable;

}

namespace common_serialization
{

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DynamicPolymorphicNotSerializable & value, S& output);
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DynamicPolymorphicNotSerializable& value);

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DynamicPolymorphicSerializable<T>& value, S& output);
template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D & input, special_types::DynamicPolymorphicSerializable<T>&value);

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::TemplateNotSerializable<T>& value, S& output);
template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::TemplateNotSerializable<T>& value);

template<typename T, typename X, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::TemplateSerializable<T, X>& value, S& output);
template<typename T, typename X, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::TemplateSerializable<T, X>& value);

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondBaseNotSerializable& value, S& output);
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondBaseNotSerializable& value);

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondEdge1NotSerializable& value, S& output);
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondEdge1NotSerializable& value);

template<serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondEdge2NotSerializable& value, S& output);
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondEdge2NotSerializable& value);

template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::DiamondSerializable<T>& value, S& output);
template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::DiamondSerializable<T>& value);

template<typename X, typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr Status serializeThis(const special_types::SpecialProcessingTypeContainSerializable<X, T>& value, S& output);
template<typename X, typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, special_types::SpecialProcessingTypeContainSerializable<X, T>& value);

}
