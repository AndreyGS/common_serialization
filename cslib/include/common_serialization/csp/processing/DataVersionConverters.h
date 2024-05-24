/**
 * @file cslib/include/common_serialization/csp/processing/DataVersionConverters.h
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

#include "common_serialization/csp/processing/DataBodyProcessor.h"

namespace common_serialization::csp::processing::data::version_converters
{

template<typename _T>
constexpr Status toOldStruct(const _T& value, uint32_t targetVersion, context::SData& ctx);
template<typename _T>
constexpr Status fromOldStruct(context::DData& ctx, uint32_t targetVersion, _T& value);

template<typename _T>
constexpr Status toOldStruct(const _T& value, uint32_t targetVersion, context::SData& ctx)
{
    return Status::ErrorNoSuchHandler;
}

template<typename _T>
constexpr Status fromOldStruct(context::DData& ctx, uint32_t targetVersion, _T& value)
{
    return Status::ErrorNoSuchHandler;
}

template<typename _T>
constexpr Status toOldStructIfNeed(const _T& value, context::SData& ctx)
{
    if constexpr (NotISerializableBased<_T>)
        return Status::ErrorInvalidType;
    else
    {
        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<_T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::NoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::ErrorInternal;
        else
            return toOldStruct(value, targetVersion, ctx);
    }
}

template<typename _T>
constexpr Status fromOldStructIfNeed(context::DData& ctx, _T& value)
{
    if constexpr (NotISerializableBased<_T>)
        return Status::ErrorInvalidType;
    else
    {
        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<_T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::NoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::ErrorInternal;
        else
            return fromOldStruct(ctx, targetVersion, value);
    }
}

template<typename... _NextTo>
struct ToVersion;

template<>
class ToVersion<>
{
public:
    explicit ToVersion(uint32_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    template<typename _From>
    Status convertOnHeap(const _From& from, context::SData& ctx) noexcept
    {
        return Status::ErrorInternal;
    }

    template<typename _From>
    Status convertOnStack(const _From& from, context::SData& ctx) noexcept
    {
        return Status::ErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};

template<ISerializableBased _To, typename... _NextTo>
class ToVersion<_To, _NextTo...> : public ToVersion<_NextTo...>
{
public:
    explicit ToVersion(interface_version_t targetVersion)
        : ToVersion<_NextTo...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableBased _From>
    Status convert(const _From& from, context::SData& ctx)
    {
        return ctx.isHeapUsedForTemp() ? convertOnHeap(from, ctx) : convertOnStack(from, ctx);
    }

protected:
    using base_class = ToVersion<_NextTo...>;

    template<ISerializableBased _From>
    Status convertOnHeap(const _From& from, context::SData& ctx)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<_To, GenericAllocatorHelper<_To, ConstructorNoexceptAllocator<_To>>>(1))
            return Status::ErrorNoMemory;

        CS_RUN(pointerKeeper.get<_To>()->init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnHeap(*pointerKeeper.get<_To>(), ctx);
        else
            return BodyProcessor::serialize(*pointerKeeper.get<_To>(), ctx);
    }

    template<ISerializableBased _From>
    Status convertOnStack(const _From& from, context::SData& ctx)
    {
        _To to;
        CS_RUN(to.init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnStack(to, ctx);
        else
            return BodyProcessor::serialize(to, ctx);
    }

    static constexpr interface_version_t privateVersion = _To::getLatestPrivateVersion();
};

template<typename... _NextFrom>
struct FromVersion;

template<>
class FromVersion<>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    template<ISerializableBased _To>
    Status convert(context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

    constexpr interface_version_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    using from_type = Dummy; // placeholder

    static constexpr interface_version_t privateVersion = traits::kInterfaceVersionUndefined;

    template<ISerializableBased _To>
    Status convertToUpperVersionOnHeap(const from_type& from, context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

    template<ISerializableBased _To>
    Status convertToUpperVersionOnStack(const from_type& from, context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};


template<ISerializableBased _From, typename... _NextFrom>
class FromVersion<_From, _NextFrom...> : public FromVersion<_NextFrom...>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : FromVersion<_NextFrom...>(targetVersion)
    { }

    constexpr interface_version_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableBased _To>
    Status convert(context::DData& ctx, _To& to)
    {
        // Skip versions that are older than serialized one
        if (base_class::privateVersion <= getTargetVersion())
            return base_class::convert(ctx, to);
        else
            return ctx.isHeapUsedForTemp() ? convertOnHeap(ctx, to) : convertOnStack(ctx, to);
    }

protected:
    using base_class = FromVersion<_NextFrom...>;
    using from_type = _From;
    using base_from = typename base_class::from_type;

    static constexpr interface_version_t privateVersion = _From::getLatestPrivateVersion();

    template<ISerializableBased _To>
    Status convertOnHeap(context::DData& ctx, _To& to)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<_From, GenericAllocatorHelper<_From, ConstructorNoexceptAllocator<_From>>>(1))
            return Status::ErrorNoMemory;

        CS_RUN(BodyProcessor::deserialize(ctx, *pointerKeeper.get<_From>()));

        return convertToUpperVersionOnHeap(*pointerKeeper.get<_From>(), ctx, to);
    }

    template<ISerializableBased _To>
    Status convertOnStack(context::DData& ctx, _To& to)
    {
        _From from;
        CS_RUN(BodyProcessor::deserialize(ctx, from));

        return convertToUpperVersionOnStack(from, ctx, to);
    }

    template<ISerializableBased _To>
    Status convertToUpperVersionOnHeap(const _From& from, context::DData& ctx, _To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            GenericPointerKeeper pointerKeeper;
            if (!pointerKeeper.allocateAndConstruct<base_from, GenericAllocatorHelper<base_from, ConstructorNoexceptAllocator<base_from>>>(1))
                return Status::ErrorNoMemory;

            if constexpr (InitableBySpecialClass<_To, base_from>)
                CS_RUN(pointerKeeper.get<base_from>()->init(from))
            else
                return Status::ErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnHeap(*pointerKeeper.get<base_from>(), ctx, to);
        }
        else if constexpr (InitableBySpecialClass<_To, _From>)
            return to.init(from);
        else
            return Status::ErrorNoSuchHandler;
    }

    template<ISerializableBased _To>
    Status convertToUpperVersionOnStack(const _From& from, context::DData& ctx, _To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            base_from bFrom;

            if constexpr (InitableBySpecialClass<_To, base_from>)
                CS_RUN(bFrom.init(from))
            else
                return Status::ErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnStack(bFrom, ctx, to);
        }
        else if constexpr (InitableBySpecialClass<_To, _From>)
            return to.init(from);
        else
            return Status::ErrorNoSuchHandler;
    }
};

} // namespace common_serialization::csp::processing::data::version_converters
