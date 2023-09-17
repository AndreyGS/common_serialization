/**
 * @file SerializeCommon.h
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

#include "ISerializable.h"

namespace common_serialization
{

// common function for pointers of known size
template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr int serializeThis(const T* value, size_t n, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serializable_concepts::SimpleAssignableAlignedToOneType<T>)
    {
        size_t bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        output.push_back_n(value, bytesSize);
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            serializeThis(value[i], output);
    }

    return 0;
}

// common function for arrays input
template<typename T, size_t N, serializable_concepts::ISerializationCapableContainer S>
constexpr int serializeThis(const T(&value)[N], S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serializable_concepts::SimpleAssignableAlignedToOneType<T>)
    {
        constexpr size_t bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeThis");

        output.push_back_n(value, bytesSize);
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (const auto& e : value)
            serializeThis(e, output);
    }

    return 0;
}

// common function for scalar input
template<typename T, serializable_concepts::ISerializationCapableContainer S>
constexpr int serializeThis(T value, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
        output.pushBackArithmeticValue(value);
    else
        static_assert(serializable_concepts::EmptyType<T>, "Type not supported");

    return 0;
}

} // common_serialization
