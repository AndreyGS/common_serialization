/**
 * @file DeserializeCommon.h
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

#include "ISerializable.h"

namespace common_serialization
{

// common function for pointers of known size
template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, size_t n, T* p)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serializable_concepts::SimpleAssignableType<T>)
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize));
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(deserializeThis(input, *(new (&p[i]) T)));
    }

    return Status::kNoError;
}

template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, size_t n, SerializationFlags flags, T* p)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if (
           std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serializable_concepts::SimpleAssignableAlignedToOneType<T> || serializable_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if ((  std::is_arithmetic_v<T> && !serializable_concepts::FixSizedArithmeticType<T> 
            || std::is_enum_v<T> && !serializable_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
                && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                for (size_t i = 0; i < n; ++i)
                    RUN(deserializeThis<originalTypeSize>(input, p[i]));

                return Status::kNoError;
            }
        }

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize));
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(deserializeThis(input, *(new (&p[i]) T)));
    }

    return Status::kNoError;
}

// common function for arrays input
template<typename T, size_t N, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, T(&arr)[N])
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serializable_concepts::SimpleAssignableType<T>)
    {
        constexpr typename D::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array output of deserializeThis");

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(arr)), bytesSize));
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (const auto& e : arr)
            RUN(deserializeThis(input, e));
    }

    return Status::kNoError;
}

template<typename T, size_t N, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, SerializationFlags flags, T(&arr)[N])
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be deserialized");

    if (
           std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serializable_concepts::SimpleAssignableAlignedToOneType<T> || serializable_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        constexpr typename T::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array output of deserializeThis");

        if ((  std::is_arithmetic_v<T> && !serializable_concepts::FixSizedArithmeticType<T> 
            || std::is_enum_v<T> && !serializable_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
                && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                for (size_t i = 0; i < N; ++i)
                    RUN(deserializeThis<originalTypeSize>(input, arr[i]));

                return Status::kNoError;
            }
        }

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(arr)), bytesSize));
    }
    else if constexpr (!serializable_concepts::EmptyType<T>)
    {
        for (const auto& e : arr)
            RUN(deserializeThis(input, e));
    }

    return Status::kNoError;
}

// common function for scalar input
template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, T& value)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
        RUN(input.readArithmeticValue(value))
    else
        static_assert(serializable_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

template<typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, SerializationFlags flags, T& value)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if ((  std::is_arithmetic_v<T> && !serializable_concepts::FixSizedArithmeticType<T> 
            || std::is_enum_v<T> && !serializable_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
                && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                RUN(deserializeThis<originalTypeSize>(input, value));
                
                return 0;
            }
        }

        RUN(input.readArithmeticValue(value));
    }
    else
        static_assert(serializable_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

template<size_t OriginalTypeSize, typename T, serializable_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeThis(D& input, T& value)
{
    static_assert( std::is_arithmetic_v<T> && !serializable_concepts::FixSizedArithmeticType<T>
                || std::is_enum_v<T> && !serializable_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>
        , "Current deserializeThis function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    uint8_t arr[OriginalTypeSize] = { 0 };
    RUN(input.read(arr));
    value = *static_cast<T*>(static_cast<void*>(arr));

    return Status::kNoError;
}

} // common_serialization
