/**
 * @file ProcessingDataProcessor.h
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
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status serializeData(const T* p, typename S::size_type n, context::SData<S, PM>& ctx);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM, typename S::size_type N>
    static constexpr Status serializeData(const T(&arr)[N], context::SData<S, PM>& ctx);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status serializeData(const T& value, context::SData<S, PM>& ctx);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, typename D::size_type n, T* p);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM, typename D::size_type N>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, T(&arr)[N]);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status deserializeData(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value);

protected:
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status serializeDataSimpleAssignable(const T& value, context::SData<S, PM>& ctx);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
        requires serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>
    static constexpr Status serializeDataSimpleAssignable(const T& value, context::SData<S, PM>& ctx);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status deserializeDataSimpleAssignable(context::DData<D, PM>& ctx, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
        requires serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>
    static constexpr Status deserializeDataSimpleAssignable(context::DData<D, PM>&ctx, T & value);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx);
    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
        requires serialization_concepts::IsNotISerializableBased<T>
    static constexpr Status convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx);

    template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    static constexpr Status convertToOldStruct(const T& value, uint32_t thisVersionCompat, context::SData<S, PM>& ctx);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value);
    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
        requires serialization_concepts::IsNotISerializableBased<T>
    static constexpr Status convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value);

    template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    static constexpr Status convertFromOldStruct(context::DData<D, PM>& ctx, uint32_t thisVersionCompat, T& value);
};

} // namespace processing

} // namespace csp

} // namespace common_serialization

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeData(const T* p, typename S::size_type n, context::SData<S, PM>& ctx)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
                    , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
                    , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);

    const context::DataFlags flags = ctx.getFlags();

    if (
           std::is_arithmetic_v<T> 
        || std::is_enum_v<T>
        || !flags.sizeOfArithmeticTypesMayBeNotEqual && (!serialization_concepts::IsISerializableBased<T> || !ctx.isInterfaceVersionsNotMatch())
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        S& output = ctx.getBinaryData();

        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                RUN(output.pushBackArithmeticValue(static_cast<uint8_t>(sizeof(T))));
            }

        RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
        {
            if (flags.checkRecursivePointers)
                (*ctx.getPointersMap())[&p[i]] = ctx.getBinaryData().size();

            RUN(serializeData(p[i], ctx));
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM, typename S::size_type N>
constexpr Status DataProcessor::serializeData(const T(&arr)[N], context::SData<S, PM>& ctx)
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return serializeData(arr, N, ctx);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeData(const T& value, context::SData<S, PM>& ctx)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    S& output = ctx.getBinaryData();
    const context::DataFlags flags = ctx.getFlags();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                RUN(output.pushBackArithmeticValue(static_cast<uint8_t>(sizeof(T))));
            }

        RUN(output.pushBackArithmeticValue(value))
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        if (!flags.allowUnmanagedPointers)
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        if (flags.checkRecursivePointers)
        {
            bool newPointer = false;
            RUN(addPointerToMap(value, ctx, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }
        else
        {
            if (value == nullptr)
            {
                RUN(output.pushBackArithmeticValue(uint8_t(0)));
                return Status::kNoError;
            }
            else
                RUN(output.pushBackArithmeticValue(uint8_t(1)));
        }

        RUN(serializeData(*value, ctx));
    }
    else if constexpr (serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>)
        RUN(serializeDataSimpleAssignable(value, ctx))
    else if constexpr (!serialization_concepts::EmptyType<T>)
        RUN(processing::serializeData(value, ctx));

    return Status::kNoError;
}

// common function for pointers of known size
template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, typename D::size_type n, T* p)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);

    const context::DataFlags flags = ctx.getFlags();

    if (
           std::is_arithmetic_v<T> 
        || std::is_enum_v<T>
        || !flags.sizeOfArithmeticTypesMayBeNotEqual && (!serialization_concepts::IsISerializableBased<T> || !ctx.isInterfaceVersionsNotMatch())
            && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        D& input = ctx.getBinaryData();

        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                uint8_t originalTypeSize = 0;
                RUN(input.readArithmeticValue(originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    RUN(deserializeData(ctx, n * originalTypeSize, static_cast<uint8_t*>(static_cast<void*>(p))));
                    return Status::kNoError;
                }
            }

        typename D::size_type readSize = 0;

        RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize, &readSize));

        if (readSize != bytesSize)
            return Status::kErrorOverflow;

    }
    else if constexpr (!serialization_concepts::EmptyType<T>)
    {
        for (size_t i = 0; i < n; ++i)
        {
            T* pItem = new (&p[i]) T;

            if (flags.checkRecursivePointers)
                (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = const_cast<from_ptr_to_const_to_ptr_t<T*>>(pItem);

            RUN(deserializeData(ctx, *pItem));
        }
    }

    return Status::kNoError;
}

// common function for arrays
template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM, typename D::size_type N>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, T(&arr)[N])
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return deserializeData(ctx, N, arr);
}

// common function for scalar and simple assignable types
template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, T& value)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    D& input = ctx.getBinaryData();
    const context::DataFlags flags = ctx.getFlags();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>)
            if (flags.sizeOfArithmeticTypesMayBeNotEqual)
            {
                uint8_t originalTypeSize = 0;
                RUN(input.readArithmeticValue(originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    RUN(deserializeData(originalTypeSize, ctx, value));

                    return Status::kNoError;
                }
            }

        RUN(input.readArithmeticValue(const_cast<std::remove_const_t<T>&>(value)));
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        if (!flags.allowUnmanagedPointers)
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        if (flags.checkRecursivePointers)
        {
            bool newPointer = false;
            RUN(getPointerFromMap(ctx, value, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }
        else
        {
            uint8_t isValidPtr = 0;
            RUN(input.readArithmeticValue(isValidPtr));
            if (!isValidPtr)
            {
                value = nullptr;
                return Status::kNoError;
            }
        }

        // think about replace this with some Allocator function
        value = ctx.allocateAndDefaultConstruct<std::remove_const_t<std::remove_pointer_t<T>>>();
        if (!value)
            return Status::kErrorNoMemory;
        //else        
            //(*ctx.getAddedPointers()).pushBack(value);

        if (flags.checkRecursivePointers)
            (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = *const_cast<from_ptr_to_const_to_ptr_t<T>*>(&value);

        RUN(deserializeData(ctx, *value));
    }
    // this is for types that are not ISerializable
    else if constexpr (serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>)
        RUN(deserializeDataSimpleAssignable(ctx, value))
    else if constexpr (!serialization_concepts::EmptyType<T>)
        RUN(processing::deserializeData(ctx, value));

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value)
{
    static_assert((std::is_arithmetic_v<T> || std::is_enum_v<T>) &&
        !serialization_concepts::FixSizedArithmeticType<T> && !serialization_concepts::FixSizedEnumType<T>
        , "Current deserializeData function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    uint8_t arr[64] = { 0 };
    typename D::size_type readSize = 0;
    RUN(ctx.getBinaryData().read(arr, originalTypeSize, &readSize));

    if (readSize != originalTypeSize)
        return Status::kErrorOverflow;

    const_cast<std::remove_const_t<T>&>(value) = *static_cast<T*>(static_cast<void*>(arr));

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
static constexpr Status DataProcessor::serializeDataSimpleAssignable(const T& value, context::SData<S, PM>& ctx)
{
    return Status::kErrorInvalidType;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    requires serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>
static constexpr Status DataProcessor::serializeDataSimpleAssignable(const T& value, context::SData<S, PM>&ctx)
{
    if (
        context::DataFlags flags = ctx.getFlags();
        !flags.sizeOfArithmeticTypesMayBeNotEqual
        && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        // for simple assignable types it is preferable to get a whole struct at a time
        RUN(ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)), sizeof(T)));

        return Status::kNoFurtherProcessingRequired;
    }
    else
        return Status::kErrorNotSupportedSerializationSettingsForStruct;       
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeDataSimpleAssignable(context::DData<D, PM>& ctx, T& value)
{
    return Status::kErrorInvalidType;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    requires serialization_concepts::SimpleAssignableType<T> || serialization_concepts::SimpleAssignableAlignedToOneType<T>
constexpr Status DataProcessor::deserializeDataSimpleAssignable(context::DData<D, PM>& ctx, T& value)
{
    if (
        context::DataFlags flags = ctx.getFlags();
        !flags.sizeOfArithmeticTypesMayBeNotEqual
        && (serialization_concepts::SimpleAssignableAlignedToOneType<T> || serialization_concepts::SimpleAssignableType<T> && !flags.alignmentMayBeNotEqual)
    )
    {
        typename D::size_type readSize = 0;
        // for simple assignable types it is preferable to get a whole struct at a time
        RUN(ctx.getBinaryData().read(static_cast<uint8_t*>(static_cast<void*>(&value)), sizeof(T), &readSize));

        if (readSize != sizeof(T))
            return Status::kErrorOverflow;

        return Status::kNoFurtherProcessingRequired;
    }
    else
        return Status::kErrorNotSupportedSerializationSettingsForStruct;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status DataProcessor::addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer)
{
    S& output = ctx.getBinaryData();

    if (!p)
    {
        newPointer = false;
        RUN(output.pushBackArithmeticValue(static_cast<uint64_t>(0)));
    }
    else
    {
        PM& pointersMap = *ctx.getPointersMap();

        if (auto it = pointersMap.find(p); it == pointersMap.end())
        {
            newPointer = true;
            RUN(output.pushBackArithmeticValue(static_cast<uint64_t>(1)));
            pointersMap[p] = output.size();
        }
        else
        {
            newPointer = false;
            RUN(output.pushBackArithmeticValue(it->second));
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status DataProcessor::getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer)
{
    D& input = ctx.getBinaryData();

    uint64_t offset = 0;

    RUN(input.readArithmeticValue(offset));

    if (!offset)
    {
        newPointer = false;
        p = nullptr;
    }
    else
    {
        PM& pointersMap = *ctx.getPointersMap();

        if (auto it = pointersMap.find(offset); it == pointersMap.end())
            newPointer = true;
        else
        {
            newPointer = false;
            if (offset >= input.tell())
                return Status::kErrorInternal;

            p = reinterpret_cast<T>(pointersMap[offset]);
        }
    }

    return Status::kNoError;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status DataProcessor::convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx)
{
    uint32_t targetVersion = traits::getBestCompatInterfaceVersion(value.getVersionsHierarchy(), value.getVersionsHierarchySize(), ctx.getInterfaceVersion());

    S& output = ctx.getBinaryData();

    if (targetVersion == value.getThisVersion())
        return Status::kNoError;
    // Normaly, next condition shall never succeed
    else if (targetVersion == traits::kInterfaceVersionMax)
        return Status::kErrorInternal;
    else
        return convertToOldStruct(value, targetVersion, ctx);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    requires serialization_concepts::IsNotISerializableBased<T>
static constexpr Status DataProcessor::convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx)
{
    return Status::kErrorInvalidType;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
static constexpr Status DataProcessor::convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value)
{
    D& input = ctx.getBinaryData();

    uint32_t targetVersion = traits::getBestCompatInterfaceVersion(value.getVersionsHierarchy(), value.getVersionsHierarchySize(), ctx.getInterfaceVersion());

    if (targetVersion == value.getThisVersion())
        return Status::kNoError;
    // Normaly, next condition shall never succeed
    else if (targetVersion == traits::kInterfaceVersionMax)
        return Status::kErrorDataCorrupted;
    else
        return convertFromOldStruct(ctx, targetVersion, value);
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    requires serialization_concepts::IsNotISerializableBased<T>
constexpr Status DataProcessor::convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value)
{
    return Status::kErrorInvalidType;
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status DataProcessor::convertToOldStruct(const T& value, uint32_t targetVersion, context::SData<S, PM>& ctx)
{
    return Status::kErrorNoSuchHandler;
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
static constexpr Status DataProcessor::convertFromOldStruct(context::DData<D, PM>& ctx, uint32_t targetVersion, T& value)
{
    return Status::kErrorNoSuchHandler;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
