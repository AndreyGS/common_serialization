/**
 * @file UnitTests/for_tests_helpers/include/ft_helpers/FillingStruct.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

namespace ft_helpers
{

template<typename T>
void fillingStruct(T& output);

using namespace common_serialization;

template<>
void fillingStruct(restricted_structs::SimplyAssignableAlignedToOne& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOne_Version0<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOne<>& output);
template<>
void fillingStruct(restricted_structs::SimplyAssignable& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignable_Version0<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignable<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableDescendant_Version0<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableDescendant<>& output);
template<>
void fillingStruct(interface_for_test::AlwaysSimplyAssignable_Version0<>& output);
template<>
void fillingStruct(interface_for_test::AlwaysSimplyAssignable<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableFixedSize_Version1<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableFixedSize<>& output);
template<>
void fillingStruct(restricted_structs::DynamicPolymorphic& output);
template<>
void fillingStruct(interface_for_test::DynamicPolymorphic<>& output);
template<>
void fillingStruct(restricted_structs::DiamondBase& output);
template<>
void fillingStruct(restricted_structs::DiamondEdge1& output);
template<>
void fillingStruct(restricted_structs::DiamondEdge2& output);
template<>
void fillingStruct(interface_for_test::Diamond<>& output);
template<>
void fillingStruct(interface_for_test::SpecialProcessingType<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOneSimilarType1<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOneSimilarType2<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableSimilarType1<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableSimilarType2<>& output);
template<>
void fillingStruct(interface_for_test::SimilarType1<>& output);
template<>
void fillingStruct(interface_for_test::SimilarType2<>& output);
template<>
void fillingStruct(interface_for_test::ManyPointersType<>& output);
template<>
void fillingStruct(interface_for_test::SForAllModesTests_Version0<>& output);
template<>
void fillingStruct(interface_for_test::SForAllModesTests_Version2<>& output);
template<>
void fillingStruct(interface_for_test::DForAllModesTests<>& output);
template<>
void fillingStruct(descendant_interface::SimpleStruct<>& output);
template<>
void fillingStruct(descendant_interface::DiamondDescendant<>& output);
template<>
void fillingStruct(another_yet_interface::SimpleStruct<>& output);
template<>
void fillingStruct(interface_for_test::SimplyAssignableWithoutSerializationFunctions<>& output);
template<>
void fillingStruct(restricted_structs::SimplyAssignableWithoutSerializationFunctions& output);
template<>
void fillingStruct(interface_for_test::ContainSimplyAssignableWithoutSerializationFunctions<>& output);
template<>
void fillingStruct(std::string& output);
template<>
void fillingStruct(std::wstring& output);
template<>
void fillingStruct(with_std_included_interface::OneBigType<>& output);

} // namespace ft_helpers