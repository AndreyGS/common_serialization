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
#include "Context.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

class DataProcessor
{
public:
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeData(const T* p, typename S::size_type n, context::Data<S, PM>& context);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM, typename S::size_type N>
    static constexpr Status serializeData(const T(&arr)[N], context::Data<S, PM>& context);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeData(const T& value, context::Data<S, PM>& context);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status serializeDataLegacy(const T& value, context::Data<S, PM>& context);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeData(context::Data<D, PM>& context, size_t n, T* p);
    template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeData(context::Data<D, PM>& context, T(&arr)[N]);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeData(context::Data<D, PM>& context, T& value);

    template<size_t OriginalTypeSize, typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status deserializeData(context::Data<D, PM>& context, T& value);

private:
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status addPointerToMap(const T* p, context::Data<S, PM>& context, bool& newPointer);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    static constexpr Status getPointerFromMap(context::Data<D, PM>& context, T* p, bool& newPointer);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status convertStructToOldIfNeed(const T& value, context::Data<S, PM>& context) noexcept;

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    static constexpr Status convertToOldStruct(const T& value, uint32_t thisVersionCompat, context::Data<S, PM>& context);
};

} // namespace processing

} // namespace csp

} // namespace common_serialization

#include "ProcessingDataHelper.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::serializeData(const T* p, typename S::size_type n, context::Data<S, PM>& context)
{
    static_assert(!std::is_reference_v<T>
                    , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
                    , "References and pointers on functions are not allowed to be serialized");

    S& output = context.getBinaryData();
    const context::Flags flags = context.getFlags();

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

            RUN(serializeData(p[i], context));
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM, typename S::size_type N>
constexpr Status DataProcessor::serializeData(const T(&arr)[N], context::Data<S, PM>& context)
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return serializeData(arr, N, context);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::serializeData(const T& value, context::Data<S, PM>& context)
{
    static_assert(!std::is_reference_v<T>
        , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    S& output = context.getBinaryData();
    const context::Flags flags = context.getFlags();

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
            RUN(addPointerToMap(value, context, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }

        RUN(serializeData(*value, context));
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
template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::Data<D, PM>& context, size_t n, T* p)
{
    static_assert(!std::is_reference_v<T>
        , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    D& input = context.getBinaryData();
    const context::Flags flags = context.getFlags();

    if (
           std::is_arithmetic_v<T> 
        || std::is_enum_v<T>
        || !flags.sizeOfArithmeticTypesMayBeNotEqual
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                size_t originalTypeSize = 0;
                RUN(input.readArithmeticValue(originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    RUN(deserializeData(context, n * originalTypeSize, static_cast<uint8_t*>(static_cast<void*>(p))));
                    return Status::kNoError;
                }
            }

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
            RUN(deserializeData(input, *(new (&p[i]) T)));
    }

    return Status::kNoError;
}

// common function for arrays
template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::Data<D, PM>& context, T(&arr)[N])
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return deserializeData(context, arr);
}

// common function for scalar and simple assignable types
template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeData(context::Data<D, PM>& context, T& value)
{
    static_assert(!std::is_reference_v<T>
        , "Pointers on references are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    D& input = context.getBinaryData();
    const context::Flags flags = context.getFlags();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                size_t originalTypeSize = 0;
                RUN(input.readArithmeticValue(originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    RUN(deserializeData<originalTypeSize>(context, value));

                    return Status::kNoError;
                }
            }

        RUN(input.readArithmeticValue(value));
    }
    else if constexpr (std::is_pointer_v<T> || std::is_member_pointer_v<T>)
    {
        if (flags.extendedPointersProcessing)
        {
            bool newPointer = false;
            RUN(getPointerFromMap(context, value, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }

        // think about replace this with some Allocator function
        value = new std::remove_pointer_t<T>;

        RUN(deserializeData(context, *value));
    }
    else if constexpr (serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>)
    {
        if (
            !flags.sizeOfArithmeticTypesMayBeNotEqual
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
            )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            RUN(input.read(static_cast<const uint8_t*>(static_cast<const void*>(&value), sizeof(T))));
        }
        else
            assert(false);
    }
    else
        static_assert(serialization_concepts::EmptyType<T>, "Type not supported");

    return Status::kNoError;
}

template<size_t OriginalTypeSize, typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::Data<D, PM>& context, T& value)
{
    static_assert(std::is_arithmetic_v<T> && !serialization_concepts::FixSizedArithmeticType<T>
        || std::is_enum_v<T> && !serialization_concepts::FixSizedArithmeticType<std::underlying_type_t<T>>
        , "Current deserializeData function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    uint8_t arr[OriginalTypeSize] = { 0 };
    RUN(context.getBinaryData().read(arr));
    value = *static_cast<T*>(static_cast<void*>(arr));

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::addPointerToMap(const T* p, context::Data<S, PM>& context, bool& newPointer)
{
    S& output = context.getBinaryData();

    if (!p)
    {
        newPointer = false;
        RUN(output.pushBackArithmeticValue(static_cast<size_t>(0)));
    }
    else
    {
        PM& pointersMap = *context.getPointersMap();

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
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::getPointerFromMap(context::Data<D, PM>& context, T* p, bool& newPointer)
{
    D& input = context.getBinary();

    size_t offset = 0;

    RUN(input.readArithmeticValue(static_cast<size_t>(offset)));

    if (!offset)
    {
        newPointer = false;
        p = nullptr;
    }
    else
    {
        PM& pointersMap = context.getPointersMap();

        if (auto it = pointersMap.find(offset); it == pointersMap.end())
            newPointer = true;
        else
        {
            newPointer = false;
            if (offset >= input.tell())
                return Status::KErrorInternal;

            p = pointersMap[offset];
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status DataProcessor::convertStructToOldIfNeed(const T& value, context::Data<S, PM>& context) noexcept
{
    if (const context::Flags flags = context.getFlags(); flags.interfaceVersionsNotMatch)
    {
        uint32_t thisVersionCompat = traits::getBestCompatInterfaceVersion(value.getVersionsHierarchy(), value.getVersionsHierarchySize(), context.getInterfaceVersion());

        if (thisVersionCompat == value.getThisVersion())
        {
            S& output = context.getBinaryData();
            RUN(output.pushBackArithmeticValue(value.getVersionsHierarchy()[0].nameHash));
            return Status::kNoError;
        }
        // Normaly, next condition shall never succeed
        else if (thisVersionCompat == traits::kInterfaceVersionMax)
            return Status::kErrorNotSupportedInterfaceVersion;
        else
            return convertToOldStruct(value, thisVersionCompat, context);
    }
    else
        return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
