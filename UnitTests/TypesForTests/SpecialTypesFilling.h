/**
 * @file SpecialTypesFilling.h
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

#include "SpecialTypes.h"
#include "../SerializableStructs/InterfaceForTest/Base/Interface.h"

namespace special_types
{

template<typename T>
void fillingStruct(T& output);

void _SimpleAssignableAlignedToOneNotSerializable(SimpleAssignableAlignedToOneNotSerializable& output);
void _SimpleAssignableAlignedToOneSerializable(SimpleAssignableAlignedToOneSerializable<>& output);
void _SimpleAssignableNotSerializable(SimpleAssignableNotSerializable& output);
void _SimpleAssignableSerializable(SimpleAssignableSerializable<>& output);
void _SimpleAssignableDescendantSerializable(SimpleAssignableDescendantSerializable<>& output);
void _DynamicPolymorphicNotSerializable(DynamicPolymorphicNotSerializable& output);
void _DynamicPolymorphicSerializable(DynamicPolymorphicSerializable<>& output);
void _DiamondBaseNotSerializable(DiamondBaseNotSerializable& output);
void _DiamondEdge1NotSerializable(DiamondEdge1NotSerializable& output);
void _DiamondEdge2NotSerializable(DiamondEdge2NotSerializable& output);
void _DiamondSerializable(DiamondSerializable<>& output);
void _SpecialProcessingTypeContainSerializable(SpecialProcessingTypeContainSerializable<>& output);
void _SimpleAssignableAlignedToOneSimilarType1Serializable(SimpleAssignableAlignedToOneSimilarType1Serializable<>& output);
void _SimpleAssignableAlignedToOneSimilarType2Serializable(SimpleAssignableAlignedToOneSimilarType2Serializable<>& output);
void _SimpleAssignableSimilarType1Serializable(SimpleAssignableSimilarType1Serializable<>& output);
void _SimpleAssignableSimilarType2Serializable(SimpleAssignableSimilarType2Serializable<>& output);
void _SimilarType1Serializable(SimilarType1Serializable<>& output);
void _SimilarType2Serializable(SimilarType2Serializable<>& output);

} // namespace special_types