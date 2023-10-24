/**
 * @file SpecialTypesSerializableVersionConverter.h
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

#include "SpecialTypesSerializableLegacy.h"

namespace special_types
{

using namespace common_serialization;
/*
class SerializableVersionConverter
    : public SerializableHandler<SimpleAssignableAlignedToOneSerializable_Version1<>, SimpleAssignableAlignedToOneSerializable_Version0<>>
    , public SerializableHandler<SimpleAssignableAlignedToOneSerializable<>, SimpleAssignableAlignedToOneSerializable_Version1<>>
{
public:
    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::ISerializationCapableContainer S>
    Status convertUp(D& input, S& output)

    template<serialization_concepts::ISerializationCapableContainer S>
    Status Process(const SimpleAssignableAlignedToOneSerializable_Legacy0<>& inStruct, SimpleAssignableAlignedToOneSerializable_Legacy1<>& outStruct) override
    {
        return Status::kNoError;
    }

    template<serialization_concepts::ISerializationCapableContainer S>
    Status Process(const SimpleAssignableAlignedToOneSerializable_Legacy1<>& inStruct, SimpleAssignableAlignedToOneSerializable_Legacy0<>& outStruct) override
    {
        return Status::kNoError;
    }
};*/

template<typename OutType, serialization_concepts::IDeserializationCapableContainer D>
Status convertThis(D& input, bool isUpConversion, OutType& outStuct)
{

}


} // namespace special_types
