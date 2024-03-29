/**
 * @file cslib/include/common_serialization/csp/processing/DataProcessor.h
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

#include "common_serialization/csp/context/Data.h"

namespace common_serialization::csp
{

template<typename T>
consteval interface_version_t getLatestInterfaceVersion();

}

namespace common_serialization::csp::processing
{

class DataProcessor
{
public:
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeData(const T* p, typename S::size_type n, context::SData<S, PM>& ctx);
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM, typename S::size_type N>
    static constexpr Status serializeData(const T(&arr)[N], context::SData<S, PM>& ctx);
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeData(const T& value, context::SData<S, PM>& ctx);

    template<ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeDataSizeT(const size_t& value, context::SData<S, PM>& ctx);

    /// @brief For now using for size_t serialization only
    /// @tparam T size_t
    /// @tparam S ISerializationCapableContainer
    /// @tparam PM ISerializationPointersMap
    /// @param targetTypeSize Size of type to which will be serialized
    /// @param value size_t value
    /// @param ctx CSP Full Data Context
    /// @return Status of operation
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeData(size_t targetTypeSize, const T& value, context::SData<S, PM>& ctx);

    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, typename D::size_type n, T* p);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM, typename D::size_type N>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, T(&arr)[N]);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeData(context::DData<D, PM>& ctx, T& value);

    template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeDataSizeT(context::DData<D, PM>& ctx, size_t& value);

    /// @brief Using for sizeOfIntegersMayBeNotEqual mode and size_t deserialization
    /// @tparam T Integral type or enum
    /// @tparam D IDeserializationCapableContainer
    /// @tparam PM IDeserializationPointersMap
    /// @param originalTypeSize Size of buffer that holds current value in serialized context
    /// @param ctx CSP Full Data Context
    /// @param value Output value
    /// @return Status of operation
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeData(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value);

protected:
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeDataSimplyAssignable(const T& value, context::SData<S, PM>& ctx);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeDataSimplyAssignable(context::DData<D, PM>& ctx, T& value);

    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer);

    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx);
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status convertToOldStruct(const T& value, uint32_t thisVersionCompat, context::SData<S, PM>& ctx);

    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status convertFromOldStruct(context::DData<D, PM>& ctx, uint32_t thisVersionCompat, T& value);

private:
    static constexpr size_t kMaxSizeOfIntegral = 8;   // maximum allowed size of integral type
};

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeData(const T* p, typename S::size_type n, context::SData<S, PM>& ctx)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
                    , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
                    , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);
    assert(n <= SIZE_MAX);

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    if (
           std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff() 
            && (!IsISerializableBased<T> || getLatestInterfaceVersion<T>() <= ctx.getInterfaceVersion())
            && (   AlwaysSimplyAssignableType<T> 
                || SimplyAssignableFixedSizeType<T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOneType<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignableType<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
    )
    {
        const typename S::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        S& output = ctx.getBinaryData();

            
        if constexpr ((std::is_arithmetic_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                CS_RUN(output.pushBackArithmeticValue(static_cast<uint8_t>(sizeof(T))));
            }

        CS_RUN(output.pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(p)), bytesSize));
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            if (ctx.checkRecursivePointers())
                (*ctx.getPointersMap())[&p[i]] = ctx.getBinaryData().size();

            CS_RUN(serializeData(p[i], ctx));
        }
    }

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM, typename S::size_type N>
constexpr Status DataProcessor::serializeData(const T(&arr)[N], context::SData<S, PM>& ctx)
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return serializeData(arr, N, ctx);
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeData(const T& value, context::SData<S, PM>& ctx)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    S& output = ctx.getBinaryData();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                CS_RUN(output.pushBackArithmeticValue(static_cast<uint8_t>(sizeof(T))));
            }

        CS_RUN(output.pushBackArithmeticValue(value))
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        if (!ctx.allowUnmanagedPointers())
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        if (ctx.checkRecursivePointers())
        {
            bool newPointer = false;
            CS_RUN(addPointerToMap(value, ctx, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }
        else
        {
            if (value == nullptr)
            {
                CS_RUN(output.pushBackArithmeticValue(uint8_t(0)));
                return Status::kNoError;
            }
            else
                CS_RUN(output.pushBackArithmeticValue(uint8_t(1)));
        }

        CS_RUN(serializeData(*value, ctx));
    }
    else if constexpr (AnySimplyAssignable<T>)
    {
        // We can be here only if there is no distinct serializeData function 
        // of type T. But if we must deserialize with simplyAssignableTagsOptimizationsAreTurnedOff
        // when interact with another CSP instance that does not support "simply assignable" tags
        // optimizations, we must to have it (distinct serializeData function).
        if (ctx.simplyAssignableTagsOptimizationsAreTurnedOff())
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        CS_RUN(serializeDataSimplyAssignable(value, ctx))
    }
    // we must implicitly use condition !EmptyType<T> otherwise we get an error which states that processing::serializeData not found
    else if constexpr (!EmptyType<T>)
        CS_RUN(processing::serializeData(value, ctx));

    return Status::kNoError;
}

template<ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeDataSizeT(const size_t& value, context::SData<S, PM>& ctx)
{
    return serializeData(ctx.bitness32() ? 4 : 8, value, ctx);
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeData(size_t targetTypeSize, const T& value, context::SData<S, PM>& ctx)
{
    static_assert((std::is_integral_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>
        , "Current serializeData function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if (kMaxSizeOfIntegral < targetTypeSize)
        return Status::kErrorTypeSizeIsTooBig;

    if (sizeof(T) == targetTypeSize)
        return ctx.getBinaryData().pushBackArithmeticValue(value);
    if (sizeof(T) < targetTypeSize)
    {
        constexpr uint8_t arr[kMaxSizeOfIntegral] = { 0 };

        S& output = ctx.getBinaryData();

        if (ctx.bigEndianFormat())
            CS_RUN(output.pushBackN(arr, targetTypeSize - sizeof(T)));

        CS_RUN(output.pushBackArithmeticValue(value));

        if (!ctx.bigEndianFormat())
            return output.pushBackN(arr, targetTypeSize - sizeof(T));
        else
            return Status::kNoError;
    }
    else
    {
        if (value >> (sizeof(T) - targetTypeSize) * 8)
            return Status::kErrorValueOverflow;

        if constexpr (helpers::isLittleEndianPlatform())
            return ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)), targetTypeSize);
        else
            return ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)) + sizeof(T) - targetTypeSize, targetTypeSize);
    }
}

// common function for pointers of known size
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, typename D::size_type n, T* p)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    if (
           std::is_arithmetic_v<T>
        || std::is_enum_v<T>
        || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
            && (!IsISerializableBased<T> || getLatestInterfaceVersion<T>() <= ctx.getInterfaceVersion())
            && (   AlwaysSimplyAssignableType<T> 
                || SimplyAssignableFixedSizeType<T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOneType<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignableType<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
    )
    {
        const typename D::size_type bytesSize = sizeof(T) * n;
        assert((n == bytesSize / sizeof(T)));

        D& input = ctx.getBinaryData();

        // In fact ctx.sizeOfIntegersMayBeNotEqual() can be true only if (std::is_arithmetic_v<T> || std::is_enum_v<T>) is true,
        // but if we do not wrap this in constexpr statement, all SimplyAssignable types would be forced to have deserializeData functions
        if constexpr ((std::is_arithmetic_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(input.readArithmeticValue(originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    for (typename D::size_type i = 0; i < n; ++i)
                        CS_RUN(deserializeData(originalTypeSize, ctx, p[i]));

                    return Status::kNoError;
                }
            }

        typename D::size_type readSize = 0;

        CS_RUN(input.read(static_cast<uint8_t*>(static_cast<void*>(p)), bytesSize, &readSize));

        if (readSize != bytesSize)
            return Status::kErrorOverflow;
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            T* pItem = new (&p[i]) T;

            if (ctx.checkRecursivePointers())
                (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = const_cast<from_ptr_to_const_to_ptr_t<T*>>(pItem);

            CS_RUN(deserializeData(ctx, *pItem));
        }
    }

    return Status::kNoError;
}

// common function for arrays
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM, typename D::size_type N>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, T(&arr)[N])
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serializeData");

    return deserializeData(ctx, N, arr);
}

// common function for scalar and simple assignable types
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(context::DData<D, PM>& ctx, T& value)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    D& input = ctx.getBinaryData();

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(input.readArithmeticValue(originalTypeSize));
                return deserializeData(originalTypeSize, ctx, value);
            }

        return input.readArithmeticValue(const_cast<std::remove_const_t<T>&>(value));
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        if (!ctx.allowUnmanagedPointers())
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        if (ctx.checkRecursivePointers())
        {
            bool newPointer = false;
            CS_RUN(getPointerFromMap(ctx, value, newPointer));

            if (!newPointer)
                return Status::kNoError;
        }
        else
        {
            uint8_t isValidPtr = 0;
            CS_RUN(input.readArithmeticValue(isValidPtr));
            if (!isValidPtr)
            {
                value = nullptr;
                return Status::kNoError;
            }
        }

        value = ctx.template allocateAndDefaultConstruct<std::remove_const_t<std::remove_pointer_t<T>>>();
        if (!value)
            return Status::kErrorNoMemory;

        if (ctx.checkRecursivePointers())
            (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = *const_cast<from_ptr_to_const_to_ptr_t<T>*>(&value);

        return deserializeData(ctx, *value);
    }
    // this will work for types that are not ISerializable
    else if constexpr (AnySimplyAssignable<T>)
    {
        // We can be here only if there is no distinct deserializeData function 
        // of type T. But if we must deserialize with simplyAssignableTagsOptimizationsAreTurnedOff
        // when interact with another CSP instance that does not support "simply assignable" tags
        // optimizations, we must to have it (distinct deserializeData function).
        if (ctx.simplyAssignableTagsOptimizationsAreTurnedOff())
            return Status::kErrorNotSupportedSerializationSettingsForStruct;

        return deserializeDataSimplyAssignable(ctx, value);
    }
    // we must implicitly use condition !EmptyType<T> otherwise we get an error which states that processing::deserializeData not found
    else if constexpr (!EmptyType<T>)
        return processing::deserializeData(ctx, value);
}

template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeDataSizeT(context::DData<D, PM>& ctx, size_t& value)
{
    return deserializeData(ctx.bitness32() ? 4 : 8, ctx, value);
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeData(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value)
{
    static_assert((std::is_integral_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>
        , "Current deserializeData function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if (kMaxSizeOfIntegral < originalTypeSize)
        return Status::kErrorTypeSizeIsTooBig;

    uint8_t arr[kMaxSizeOfIntegral] = { 0 };
    typename D::size_type readSize = 0;
    CS_RUN(ctx.getBinaryData().read(arr, originalTypeSize, &readSize));

    if (readSize != originalTypeSize)
        return Status::kErrorDataCorrupted;

    if (sizeof(T) == originalTypeSize)
        const_cast<std::remove_const_t<T>&>(value) = *static_cast<T*>(static_cast<void*>(arr));
    if (sizeof(T) < originalTypeSize)
    {
        if (ctx.bigEndianFormat())
        {
            for (size_t i = 0; i < originalTypeSize - sizeof(T); ++i)
                if (arr[i])
                    return Status::kErrorValueOverflow;

            const_cast<std::remove_const_t<T>&>(value) = *static_cast<T*>(static_cast<void*>(arr + originalTypeSize - sizeof(T)));
        }
        else
        {
            for (size_t i = sizeof(T); i < originalTypeSize; ++i)
                if (arr[i])
                    return Status::kErrorValueOverflow;

            const_cast<std::remove_const_t<T>&>(value) = *static_cast<T*>(static_cast<void*>(arr));
        }
    }
    else
        const_cast<std::remove_const_t<T>&>(value) = *static_cast<T*>(static_cast<void*>(arr));

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::serializeDataSimplyAssignable(const T& value, context::SData<S, PM>&ctx)
{
    if constexpr (NotSimplyAssignable<T>)
        return Status::kErrorInvalidType;
    else
    {
        if constexpr (IsISerializableBased<T>)
            if (T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::kNoError;

        if (   AlwaysSimplyAssignableType<T>
            || SimplyAssignableFixedSizeType<T> && !ctx.alignmentMayBeNotEqual()
            || SimplyAssignableAlignedToOneType<T> && !ctx.sizeOfIntegersMayBeNotEqual()
            || SimplyAssignableType<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
        )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(ctx.getBinaryData().pushBackN(static_cast<const uint8_t*>(static_cast<const void*>(&value)), sizeof(T)));

            return Status::kNoFurtherProcessingRequired;
        }
        else
            return Status::kErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::deserializeDataSimplyAssignable(context::DData<D, PM>& ctx, T& value)
{
    if constexpr (NotSimplyAssignable<T>)
        return Status::kErrorInvalidType;
    else
    {
        if constexpr (IsISerializableBased<T>)
            if (T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::kNoError;

        if (   AlwaysSimplyAssignableType<T>
            || SimplyAssignableFixedSizeType<T> && !ctx.alignmentMayBeNotEqual()
            || SimplyAssignableAlignedToOneType<T> && !ctx.sizeOfIntegersMayBeNotEqual()
            || SimplyAssignableType<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
        )
        {
            typename D::size_type readSize = 0;
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(ctx.getBinaryData().read(static_cast<uint8_t*>(static_cast<void*>(&value)), sizeof(T), &readSize));

            if (readSize != sizeof(T))
                return Status::kErrorOverflow;

            return Status::kNoFurtherProcessingRequired;
        }
        else
            return Status::kErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer)
{
    S& output = ctx.getBinaryData();

    if (!p)
    {
        newPointer = false;
        CS_RUN(serializeDataSizeT(0, ctx));
    }
    else
    {
        PM& pointersMap = *ctx.getPointersMap();

        if (auto it = pointersMap.find(p); it == pointersMap.end())
        {
            newPointer = true;
            CS_RUN(serializeDataSizeT(1, ctx));
            pointersMap[p] = output.size();
        }
        else
        {
            newPointer = false;
            CS_RUN(serializeDataSizeT(it->second, ctx));
        }
    }

    return Status::kNoError;
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer)
{
    D& input = ctx.getBinaryData();

    size_t offset = 0;

    CS_RUN(deserializeDataSizeT(ctx, offset));

    if (!offset)
    {
        newPointer = false;
        p = nullptr;
    }
    else if (offset == 1)
        newPointer = true;
    else
    {
        PM& pointersMap = *ctx.getPointersMap();

        newPointer = false;
        if (offset >= input.tell())
            return Status::kErrorInternal;

        p = reinterpret_cast<T>(pointersMap[offset]);
    }

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::convertToOldStructIfNeed(const T& value, context::SData<S, PM>& ctx)
{
    if constexpr (IsNotISerializableBased<T>)
        return Status::kErrorInvalidType;
    else
    {
        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<T>(ctx.getInterfaceVersion());

        S& output = ctx.getBinaryData();

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::kNoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInternal;
        else
            return convertToOldStruct(value, targetVersion, ctx);
    }
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::convertFromOldStructIfNeed(context::DData<D, PM>& ctx, T& value)
{
    if constexpr (IsNotISerializableBased<T>)
        return Status::kErrorInvalidType;
    else
    {
        D& input = ctx.getBinaryData();

        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::kNoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInternal;
        else
            return convertFromOldStruct(ctx, targetVersion, value);
    }
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status DataProcessor::convertToOldStruct(const T& value, uint32_t targetVersion, context::SData<S, PM>& ctx)
{
    return Status::kErrorNoSuchHandler;
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status DataProcessor::convertFromOldStruct(context::DData<D, PM>& ctx, uint32_t targetVersion, T& value)
{
    return Status::kErrorNoSuchHandler;
}

} // namespace common_serialization::csp::processing
