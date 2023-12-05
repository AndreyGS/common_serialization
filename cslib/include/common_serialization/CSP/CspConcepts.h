/**
 * @file cslib/include/common_serialization/CSP/CspConcepts.h
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

#include "common_serialization/Status.h"
#include "common_serialization/Containers/GenericPointerKeeper.h"

namespace common_serialization::csp
{

template<typename> class ISerializable;

template<typename S>
concept ISerializationCapableContainer
    =  requires(S e)
         {
             typename S::value_type;
             typename S::size_type;

             { e.clear() };
             { e.data() } -> std::same_as<typename S::value_type*>;
             { e.size() } -> std::same_as<typename S::size_type>;
             { e.capacity() } -> std::same_as<typename S::size_type>;

             { e.reserve(1) } -> std::same_as<Status>;
             { e.pushBackN(nullptr, static_cast<typename S::size_type>(0)) } -> std::same_as<Status>;
             { e.pushBackArithmeticValue(1ull) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename S::value_type, uint8_t>;

template<typename D>
concept IDeserializationCapableContainer
    =  requires(D e)
         {
             typename D::value_type;
             typename D::size_type;

             { e.clear() };
             { e.data() } -> std::same_as<typename D::value_type*>;
             { e.size() } -> std::same_as<typename D::size_type>;

             // the next two functions are questionable so far
             { e.tell() } -> std::same_as<typename D::size_type>;
             { e.seek(0) } -> std::same_as<Status>;
             
             { e.read(nullptr, static_cast<typename D::size_type>(0)) } -> std::same_as<Status>;
             { e.readArithmeticValue(*(new unsigned)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename D::value_type, uint8_t>;

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

template<typename T>
concept FixSizedArithmeticType
    =  std::is_same_v<std::remove_cv_t<T>, char8_t> || std::is_same_v<std::remove_cv_t<T>, char16_t> || std::is_same_v<std::remove_cv_t<T>, char32_t>
    || std::is_same_v<std::remove_cv_t<T>, float> || std::is_same_v<std::remove_cv_t<T>, double>;

template<typename T>
concept FixSizedEnumType = std::is_enum_v<T> && FixSizedArithmeticType<std::underlying_type_t<T>>;

template<typename T>
using normalize_t = std::remove_const_t<std::remove_reference_t<T>>;

// can be copied with memcpy (if alignments are the same and arithmetic types in are fixed length)
template<typename T>
concept SimpleAssignableType
    =  requires(T t) { typename normalize_t<T>::simple_assignable_tag; };

// can be copied with memcpy (if all arithmetic types in are fixed length)
template<typename T>
concept SimpleAssignableAlignedToOneType = SimpleAssignableType<T> && alignof(T) == 1;

template<typename T>
concept EmptyType 
    =  requires(T t) { typename normalize_t<T>::empty_type_tag; };

template<typename T>
concept IsISerializableBased = std::is_base_of_v<csp::ISerializable<normalize_t<T>>, normalize_t<T>>;

template<typename T>
concept IsNotISerializableBased = !IsISerializableBased<T>;

} // namespace common_serialization::csp
