/**
 * @file common_serialization/csp_base/processing/data/BodyProcessor.h
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

#include <common_serialization/csp_base/Interface.h>
#include <common_serialization/csp_base/context/Data.h>
#include <common_serialization/csp_base/processing/rw.h>
#include <common_serialization/csp_base/processing/data/ContextProcessor.h>

namespace common_serialization::csp::processing::data
{

template<typename _T, typename... _Ts>
class TemplateProcessor;

class VersionConverter;

class BodyProcessor
{
public:
    template<typename _T>
    static constexpr Status serialize(const _T* p, csp_size_t n, context::SData& ctx);
    template<typename _T, csp_size_t _N>
    static constexpr Status serialize(const _T(&arr)[_N], context::SData& ctx);
    template<typename _T>
    static constexpr Status serialize(const _T& value, context::SData& ctx);

    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status serializeSizeT(_T value, context::SData& ctx);

    /// @brief For now using for size_t serialization only
    /// @tparam _T size_t
    /// @param targetTypeSize Size of type to which will be serialized
    /// @param value size_t value
    /// @param ctx CSP Full Data Context
    /// @return Status of operation
    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status serializeToAnotherSize(csp_size_t targetTypeSize, _T value, context::SData& ctx);

    template<typename _T>
    static constexpr Status deserialize(context::DData& ctx, csp_size_t n, _T* p);
    template<typename _T, csp_size_t _N>
    static constexpr Status deserialize(context::DData& ctx, _T(&arr)[_N]);
    template<typename _T>
    static constexpr Status deserialize(context::DData& ctx, _T& value);

    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status deserializeSizeT(context::DData& ctx, _T& value);

    /// @brief Using for sizeOfIntegersMayBeNotEqual mode and size_t deserialization
    /// @tparam _T Integral type or enum
    /// @param originalTypeSize Size of buffer that holds current value in serialized context
    /// @param ctx CSP Full Data Context
    /// @param value Output value
    /// @return Status of operation
    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status deserializeFromAnotherSize(csp_size_t originalTypeSize, context::DData& ctx, _T& value);

protected:
    template<csp_size_t _targetTypeSize, typename _T>
    static constexpr Status serializeToAnotherSizeInternal(_T value, context::SData& ctx);
    template<csp_size_t _originalTypeSize, typename _T>
    static constexpr Status deserializeFromAnotherSizeInternal(context::DData& ctx, _T& value);

    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status serializeSimplyAssignable(const _T& value, context::SData& ctx);
    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status deserializeSimplyAssignable(context::DData& ctx, _T& value);

    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status addPointerToMap(const _T p, context::SData& ctx, bool& newPointer);
    template<typename _T>
    static CS_ALWAYS_INLINE constexpr Status getPointerFromMap(context::DData& ctx, _T& p, bool& newPointer);


    template<template<typename...> typename _T, typename... _Ts>
    static CS_ALWAYS_INLINE Status templateProcessorSerializationWrapper(const _T<_Ts...>& value, context::SData& ctx);
    template<template<typename...> typename _T, typename... _Ts>
    static CS_ALWAYS_INLINE Status templateProcessorDeserializationWrapper(context::DData& ctx, _T<_Ts...>& value);

private:
    static constexpr size_t kMaxSizeOfIntegral = 8;   // maximum allowed size of integral type
};

// can be copied with memcpy (if alignments are the same and no kSizeOfIntegersMayBeNotEqual flag is set)
template<typename T>
concept SimplyAssignable
    =  requires(T t) { typename normalize_t<T>::simply_assignable_tag; };

// can be copied with memcpy (if no kSizeOfIntegersMayBeNotEqual flag is set)
template<typename T>
concept SimplyAssignableAlignedToOne
    = requires(T t) { typename normalize_t<T>::simply_assignable_aligned_to_one_tag; } && alignof(T) == 1;

// all fields must be primitives an with strictly defined sizes regardless of execution environment
// can be copied with memcpy (if alignments are the same)
template<typename T>
concept SimplyAssignableFixedSize
    =  requires(T t) { typename normalize_t<T>::simply_assignable_fixed_size_tag; };

// same as FixedSizeSimplyAssignableType but with alignment to 1 requirement (type must always have same size)
// can be copied with memcpy
template<typename T>
concept AlwaysSimplyAssignable
    =  requires(T t) { typename normalize_t<T>::always_simply_assignable_tag; } && alignof(T) == 1;

template<typename T>
concept AnySimplyAssignable = AlwaysSimplyAssignable<T> || SimplyAssignableFixedSize<T> || SimplyAssignableAlignedToOne<T> || SimplyAssignable<T>;

template<typename T>
concept EndiannessTolerant 
    = requires(T t) { typename normalize_t<T>::endianness_tolerant_tag; } || sizeof(T) == 1;

template<typename T>
concept NotSimplyAssignable = !AnySimplyAssignable<T>;

template<typename T>
concept EmptyType 
    =  requires(T t) { typename normalize_t<T>::empty_type_tag; };

/// @brief Shortcut to get type interface version in template contexts
/// @tparam _T Type for which interface version is requested
/// @return Latest version of interface if _T implements ISerializable and 0 otherwise
template<typename _T>
consteval interface_version_t getLatestInterfaceVersion()
{
    if constexpr (ISerializableImpl<_T>)
        return _T::getLatestInterfaceVersion();
    else
        return 0;
}

template<typename _T>
constexpr Status BodyProcessor::serialize(const _T* p, csp_size_t n, context::SData& ctx)
{
    static_assert(!std::is_reference_v<_T> || !std::is_member_pointer_v<_T>
                    , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<_T> || std::is_member_function_pointer_v<_T>)
                    , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);
    assert(n <= SIZE_MAX);

    if constexpr (EmptyType<_T>)
        return Status::NoError;

    if (
           (!ctx.endiannessDifference() || EndiannessTolerant<_T>)
        && (   std::is_arithmetic_v<_T>
            || std::is_enum_v<_T>
            || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
                && (!ISerializableImpl<_T> || getLatestInterfaceVersion<_T>() <= ctx.getInterfaceVersion())
                && (   AlwaysSimplyAssignable<_T>
                    || SimplyAssignableFixedSize<_T> && !ctx.alignmentMayBeNotEqual()
                    || SimplyAssignableAlignedToOne<_T> && !ctx.sizeOfIntegersMayBeNotEqual()
                    || SimplyAssignable<_T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
            )
    )
    {   
        if constexpr ((std::is_arithmetic_v<_T> || std::is_enum_v<_T>) && !FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
                CS_RUN(writePrimitive(static_cast<uint8_t>(sizeof(_T)), ctx));

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

        return Status::NoError;
    }
}

template<typename _T, csp_size_t _N>
constexpr Status BodyProcessor::serialize(const _T(&arr)[_N], context::SData& ctx)
{
    static_assert(_N == sizeof(_T) * _N / sizeof(_T), "Oveflow occured in (sizeof(_T) * _N) in instantiation of array input of serialize");

    return serialize(arr, _N, ctx);
}

template<typename _T>
constexpr Status BodyProcessor::serialize(const _T& value, context::SData& ctx)
{
    static_assert(!std::is_reference_v<_T> || !std::is_member_pointer_v<_T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<_T> || std::is_member_function_pointer_v<_T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<_T> || std::is_enum_v<_T>)
    {
        if constexpr (!FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
                CS_RUN(writePrimitive(static_cast<uint8_t>(sizeof(_T)), ctx));

        return writePrimitive(value, ctx);
    }
    else if constexpr (std::is_pointer_v<_T>)
    {
        if (!ctx.allowUnmanagedPointers())
            return Status::ErrorNotSupportedSerializationSettingsForStruct;

        if (ctx.checkRecursivePointers())
        {
            bool newPointer = false;
            CS_RUN(addPointerToMap(value, ctx, newPointer));

            if (!newPointer)
                return Status::NoError;
        }
        else
        {
            if (value == nullptr)
                return writePrimitive(uint8_t(0), ctx);
            else
                CS_RUN(writePrimitive(uint8_t(1), ctx));
        }

        return serialize(*value, ctx);
    }
    else if constexpr (AnySimplyAssignable<_T>)
        return serializeSimplyAssignable(value, ctx);
    else if constexpr (EmptyType<_T>)
        return Status::NoError;
    else if constexpr (!ISerializableImpl<_T> && is_template_v<_T>)
        return templateProcessorSerializationWrapper(value, ctx);
    else
        static_assert("Type has no applicable function for serialization");
}

template<typename _T>
constexpr Status BodyProcessor::serializeSizeT(_T value, context::SData& ctx)
{
    return serializeToAnotherSize(!ctx.bitness32() ? 8 : 4, value, ctx);
}

template<typename _T>
constexpr Status BodyProcessor::serializeToAnotherSize(csp_size_t targetTypeSize, _T value, context::SData& ctx)
{
    switch (targetTypeSize)
    {
    case 8: return serializeToAnotherSizeInternal<8>(value, ctx);
    case 4: return serializeToAnotherSizeInternal<4>(value, ctx);
    case 2: return serializeToAnotherSizeInternal<2>(value, ctx);
    case 1: return serializeToAnotherSizeInternal<1>(value, ctx);
    default:
        assert(false);
        return Status::ErrorInternal;
    }
}

template<size_t _targetTypeSize, typename _T>
constexpr Status BodyProcessor::serializeToAnotherSizeInternal(_T value, context::SData& ctx)
{
    static_assert((std::is_integral_v<_T> || std::is_enum_v<_T>) && !FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>
        , "Current serialize function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if constexpr (kMaxSizeOfIntegral < _targetTypeSize)
        return Status::ErrorTypeSizeIsTooBig;

    if constexpr (sizeof(_T) == _targetTypeSize)
        return writePrimitive(value, ctx);

    helpers::fixed_width_integer_t<_targetTypeSize, Signed<_T>> targetValue = 0;

    if constexpr (sizeof(_T) < _targetTypeSize)
        helpers::castToBiggerType(value, targetValue);
    else
        CS_RUN(helpers::castToSmallerType<_targetTypeSize>(value, targetValue));

    return writePrimitive(targetValue, ctx);
}

// common function for pointers of known size
template<typename _T>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, csp_size_t n, _T* p)
{
    static_assert(!std::is_reference_v<_T> || !std::is_member_pointer_v<_T>
        , "Pointers on references and member_pointers are not allowed");
    static_assert(!(std::is_function_v<_T> || std::is_member_function_pointer_v<_T>)
        , "References and pointers on functions are not allowed to be serialized");

    assert(p && n > 0 || n == 0);

    if constexpr (EmptyType<_T>)
        return Status::NoError;

    if (
           (!ctx.endiannessDifference() || EndiannessTolerant<_T>)
        && (   std::is_arithmetic_v<_T>
            || std::is_enum_v<_T>
            || !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
                && (!ISerializableImpl<_T> || getLatestInterfaceVersion<_T>() <= ctx.getInterfaceVersion())
                && (   AlwaysSimplyAssignable<_T>
                    || SimplyAssignableFixedSize<_T> && !ctx.alignmentMayBeNotEqual()
                    || SimplyAssignableAlignedToOne<_T> && !ctx.sizeOfIntegersMayBeNotEqual()
                    || SimplyAssignable<_T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual())
            )
    )
    {
        // In fact ctx.sizeOfIntegersMayBeNotEqual() can be true only if (std::is_arithmetic_v<_T> || std::is_enum_v<_T>) is true,
        // but if we do not wrap this in constexpr statement, all SimplyAssignable types would be forced to have deserialize functions
        if constexpr ((std::is_arithmetic_v<_T> || std::is_enum_v<_T>) && !FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(readPrimitive(ctx, originalTypeSize));
                if (originalTypeSize != sizeof(_T))
                {
                    for (csp_size_t i = 0; i < n; ++i)
                        CS_RUN(deserializeFromAnotherSize(originalTypeSize, ctx, p[i]));

                    return Status::NoError;
                }
            }

        csp_size_t readSize = 0;

        return readRawData(ctx, n, p);
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            _T* pItem = new (&p[i]) _T;

            if (ctx.checkRecursivePointers())
                (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = const_cast<from_ptr_to_const_to_ptr_t<_T*>>(pItem);

            CS_RUN(deserialize(ctx, *pItem));
        }

        return Status::NoError;
    }
}

// common function for arrays
template<typename _T, csp_size_t _N>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, _T(&arr)[_N])
{
    static_assert(_N == sizeof(_T) * _N / sizeof(_T), "Oveflow occured in (sizeof(_T) * _N) in instantiation of array input of serialize");

    return deserialize(ctx, _N, arr);
}

// common function for scalar and simple assignable types
template<typename _T>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, _T& value)
{
    static_assert(!std::is_reference_v<_T> || !std::is_member_pointer_v<_T>
        , "References and member pointers not allowed");
    static_assert(!(std::is_function_v<_T> || std::is_member_function_pointer_v<_T>)
        , "References and pointers on functions are not allowed to be serialized");

    if constexpr (std::is_arithmetic_v<_T> || std::is_enum_v<_T>)
    {
        if constexpr (!FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>)
            if (ctx.sizeOfIntegersMayBeNotEqual())
            {
                uint8_t originalTypeSize = 0;
                CS_RUN(readPrimitive(ctx, originalTypeSize));
                return deserializeFromAnotherSize(originalTypeSize, ctx, value);
            }

        return readPrimitive(ctx, value);
    }
    else if constexpr (std::is_pointer_v<_T>)
    {
        if (!ctx.allowUnmanagedPointers())
            return Status::ErrorNotSupportedSerializationSettingsForStruct;

        if (ctx.checkRecursivePointers())
        {
            bool newPointer = false;
            CS_RUN(getPointerFromMap(ctx, value, newPointer));

            if (!newPointer)
                return Status::NoError;
        }
        else
        {
            uint8_t isValidPtr = 0;
            CS_RUN(readPrimitive(ctx, isValidPtr));
            if (!isValidPtr)
            {
                value = nullptr;
                return Status::NoError;
            }
        }

        value = ctx.template allocateAndDefaultConstruct<std::remove_const_t<std::remove_pointer_t<_T>>>();
        if (!value)
            return Status::ErrorNoMemory;

        if (ctx.checkRecursivePointers())
            (*ctx.getPointersMap())[ctx.getBinaryData().tell()] = *const_cast<from_ptr_to_const_to_ptr_t<_T>*>(&value);

        return deserialize(ctx, *value);
    }
    else if constexpr (AnySimplyAssignable<_T>)
        return deserializeSimplyAssignable(ctx, value);
    else if constexpr (EmptyType<_T>)
        return Status::NoError;
    else if constexpr (!ISerializableImpl<_T> && is_template_v<_T>)
        return templateProcessorDeserializationWrapper(ctx, value);
    else
        static_assert("Type has no applicable function for deserialization");
}

template<typename _T>
constexpr Status BodyProcessor::deserializeSizeT(context::DData& ctx, _T& value)
{
    return deserializeFromAnotherSize(!ctx.bitness32() ? 8 : 4, ctx, value);
}

template<typename _T>
constexpr Status BodyProcessor::deserializeFromAnotherSize(csp_size_t originalTypeSize, context::DData& ctx, _T& value)
{
    switch (originalTypeSize)
    {
    case 8: return deserializeFromAnotherSizeInternal<8>(ctx, value);
    case 4: return deserializeFromAnotherSizeInternal<4>(ctx, value);
    case 2: return deserializeFromAnotherSizeInternal<2>(ctx, value);
    case 1: return deserializeFromAnotherSizeInternal<1>(ctx, value);
    default:
        assert(false);
        return Status::ErrorInternal;
    }
}

template<size_t _originalTypeSize, typename _T>
constexpr Status BodyProcessor::deserializeFromAnotherSizeInternal(context::DData& ctx, _T& value)
{
    static_assert((std::is_integral_v<_T> || std::is_enum_v<_T>) && !FixSizedArithmeticType<_T> && !FixSizedEnumType<_T>
        , "Current deserialize function overload is only for variable length arithmetic types and enums. You shouldn't be here.");

    if constexpr (kMaxSizeOfIntegral < _originalTypeSize)
        return Status::ErrorTypeSizeIsTooBig;

    if constexpr (sizeof(_T) == _originalTypeSize)
        return readPrimitive(ctx, value);

    Status status = Status::NoError;

    helpers::fixed_width_integer_t<_originalTypeSize, Signed<_T>> originalValue = 0;
    status = readPrimitive(ctx, originalValue);

    if (!statusSuccess(status))
    {
        if (status == Status::ErrorOverflow)
            return Status::ErrorDataCorrupted;
        return status;
    }

    using sameSizedInteger = helpers::fixed_width_integer_t<sizeof(_T), Signed<_T>>;

    if constexpr (sizeof(_T) < _originalTypeSize)
        CS_RUN(helpers::castToSmallerType<sizeof(_T)>(originalValue, *static_cast<sameSizedInteger*>(static_cast<void*>(&const_cast<std::remove_const_t<_T>&>(value)))))
    else
        helpers::castToBiggerType<sizeof(_T)>(originalValue, *static_cast<sameSizedInteger*>(static_cast<void*>(&const_cast<std::remove_const_t<_T>&>(value))));

    return Status::NoError;
}

template<typename _T>
constexpr Status BodyProcessor::serializeSimplyAssignable(const _T& value, context::SData&ctx)
{
    if constexpr (!AnySimplyAssignable<decltype(value)>)
        return Status::NoError;
    else
    {
        if constexpr (ISerializableImpl<_T>)
            if (_T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::NoError;

        if constexpr (!EndiannessTolerant<_T>)
            if (ctx.endiannessDifference())
                return Status::NoError;

        if (   !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
            && (   AlwaysSimplyAssignable<_T>
                || SimplyAssignableFixedSize<_T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOne<_T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignable<_T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
               )
        )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(writeRawData(&value, 1, ctx));

            return Status::NoFurtherProcessingRequired;
        }
        else
            return Status::ErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename _T>
constexpr Status BodyProcessor::deserializeSimplyAssignable(context::DData& ctx, _T& value)
{
    if constexpr (NotSimplyAssignable<_T>)
        return Status::ErrorInvalidType;
    else
    {
        if constexpr (ISerializableImpl<_T>)
            if (_T::getLatestInterfaceVersion() > ctx.getInterfaceVersion())
                return Status::NoError;

        if constexpr (!EndiannessTolerant<_T>)
            if (ctx.endiannessDifference())
                return Status::NoError;

        if (   !ctx.simplyAssignableTagsOptimizationsAreTurnedOff()
            && (   AlwaysSimplyAssignable<_T>
                || SimplyAssignableFixedSize<_T> && !ctx.alignmentMayBeNotEqual()
                || SimplyAssignableAlignedToOne<_T> && !ctx.sizeOfIntegersMayBeNotEqual()
                || SimplyAssignable<_T> && !ctx.alignmentMayBeNotEqual() && !ctx.sizeOfIntegersMayBeNotEqual()
               )
        )
        {
            // for simple assignable types it is preferable to get a whole struct at a time
            CS_RUN(readRawData(ctx, 1, &value));

            return Status::NoFurtherProcessingRequired;
        }
        else
            return Status::ErrorNotSupportedSerializationSettingsForStruct;
    }
}

template<typename _T>
constexpr Status BodyProcessor::addPointerToMap(const _T p, context::SData& ctx, bool& newPointer)
{
    if (!p)
    {
        newPointer = false;
        return serializeSizeT(static_cast<csp_size_t>(0), ctx);
    }
    else
    {
        auto& pointersMap = *ctx.getPointersMap();

        if (auto it = pointersMap.find(p); it == pointersMap.end())
        {
            newPointer = true;
            CS_RUN(serializeSizeT(static_cast<csp_size_t>(1), ctx));
            pointersMap[p] = ctx.getBinaryData().size();
            return Status::NoError;
        }
        else
        {
            newPointer = false;
            return serializeSizeT(it->second, ctx);
        }
    }
}

template<typename _T>
constexpr Status BodyProcessor::getPointerFromMap(context::DData& ctx, _T& p, bool& newPointer)
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
        auto& pointersMap = *ctx.getPointersMap();

        newPointer = false;
        if (offset >= ctx.getBinaryData().tell())
            return Status::ErrorInternal;

        p = reinterpret_cast<_T>(pointersMap[offset]);
    }

    return Status::NoError;
}

template<template<typename...> typename _T, typename... _Ts>
CS_ALWAYS_INLINE Status BodyProcessor::templateProcessorSerializationWrapper(const _T<_Ts...>& value, context::SData& ctx)
{
    return TemplateProcessor<_T<_Ts...>, _Ts...>::serialize(value, ctx);
}

template<template<typename...> typename _T, typename... _Ts>
CS_ALWAYS_INLINE Status BodyProcessor::templateProcessorDeserializationWrapper(context::DData& ctx, _T<_Ts...>& value)
{
    return TemplateProcessor<_T<_Ts...>, _Ts...>::deserialize(ctx, value);
}

} // namespace common_serialization::csp::processing::data

#define CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx)                                 \
{                                                                                       \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                 \
    {                                                                                   \
        Status status = serializeSimplyAssignable(value, ctx);                          \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (                                                                       \
                    !statusSuccess(status)                                              \
                && status != Status::ErrorNotSupportedSerializationSettingsForStruct    \
        )                                                                               \
            return status;                                                              \
                                                                                        \
        /* if we get Status::ErrorNotSupportedSerializationSettingsForStruct, */        \
        /* than we should serialize it field-by-field */                                \
    }                                                                                   \
}

#define CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)                               \
{                                                                                       \
    if constexpr (AnySimplyAssignable<decltype(value)>)                                 \
    {                                                                                   \
        Status status = deserializeSimplyAssignable(ctx, value);                        \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (                                                                       \
                    !statusSuccess(status)                                              \
                && status != Status::ErrorNotSupportedSerializationSettingsForStruct    \
        )                                                                               \
            return status;                                                              \
                                                                                        \
        /* if we get Status::ErrorNotSupportedSerializationSettingsForStruct, */        \
        /* than we should deserialize it field-by-field */                              \
   }                                                                                    \
}                                                            

#define CSP_SERIALIZE_COMMON(value, ctx)                                                \
{                                                                                       \
    if (                                                                                \
           ISerializableImpl<decltype(value)>                                           \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = VersionConverter::toOldStructIfNeed(value, ctx);                \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
        else                                                                            \
            CS_RUN(ContextProcessor::testDataFlagsCompatibility                         \
                    <normalize_t<decltype(value)>>(ctx.getDataFlags()));                \
    }                                                                                   \
                                                                                        \
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx)                                     \
}                                                                                       

#define CSP_DESERIALIZE_COMMON(ctx, value)                                              \
{                                                                                       \
    if (                                                                                \
           ISerializableImpl<decltype(value)>                                           \
        && ctx.isInterfaceVersionsNotMatch()                                            \
    )                                                                                   \
    {                                                                                   \
        Status status = VersionConverter::fromOldStructIfNeed(ctx, value);              \
                                                                                        \
        if (status == Status::NoFurtherProcessingRequired)                              \
            return Status::NoError;                                                     \
        else if (!statusSuccess(status))                                                \
            return status;                                                              \
        else                                                                            \
            CS_RUN(ContextProcessor::testDataFlagsCompatibility                         \
                    <normalize_t<decltype(value)>>(ctx.getDataFlags()));                \
    }                                                                                   \
                                                                                        \
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)                                   \
}

namespace common_serialization::csp::processing::data
{

template<>
constexpr Status BodyProcessor::serialize(const Id& value, context::SData& ctx)
{
    CS_RUN(serialize(value.m_high, ctx));
    CS_RUN(serialize(value.m_low, ctx));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, Id& value)
{
    CS_RUN(deserialize(ctx, value.m_high));
    CS_RUN(deserialize(ctx, value.m_low));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::serialize(const context::DataFlags& value, context::SData& ctx)
{
    CS_RUN(serialize(static_cast<uint32_t>(value), ctx));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, context::DataFlags& value)
{
    uint32_t dataFlags{ 0 };
    CS_RUN(deserialize(ctx, dataFlags));
    value = dataFlags;

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::serialize(const Interface& value, context::SData& ctx)
{
    CSP_SERIALIZE_ANY_SIMPLY_ASSIGNABLE(value, ctx);

    CS_RUN(serialize(value.id, ctx));
    CS_RUN(serialize(value.version, ctx));
    CS_RUN(serialize(value.mandatoryDataFlags, ctx));
    CS_RUN(serialize(value.forbiddenDataFlags, ctx));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, Interface& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)

    CS_RUN(deserialize(ctx, value.id));
    CS_RUN(deserialize(ctx, value.version));
    CS_RUN(deserialize(ctx, value.mandatoryDataFlags));
    CS_RUN(deserialize(ctx, value.forbiddenDataFlags));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
