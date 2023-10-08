/**
 * @file SerializableConcepts.h
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

#include "../CsStatus.h"

namespace common_serialization
{

template<typename> class ISerializable;

namespace serialization_concepts
{

template<typename S>
concept ISerializationCapableContainer
    =  requires(S e)
         {
             typename S::value_type;
             typename S::size_type;

             { e.data() } -> std::same_as<typename S::value_type*>;
             { e.size() } -> std::same_as<typename S::size_type>;
             { e.capacity() } -> std::same_as<typename S::size_type>;

             { e.reserve(1) } -> std::same_as<Status>;
             { e.pushBackN(nullptr, 0) } -> std::same_as<Status>;
             { e.pushBackArithmeticValue(1ll) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename S::value_type, uint8_t>;

template<typename D>
concept IDeserializationCapableContainer
    =  requires(D e)
         {
             typename D::value_type;
             typename D::size_type;

             { e.data() } -> std::same_as<typename D::value_type*>;
             { e.size() } -> std::same_as<typename D::size_type>;

             // the next two functions are questionable so far
             { e.tell() } -> std::same_as<typename D::size_type>;
             { e.seek(0) } -> std::same_as<Status>;
             
             { e.read(nullptr, 0) } -> std::same_as<Status>;
             { e.readArithmeticValue(*(new int)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename D::value_type, uint8_t>;

template<typename T>
concept FixSizedArithmeticType
    =  std::is_same_v<std::remove_cv_t<T>, char8_t> || std::is_same_v<std::remove_cv_t<T>, char16_t> || std::is_same_v<std::remove_cv_t<T>, char32_t>
    || std::is_same_v<std::remove_cv_t<T>, float> || std::is_same_v<std::remove_cv_t<T>, double>;

// can be copied with memcpy (if alignments are the same and arithmetic types in are fixed length)
template<typename T>
concept SimpleAssignableType
    =  requires(T t) { typename T::simple_assignable; } 
    && std::is_same_v<typename T::simple_assignable, std::true_type>;

// can be copied with memcpy (if all arithmetic types in are fixed length)
template<typename T>
concept SimpleAssignableAlignedToOneType = SimpleAssignableType<T> && alignof(T) == 1;

template<typename T>
concept CompositeType 
    =  requires(T t) { typename T::composite_type; } 
    && std::is_same_v<typename T::composite_type, std::true_type>;

template<typename T>
concept EmptyType 
    =  requires(T t) { typename T::empty_type; }
    && std::is_same_v<typename T::empty_type, std::true_type>;

template<typename T>
concept IsISerializableBased = std::is_base_of_v<ISerializable<T>, T>;

} // namespace serialization_concepts

} // namespace common_serialization
