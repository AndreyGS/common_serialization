/**
 * @file SerializableConcepts.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

namespace common_serialization
{

namespace serializable_concepts
{

template<typename E>
concept ISerializationCapableContainer
    =  requires(E e)
         {
             typename E::value_type;
             typename E::size_type;

             { e.data() } -> std::same_as<typename E::value_type*>;
             { e.size() } -> std::same_as<typename E::size_type>;
             { e.capacity() } -> std::same_as<typename E::size_type>;

             { e.reserve(1) } -> std::same_as<bool>;
             { e.push_back_n(nullptr, 0) };
             { e.pushBackArithmeticValue(1ll) };
         } 
    && std::is_same_v<typename E::value_type, uint8_t>;

template<typename E>
concept IDeserializationCapableContainer = true;

// There is some limitations of deducing the base of serializable class because of lack of std libs,
// for instance, in kernel modes. So we need to add some additional concepts that indicate
// that this instance Serializable<T> type or not.

template<typename T>
concept SimpleAssignableAlignedToOneType
    =  requires(T t) { typename T::simple_assignable; } 
    && std::is_same_v<typename T::simple_assignable, std::true_type> 
    && alignof(T) == 1 ;

template<typename T>
concept CompositeType 
    =  requires(T t) { typename T::composite_type; } 
    && std::is_same_v<typename T::composite_type, std::true_type>;

template<typename T>
concept EmptyType 
    =  requires(T t) { typename T::empty_type; }
    && std::is_same_v<typename T::empty_type, std::true_type>;

template<typename T>
concept NotInheritedFromSerializableType
    =  requires(T t) { typename T::not_inherited_from_serializable; } 
    && std::is_same_v<typename T::not_inherited_from_serializable, std::true_type>;

} // namespace serializable_concepts

} // namespace common_serialization
