/**
 * @file cslib/include/common_serialization/csp/Concepts.h
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

#include "common_serialization/Containers/Concepts.h"
#include "common_serialization/csp/context/DataFlags.h"

namespace common_serialization::csp
{

template<typename> class ISerializable;

template<typename Sbin>
concept ISerializationBinContainer
    =  requires(Sbin e)
         {
             typename Sbin::value_type;
             typename Sbin::size_type;

             { e.clear() };
             { e.size() } -> std::same_as<typename Sbin::size_type>;

             { e.reserve(1) } -> std::same_as<Status>;

             { e.pushBackN(nullptr, static_cast<typename Sbin::size_type>(0)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename Sbin::value_type, uint8_t>;

template<typename Dbin>
concept IDeserializationBinContainer
    =  requires(Dbin e)
         {
             typename Dbin::value_type;
             typename Dbin::size_type;

             { e.clear() };
             { e.size() } -> std::same_as<typename Dbin::size_type>;

             { e.tell() } -> std::same_as<typename Dbin::size_type>;
             { e.seek(0) } -> std::same_as<Status>;
             
             { e.read(nullptr, static_cast<typename Dbin::size_type>(0)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename Dbin::value_type, uint8_t>;

template<typename PM>
concept ISerializationPointersMap
    = requires(PM pm)
        {
            typename PM::key_type;
            typename PM::mapped_type;
            
            { pm.find(nullptr) };
            { pm.end() };
            { pm[*(new typename PM::key_type)] } -> std::same_as<typename PM::mapped_type&>;
            { pm.clear() };
        }
    && std::is_same_v<typename PM::key_type, const void*> && std::is_same_v<typename PM::mapped_type, uint64_t>;

template<typename PM>
concept IDeserializationPointersMap
    = requires(PM pm)
        {
            typename PM::key_type;
            typename PM::mapped_type;
            
            { pm.find(1) };
            { pm.end() };
            { pm[*(new typename PM::key_type)] } -> std::same_as<typename PM::mapped_type&>;
            { pm.clear() };
        }
    && std::is_same_v<typename PM::key_type, uint64_t> && std::is_same_v<typename PM::mapped_type, void*>;

template<typename PM>
concept IPointersMap = ISerializationPointersMap<PM> || IDeserializationPointersMap<PM>;

// FixSizedArithmeticType is type that can't be changed in its size
// So long double is also match this criteria, despite its size is different
// on one or another platforms. Because of this, care must be taken
// to well-defined Interface with respect to long double size.
template<typename T>
concept FixSizedArithmeticType
    =  std::is_same_v<std::remove_cv_t<T>, char8_t> || std::is_same_v<std::remove_cv_t<T>, char16_t> || std::is_same_v<std::remove_cv_t<T>, char32_t>
    || std::is_floating_point_v<T>;

template<typename T>
concept FixSizedEnumType = std::is_enum_v<T> && FixSizedArithmeticType<std::underlying_type_t<T>>;

// can be copied with memcpy (if alignments are the same and no kSizeOfIntegersMayBeNotEqual flag is set)
template<typename T>
concept SimplyAssignable
    =  requires(T t) { typename normalize_t<T>::simply_assignable_tag; };

// can be copied with memcpy (if no kSizeOfIntegersMayBeNotEqual flag is set)
template<typename T>
concept SimplyAssignableAlignedToOne
    = requires(T t) { typename normalize_t<T>::simply_assignable_aligned_to_one_tag; } && alignof(T) == 1;

// all fields must be primitives an with strictly defined sizes regardless of execution environment
// can be copied with memcpy (if alignments are the same)
template<typename T>
concept SimplyAssignableFixedSize
    =  requires(T t) { typename normalize_t<T>::simply_assignable_fixed_size_tag; };

// same as FixedSizeSimplyAssignableType but with alignment to 1 requirement (type must always have same size)
// can be copied with memcpy
template<typename T>
concept AlwaysSimplyAssignable
    =  requires(T t) { typename normalize_t<T>::always_simply_assignable_tag; } && alignof(T) == 1;

template<typename T>
concept AnySimplyAssignable = AlwaysSimplyAssignable<T> || SimplyAssignableFixedSize<T> || SimplyAssignableAlignedToOne<T> || SimplyAssignable<T>;

template<typename T>
concept EndiannessTolerant 
    = requires(T t) { typename normalize_t<T>::endianness_tolerant_tag; } || sizeof(T) == 1;

template<typename T>
concept NotSimplyAssignable = !AnySimplyAssignable<T>;

template<typename T>
concept EmptyType 
    =  requires(T t) { typename normalize_t<T>::empty_type_tag; };

template<typename T>
concept ISerializableBased = std::is_base_of_v<csp::ISerializable<normalize_t<T>>, normalize_t<T>>;

template<typename T>
concept NotISerializableBased = !ISerializableBased<T>;

template<typename T>
concept StructHaveDataFlags = requires
{
    { T::getEffectiveMandatoryDataFlags() } -> std::same_as<context::DataFlags>;
    { T::getEffectiveForbiddenDataFlags() } -> std::same_as<context::DataFlags>;
};

} // namespace common_serialization::csp
