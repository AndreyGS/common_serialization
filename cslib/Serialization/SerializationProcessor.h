/**
 * @file SerializationProcessor.h
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

#include "SerializationConcepts.h"
#include "SerializationFlags.h"

namespace common_serialization
{

class SerializationProcessor
{
public:
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T* p, size_t n, S& output);
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T* p, size_t n, SerializationFlags flags, S& output);
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeDataCompat(const T* p, size_t n, SerializationFlags flags, uint32_t interfaceVersionCompat
        , std::unordered_map<void*, size_t>& mapOfPointers, S& output);

    template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T(&arr)[N], S& output);
    template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T(&arr)[N], SerializationFlags flags, S& output);
    template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeDataCompat(const T(&arr)[N], SerializationFlags flags, uint32_t interfaceVersionCompat
        , std::unordered_map<void*, size_t>& mapOfPointers, S& output);

    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T& value, S& output);
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeData(const T& value, SerializationFlags flags, S& output);
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeDataCompat(const T& value, SerializationFlags flags, uint32_t interfaceVersionCompat
        , std::unordered_map<void*, size_t>& mapOfPointers, S& output);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, size_t n, T* p);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, size_t n, SerializationFlags flags, T* p);

    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T(&arr)[N]);
    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, SerializationFlags flags, T(&arr)[N]);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, SerializationFlags flags, T& value);

    template<size_t OriginalTypeSize, typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T& value);

private:
    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status addPointerToMap(const T* p, std::unordered_map<void*, size_t>& mapOfPointers, S& output, bool& newPointer);

    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status convertStructToOldIfNeed(const T& value, SerializationFlags flags, uint32_t interfaceVersionCompat
        , std::unordered_map<void*, size_t>& mapOfPointers, S& output) noexcept;

    template<typename T, serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status convertStructToOld(const T& value, SerializationFlags flags, uint32_t thisVersionCompat
        , uint32_t interfaceVersionCompat, std::unordered_map<void*, size_t>& mapOfPointers, S& output) noexcept;
};

} // namespace common_serialization

#include "SerializationDataHelper.h"

namespace common_serialization
{

// common function for pointers of known size
template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T* p, size_t n, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serialization_concepts::SimpleAssignableType<T>)
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(serializeDataHelper(p[i], output));
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T* p, size_t n, SerializationFlags flags, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    // In mode with checked flags we may serialize a whole block pointed by p as one big chunk if one of the following is true.
    // 
    // Type on which p points is arithmetic or enum,
    // or if it is a struct that is simple assignable and aligned to one or a struct that simple assignable but
    // alignment shall be preserved, and for both this (struct) cases there must not be the flag that indicates
    // that some of arithmetic types may be not equal (cause we don't know contain this struct some of them or not)
    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(serializeDataHelper(p[i], flags, output));
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeDataCompat(const T* p, size_t n, SerializationFlags flags, uint32_t interfaceVersionCompat
    , std::unordered_map<void*, size_t>& mapOfPointers, S& output)
{
    static_assert(!std::is_reference_v<T>
        , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    // In mode with checked flags we may serialize a whole block pointed by p as one big chunk if one of the following is true.
    // 
    // Type on which p points is arithmetic or enum,
    // or if it is a struct that is simple assignable and aligned to one or a struct that simple assignable but
    // alignment shall be preserved, and for both this (struct) cases there must not be the flag that indicates
    // that some of arithmetic types may be not equal (cause we don't know contain this struct some of them or not)
    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || !serialization_concepts::IsISerializableBased<T>
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            if (flags.extendedPointerProcessing)
            {
                bool newPointer = false;
                RUN(addPointerToMap(&p[i], mapOfPointers, output, newPointer));

                if (!newPointer)
                    continue;
            }

            RUN(serializeDataHelper(p[i], flags, interfaceVersionCompat, mapOfPointers, output));
        }
    }

    return Status::kNoError;
}

// common function for arrays input
template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T(&arr)[N], S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serialization_concepts::SimpleAssignableType<T>)
    {
        constexpr typename S::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeThis");

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(arr)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (const auto& e : arr)
            RUN(serializeDataHelper(e, output));
    }

    return Status::kNoError;
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T(&arr)[N], SerializationFlags flags, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be serialized");

    // In mode with checked flags we may serialize a whole array as one big chunk if one of the following is true.
    // 
    // Type of units in arr is arithmetic or enum,
    // or if it is a struct that is simple assignable and aligned to one or a struct that simple assignable but
    // alignment shall be preserved, and for both this (struct) cases there must not be the flag that indicates
    // that some of arithmetic types may be not equal (cause we don't know contain this struct some of them or not)
    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        constexpr typename S::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeThis");

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(arr)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (const auto& e : arr)
            serializeDataHelper(e, flags, output);
    }

    return Status::kNoError;
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeDataCompat(const T(&arr)[N], SerializationFlags flags, uint32_t interfaceVersionCompat
    , std::unordered_map<void*, size_t>& mapOfPointers, S& output)
{
    static_assert(!std::is_reference_v<T>
        , "Arrays of references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be serialized");

    // In mode with checked flags we may serialize a whole array as one big chunk if one of the following is true.
    // 
    // Type of units in arr is arithmetic or enum,
    // or if it is a struct that is simple assignable and aligned to one or a struct that simple assignable but
    // alignment shall be preserved, and for both this (struct) cases there must not be the flag that indicates
    // that some of arithmetic types may be not equal (cause we don't know contain this struct some of them or not)
    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || !serialization_concepts::IsISerializableBased<T>
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        constexpr typename S::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeThis");

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(arr)), bytesSize));
    }
    else
    {
        for (const auto& e : arr)
        {
            if (flags.extendedPointerProcessing)
            {
                bool newPointer = false;
                RUN(addPointerToMap(&e, mapOfPointers, output, newPointer));

                if (!newPointer)
                    continue;
            }

            RUN(serializeDataHelper(e, flags, interfaceVersionCompat, mapOfPointers, output));
        }
    }

    return Status::kNoError;
}

// common function for scalar and simple assignable types input without flags provided
template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T& value, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
        RUN(output.pushBackArithmeticValue(value))
    else if constexpr (serialization_concepts::SimpleAssignableType<T>)
    {
        // for simple assignable types with no flags provided it is preferable to add a whole struct at a time
        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)), sizeof(T)));
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

// common function for scalar input with flags provided
template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeData(const T& value, SerializationFlags flags, S& output)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackArithmeticValue(value));
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

// common function for scalar input with flags provided
template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::serializeDataCompat(const T& value, SerializationFlags flags, uint32_t interfaceVersionCompat
    , std::unordered_map<void*, size_t>& mapOfPointers, S& output)
{
    static_assert(!std::is_reference_v<T>
        , "References are not allowed to be serialized");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            RUN(output.pushBackArithmeticValue(sizeof(T)));
        }

        RUN(output.pushBackArithmeticValue(value));
    }
    else 
    if constexpr (std::is_pointer_v<T> || std::is_member_pointer_v<T>)
    {
        if (flags.extendedPointerProcessing)
        {
            bool newPointer = false;
            RUN(addPointerToMap(&value, mapOfPointers, output, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }

        RUN(serializeDataCompatHelper(*value, flags, interfaceVersionCompat, mapOfPointers, output));
    }
    else
        static_assert(!(std::is_arithmetic_v<T> || std::is_enum_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>), "Type not supported");

    return Status::kNoError;
}

// common function for pointers of known size
template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, size_t n, T* p)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serialization_concepts::SimpleAssignableType<T>)
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(deserializeDataHelper(input, *(new (&p[i]) T)));
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, size_t n, SerializationFlags flags, T* p)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "References on pointers and pointers on pointers are not supported");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
        && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                for (size_t i = 0; i < n; ++i)
                    RUN(deserializeData<originalTypeSize>(input, p[i]));

                return Status::kNoError;
            }
        }

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(deserializeDataHelper(input, *(new (&p[i]) T)));
    }

    return Status::kNoError;
}

// common function for arrays input
template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, T(&arr)[N])
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T> || serialization_concepts::SimpleAssignableType<T>)
    {
        constexpr typename D::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array output of deserializeThis");

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(arr)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (auto& e : arr)
            RUN(deserializeDataHelper(input, e));
    }

    return Status::kNoError;
}

template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, SerializationFlags flags, T(&arr)[N])
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain arrays of references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "Arrays of references or pointers on functions are not allowed to be deserialized");

    if (
        std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
        && !flags.sizeOfArithmeticTypesMayBeNotEqual)
    {
        constexpr typename T::size_type bytesSize = sizeof(T) * N;
        static_assert(N == bytesSize / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array output of deserializeThis");

        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                for (size_t i = 0; i < N; ++i)
                    RUN(deserializeData<originalTypeSize>(input, arr[i]));

                return Status::kNoError;
            }
        }

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(arr)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (auto& e : arr)
            RUN(deserializeDataHelper(input, e));
    }

    return Status::kNoError;
}

// common function for scalar and simple assignable types output without flags provided
template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, T& value)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
        RUN(input.readArithmeticValue(value))
    else if constexpr (serialization_concepts::SimpleAssignableType<T>)
    {
        // for simple assignable types with no flags provided it is preferable to read a whole struct at a time
        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(&value)), sizeof(T)));
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

// common function for scalar output with flags provided
template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, SerializationFlags flags, T& value)
{
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> || std::is_member_pointer_v<T>)
        , "This type of struct cannot contain references or pointers");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be deserialized");

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if ((std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
            || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>)
            && flags.sizeOfArithmeticTypesMayBeNotEqual)
        {
            size_t originalTypeSize = 0;
            RUN(input.readArithmeticValue(originalTypeSize));
            if (originalTypeSize != sizeof(T))
            {
                RUN(deserializeData<originalTypeSize>(input, value));

                return 0;
            }
        }

        RUN(input.readArithmeticValue(value));
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

template<size_t OriginalTypeSize, typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status SerializationProcessor::deserializeData(D& input, T& value)
{
    static_assert(std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
        || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>
        , "Current deserializeThis function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    uint8_t arr[OriginalTypeSize] = { 0 };
    RUN(input.read(arr));
    value = *static_cast<T*>(static_cast<void*>(arr));

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::addPointerToMap(const T* p, std::unordered_map<void*, size_t>& mapOfPointers, S& output, bool& newPointer)
{
    if (!p)
    {
        newPointer = false;
        RUN(output.pushBackArithmeticValue(static_cast<size_t>(0)));
    }
    else
    {
        if (auto it = mapOfPointers.find(p); it == mapOfPointers.end())
        {
            newPointer = true;
            mapOfPointers[p] = output.size() + sizeof(size_t);
            RUN(output.pushBackArithmeticValue(static_cast<size_t>(-1)));
        }
        else
        {
            newPointer = false;
            RUN(output.pushBackArithmeticValue(it.second));
        }
    }
}

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status SerializationProcessor::convertStructToOldIfNeed(const T& value, SerializationFlags flags
    , uint32_t interfaceVersionCompat, std::unordered_map<void*, size_t>& mapOfPointers, S& output) noexcept
{
    uint32_t thisVersionCompat = value.getBestCompatInterfaceVersion(value.getVersionsHierarchy(), value.getVersionsHierarchySize(), interfaceVersionCompat);

    if (thisVersionCompat == value.getThisVersion())
    {
        RUN(output.pushBackArithmeticValue(value.getVersionsHierarchy()[0].nameHash));
        return Status::kNoError;
    }
    // Normaly, next condition shall never succeed
    else if (thisVersionCompat == value.kInterfaceVersionMax)
        return Status::kErrorNotSupportedSerializationInterfaceVersion;
    else
        return convertStructToOld(value, flags, thisVersionCompat, interfaceVersionCompat, mapOfPointers, output);
}

} // namespace common_serialization
