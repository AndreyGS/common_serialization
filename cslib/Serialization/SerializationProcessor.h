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
#include "CspContext.h"

namespace common_serialization
{

class SerializationProcessor
{
public:
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeData(const T* p, typename S::size_type n, CspContextSerializeData<S, PM>& context);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM, typename S::size_type N>
    static constexpr Status serializeData(const T(&arr)[N], CspContextSerializeData<S, PM>& context);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeData(const T& value, CspContextSerializeData<S, PM>& context);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeDataLegacy(const T& value, CspContextSerializeData<S, PM>& context);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, size_t n, T* p);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, size_t n, CspFlags flags, T* p);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
        , uint32_t interfaceVersionCompat, PM& pointersMap, T* p);

    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T(&arr)[N]);
    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, CspFlags flags, T(&arr)[N]);
    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
        , uint32_t interfaceVersionCompat, PM& pointersMap, T(&arr)[N]);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, CspFlags flags, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
        , uint32_t interfaceVersionCompat, PM& pointersMap, T& value);

    template<size_t OriginalTypeSize, typename T, serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeData(D& input, T& value);

private:
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status addPointerToMap(const T* p, CspContextSerializeData<S, PM>& context, bool& newPointer);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status convertStructToOldIfNeed(const T& value, CspContextSerializeData<S, PM>& context) noexcept;

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status convertToOldStruct(const T& value, uint32_t thisVersionCompat, CspContextSerializeData<S, PM>& context) noexcept;
};

} // namespace common_serialization

#include "SerializationDataHelper.h"

namespace common_serialization
{

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status SerializationProcessor::serializeData(const T* p, typename S::size_type n, CspContextSerializeData<S, PM>& context)
{
    static_assert(!std::is_reference_v<T>
                    , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
                    , "References and pointers on functions are not allowed to be serialized");

    S& output = context.getOutput();
    CspContextDataExtension<PM>& extension = context.getExtension();
    const CspFlags& flags = extension.getFlags();

    if (
           std::is_arithmetic_v<T> 
        || std::is_enum_v<T>
        || !flags.sizeOfArithmeticTypesMayBeNotEqual
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                RUN(output.pushBackArithmeticValue(sizeof(T)));
            }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
        {
            if (flags.extendedPointersProcessing)
            {
                bool newPointer = false;
                RUN(addPointerToMap(&p[i], context, newPointer));

                if (!newPointer)
                    continue;
            }

            RUN(serializeDataHelper(p[i], context));
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM, typename S::size_type N>
constexpr Status SerializationProcessor::serializeData(const T(&arr)[N], CspContextSerializeData<S, PM>& context)
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeThis");

    return serializeData(arr, N, context);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status SerializationProcessor::serializeData(const T& value, CspContextSerializeData<S, PM>& context)
{
    static_assert(!std::is_reference_v<T>
        , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    S& output = context.getOutput();
    CspContextDataExtension<PM>& extension = context.getExtension();
    const CspFlags& flags = extension.getFlags();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                RUN(output.pushBackArithmeticValue(sizeof(T)));
            }

        RUN(output.pushBackArithmeticValue(value))
    }
    else if constexpr (std::is_pointer_v<T> || std::is_member_pointer_v<T>)
    {
        if (flags.extendedPointersProcessing)
        {
            bool newPointer = false;
            RUN(addPointerToMap(&value, context, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }

        RUN(serializeDataCompatHelper(*value, context));
    }
    else if constexpr (serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>)
    {
        if (
            !flags.sizeOfArithmeticTypesMayBeNotEqual
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
        )
        {
            // for simple assignable types it is preferable to add a whole struct at a time
            RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)), sizeof(T)));
        }
        else
            assert(false);
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

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
constexpr Status SerializationProcessor::deserializeData(D& input, size_t n, CspFlags flags, T* p)
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

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
static constexpr Status SerializationProcessor::deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, T* p)
{
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
constexpr Status SerializationProcessor::deserializeData(D& input, CspFlags flags, T(&arr)[N])
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

template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
static constexpr Status SerializationProcessor::deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, T(&arr)[N])
{
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
constexpr Status SerializationProcessor::deserializeData(D& input, CspFlags flags, T& value)
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

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
static constexpr Status SerializationProcessor::deserializeDataCompat(D& input, CspFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, T& value)
{
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

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status SerializationProcessor::addPointerToMap(const T* p, CspContextSerializeData<S, PM>& context, bool& newPointer)
{
    S& output = context.getOutput();

    if (!p)
    {
        newPointer = false;
        RUN(output.pushBackArithmeticValue(static_cast<size_t>(0)));
    }
    else
    {
        PM& pointersMap = *context.getExtension().getPointersMap();

        if (auto it = pointersMap.find(p); it == pointersMap.end())
        {
            newPointer = true;
            pointersMap[p] = output.size() + sizeof(size_t);
            RUN(output.pushBackArithmeticValue(static_cast<size_t>(-1)));
        }
        else
        {
            newPointer = false;
            RUN(output.pushBackArithmeticValue(it->second));
        }

        return Status::kNoError;
    }
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status SerializationProcessor::convertStructToOldIfNeed(const T& value, CspContextSerializeData<S, PM>& context) noexcept
{
    CspContextDataExtension<PM>& extension = context.getExtension();

    if (const CspFlags& flags = extension.getFlags(); flags.interfaceVersionsNotMatch)
    {
        uint32_t thisVersionCompat = value.getBestCompatInterfaceVersion(value.getVersionsHierarchy(), value.getVersionsHierarchySize(), extension.getInterfaceVersion());

        if (thisVersionCompat == value.getThisVersion())
        {
            S& output = context.getOutput();
            RUN(output.pushBackArithmeticValue(value.getVersionsHierarchy()[0].nameHash));
            return Status::kNoError;
        }
        // Normaly, next condition shall never succeed
        else if (thisVersionCompat == value.kInterfaceVersionMax)
            return Status::kErrorNotSupportedSerializationInterfaceVersion;
        else
            return convertToOldStruct(value, thisVersionCompat, context);
    }
    else
        return Status::kNoError;
}

} // namespace common_serialization
