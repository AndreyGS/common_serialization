/**
 * @file cslib/include/common_serialization/csp/processing/DataBodyProcessor.h
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
#include "common_serialization/csp/processing/Rw.h"

namespace common_serialization::csp
{

template<typename T>
consteval interface_version_t getLatestInterfaceVersion();

}

namespace common_serialization::csp::processing::data
{

class BodyProcessor
{
public:
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serialize(const T* p, typename S::size_type n, context::SData<S, PM>& ctx);
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM, typename S::size_type N>
    static constexpr Status serialize(const T(&arr)[N], context::SData<S, PM>& ctx);
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serialize(const T& value, context::SData<S, PM>& ctx);

    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status serializeSizeT(T value, context::SData<S, PM>& ctx);

    /// @brief For now using for size_t serialization only
    /// @tparam T size_t
    /// @tparam S ISerializationCapableContainer
    /// @tparam PM ISerializationPointersMap
    /// @param targetTypeSize Size of type to which will be serialized
    /// @param value size_t value
    /// @param ctx CSP Full Data Context
    /// @return Status of operation
    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status serializeToAnotherSize(size_t targetTypeSize, T value, context::SData<S, PM>& ctx);

    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserialize(context::DData<D, PM>& ctx, typename D::size_type n, T* p);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM, typename D::size_type N>
    static constexpr Status deserialize(context::DData<D, PM>& ctx, T(&arr)[N]);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserialize(context::DData<D, PM>& ctx, T& value);

    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status deserializeSizeT(context::DData<D, PM>& ctx, T& value);

    /// @brief Using for sizeOfIntegersMayBeNotEqual mode and size_t deserialization
    /// @tparam T Integral type or enum
    /// @tparam D IDeserializationCapableContainer
    /// @tparam PM IDeserializationPointersMap
    /// @param originalTypeSize Size of buffer that holds current value in serialized context
    /// @param ctx CSP Full Data Context
    /// @param value Output value
    /// @return Status of operation
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status deserializeFromAnotherSize(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value);

protected:
    template<size_t targetTypeSize, typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static constexpr Status serializeToAnotherSizeInternal(T value, context::SData<S, PM>& ctx);
    template<size_t originalTypeSize,typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static constexpr Status deserializeFromAnotherSizeInternal(context::DData<D, PM>& ctx, T& value);

    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status serializeSimplyAssignable(const T& value, context::SData<S, PM>& ctx);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status deserializeSimplyAssignable(context::DData<D, PM>& ctx, T& value);

    template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer);
    template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    static CS_ALWAYS_INLINE constexpr Status getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer);

private:
    static constexpr size_t kMaxSizeOfIntegral = 8;   // maximum allowed size of integral type
};

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status BodyProcessor::serialize(const T* p, typename S::size_type n, context::SData<S, PM>& ctx)
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
           (!ctx.endiannessDifference() || EndiannessTolerant<T>)
        && (   std::is_arithmetic_v<T>
            || std::is_enum_v<T>
            || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
                && (!IsISerializableBased<T> || getLatestInterfaceVersion<T>() <= ctx.getInterfaceVersion())
                && (   AlwaysSimplyAssignable<T>
                    || SimplyAssignableFixedSize<T> && !ctx.alignmentMayBeNotEqual()
                    || SimplyAssignableAlignedToOne<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                    || SimplyAssignable<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
            )
    )
    {   
        if constexpr ((std::is_arithmetic_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
                CS_RUN(writePrimitive(static_cast<uint8_t>(sizeof(T)), ctx));

        return writeRawData(p, n, ctx);
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            if (ctx.checkRecursivePointers())
                (*ctx.getPointersMap())[&p[i]] = ctx.getBinaryData().size();

            CS_RUN(serialize(p[i], ctx));
        }

        return Status::kNoError;
    }
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM, typename S::size_type N>
constexpr Status BodyProcessor::serialize(const T(&arr)[N], context::SData<S, PM>& ctx)
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serialize");

    return serialize(arr, N, ctx);
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status BodyProcessor::serialize(const T& value, context::SData<S, PM>& ctx)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
                CS_RUN(writePrimitive(static_cast<uint8_t>(sizeof(T)), ctx));

        return writePrimitive(value, ctx);
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
                CS_RUN(writePrimitive(uint8_t(0), ctx));
                return Status::kNoError;
            }
            else
                CS_RUN(writePrimitive(uint8_t(1), ctx));
        }

        return serialize(*value, ctx);
    }
    else if constexpr (AnySimplyAssignable<T>)
        return serializeSimplyAssignable(value, ctx);
    // we must implicitly use condition !EmptyType<T> otherwise we get an error which states that processing::serialize not found
    else if constexpr (!EmptyType<T>)
        return templates::serialize(value, ctx);
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::serializeSizeT(T value, context::SData<S, PM>& ctx)
{
    return serializeToAnotherSize(!ctx.bitness32() ? 8 : 4, value, ctx);
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::serializeToAnotherSize(size_t targetTypeSize, T value, context::SData<S, PM>& ctx)
{
    switch (targetTypeSize)
    {
    case 8: return serializeToAnotherSizeInternal<8>(value, ctx);
    case 4: return serializeToAnotherSizeInternal<4>(value, ctx);
    case 2: return serializeToAnotherSizeInternal<2>(value, ctx);
    case 1: return serializeToAnotherSizeInternal<1>(value, ctx);
    default:
        assert(false);
        return Status::kErrorInternal;
    }
}

template<size_t targetTypeSize, typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status BodyProcessor::serializeToAnotherSizeInternal(T value, context::SData<S, PM>& ctx)
{
    static_assert((std::is_integral_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>
        , "Current serialize function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if constexpr (kMaxSizeOfIntegral < targetTypeSize)
        return Status::kErrorTypeSizeIsTooBig;

    if constexpr (sizeof(T) == targetTypeSize)
        return writePrimitive(value, ctx);

    helpers::fixed_width_integer_t<targetTypeSize, IsSigned<T>> targetValue = 0;

    if constexpr (sizeof(T) < targetTypeSize)
        helpers::castToBiggerType(value, targetValue);
    else
        CS_RUN(helpers::castToSmallerType<targetTypeSize>(value, targetValue));

    return writePrimitive(targetValue, ctx);
}

// common function for pointers of known size
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status BodyProcessor::deserialize(context::DData<D, PM>& ctx, typename D::size_type n, T* p)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    if (
           (!ctx.endiannessDifference() || EndiannessTolerant<T>)
        && (   std::is_arithmetic_v<T>
            || std::is_enum_v<T>
            || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
                && (!IsISerializableBased<T> || getLatestInterfaceVersion<T>() <= ctx.getInterfaceVersion())
                && (   AlwaysSimplyAssignable<T>
                    || SimplyAssignableFixedSize<T> && !ctx.alignmentMayBeNotEqual()
                    || SimplyAssignableAlignedToOne<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                    || SimplyAssignable<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
            )
    )
    {
        // In fact ctx.sizeOfIntegersMayBeNotEqual() can be true only if (std::is_arithmetic_v<T> || std::is_enum_v<T>) is true,
        // but if we do not wrap this in constexpr statement, all SimplyAssignable types would be forced to have deserialize functions
        if constexpr ((std::is_arithmetic_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(readPrimitive(ctx, originalTypeSize));
                if (originalTypeSize != sizeof(T))
                {
                    for (typename D::size_type i = 0; i < n; ++i)
                        CS_RUN(deserializeFromAnotherSize(originalTypeSize, ctx, p[i]));

                    return Status::kNoError;
                }
            }

        typename D::size_type readSize = 0;

        return readRawData(ctx, n, p);
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            T* pItem = new (&p[i]) T;

            if (ctx.checkRecursivePointers())
                (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = const_cast<from_ptr_to_const_to_ptr_t<T*>>(pItem);

            CS_RUN(deserialize(ctx, *pItem));
        }

        return Status::kNoError;
    }
}

// common function for arrays
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM, typename D::size_type N>
constexpr Status BodyProcessor::deserialize(context::DData<D, PM>& ctx, T(&arr)[N])
{
    static_assert(N == sizeof(T) * N / sizeof(T), "Oveflow occured in (sizeof(T) * N) in instantiation of array input of serialize");

    return deserialize(ctx, N, arr);
}

// common function for scalar and simple assignable types
template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status BodyProcessor::deserialize(context::DData<D, PM>& ctx, T& value)
{
    static_assert(!std::is_reference_v<T> || !std::is_member_pointer_v<T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<T> || std::is_member_function_pointer_v<T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (EmptyType<T>)
        return Status::kNoError;

    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
    {
        if constexpr (!FixSizedArithmeticType<T> && !FixSizedEnumType<T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(readPrimitive(ctx, originalTypeSize));
                return deserializeFromAnotherSize(originalTypeSize, ctx, value);
            }

        return readPrimitive(ctx, value);
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
            CS_RUN(readPrimitive(ctx, isValidPtr));
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

        return deserialize(ctx, *value);
    }
    // this will work for types that are not ISerializable
    else if constexpr (AnySimplyAssignable<T>)
        return deserializeSimplyAssignable(ctx, value);
    // we must implicitly use condition !EmptyType<T> otherwise we get an error which states that processing::deserialize not found
    else if constexpr (!EmptyType<T>)
        return templates::deserialize(ctx, value);
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::deserializeSizeT(context::DData<D, PM>& ctx, T& value)
{
    return deserializeFromAnotherSize(!ctx.bitness32() ? 8 : 4, ctx, value);
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::deserializeFromAnotherSize(size_t originalTypeSize, context::DData<D, PM>& ctx, T& value)
{
    switch (originalTypeSize)
    {
    case 8: return deserializeFromAnotherSizeInternal<8>(ctx, value);
    case 4: return deserializeFromAnotherSizeInternal<4>(ctx, value);
    case 2: return deserializeFromAnotherSizeInternal<2>(ctx, value);
    case 1: return deserializeFromAnotherSizeInternal<1>(ctx, value);
    default:
        assert(false);
        return Status::kErrorInternal;
    }
}

template<size_t originalTypeSize, typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status BodyProcessor::deserializeFromAnotherSizeInternal(context::DData<D, PM>& ctx, T& value)
{
    static_assert((std::is_integral_v<T> || std::is_enum_v<T>) && !FixSizedArithmeticType<T> && !FixSizedEnumType<T>
        , "Current deserialize function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if constexpr (kMaxSizeOfIntegral < originalTypeSize)
        return Status::kErrorTypeSizeIsTooBig;

    if constexpr (sizeof(T) == originalTypeSize)
        return readPrimitive(ctx, value);

    Status status = Status::kNoError;

    helpers::fixed_width_integer_t<originalTypeSize, IsSigned<T>> originalValue = 0;
    status = readPrimitive(ctx, originalValue);

    if (!statusSuccess(status))
    {
        if (status == Status::kErrorOverflow)
            return Status::kErrorDataCorrupted;
        return status;
    }

    using sameSizedInteger = helpers::fixed_width_integer_t<sizeof(T), IsSigned<T>>;

    if constexpr (sizeof(T) < originalTypeSize)
        CS_RUN(helpers::castToSmallerType<sizeof(T)>(originalValue, *static_cast<sameSizedInteger*>(static_cast<void*>(&const_cast<std::remove_const_t<T>&>(value)))))
    else
        helpers::castToBiggerType<sizeof(T)>(originalValue, *static_cast<sameSizedInteger*>(static_cast<void*>(&const_cast<std::remove_const_t<T>&>(value))));

    return Status::kNoError;
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::serializeSimplyAssignable(const T& value, context::SData<S, PM>&ctx)
{
    if constexpr (NotSimplyAssignable<T>)
        return Status::kErrorInvalidType;
    else
    {
        if constexpr (IsISerializableBased<T>)
            if (T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::kNoError;

        if constexpr (!EndiannessTolerant<T>)
            if (ctx.endiannessDifference())
                return Status::kNoError;

        if (   !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
            && (   AlwaysSimplyAssignable<T>
                || SimplyAssignableFixedSize<T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOne<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignable<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
               )
        )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(writeRawData(&value, 1, ctx));

            return Status::kNoFurtherProcessingRequired;
        }
        else
            return Status::kErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::deserializeSimplyAssignable(context::DData<D, PM>& ctx, T& value)
{
    if constexpr (NotSimplyAssignable<T>)
        return Status::kErrorInvalidType;
    else
    {
        if constexpr (IsISerializableBased<T>)
            if (T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::kNoError;

        if constexpr (!EndiannessTolerant<T>)
            if (ctx.endiannessDifference())
                return Status::kNoError;

        if (   !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
            && (   AlwaysSimplyAssignable<T>
                || SimplyAssignableFixedSize<T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOne<T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignable<T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
               )
        )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(readRawData(ctx, 1, &value));

            return Status::kNoFurtherProcessingRequired;
        }
        else
            return Status::kErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename T, ISerializationCapableContainer S, ISerializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::addPointerToMap(const T p, context::SData<S, PM>& ctx, bool& newPointer)
{
    if (!p)
    {
        newPointer = false;
        return serializeSizeT(static_cast<csp_size_t>(0), ctx);
    }
    else
    {
        PM& pointersMap = *ctx.getPointersMap();

        if (auto it = pointersMap.find(p); it == pointersMap.end())
        {
            newPointer = true;
            CS_RUN(serializeSizeT(static_cast<csp_size_t>(1), ctx));
            pointersMap[p] = ctx.getBinaryData().size();
            return Status::kNoError;
        }
        else
        {
            newPointer = false;
            return serializeSizeT(it->second, ctx);
        }
    }
}

template<typename T, IDeserializationCapableContainer D, IDeserializationPointersMap PM>
CS_ALWAYS_INLINE constexpr Status BodyProcessor::getPointerFromMap(context::DData<D, PM>& ctx, T& p, bool& newPointer)
{
    csp_size_t offset = 0;

    CS_RUN(deserializeSizeT(ctx, offset));

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
        if (offset >= ctx.getBinaryData().tell())
            return Status::kErrorInternal;

        p = reinterpret_cast<T>(pointersMap[offset]);
    }

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing::data
