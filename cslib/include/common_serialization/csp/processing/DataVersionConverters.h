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

#include "common_serialization/csp/processing/DataBodyProcessor.h"

namespace common_serialization::csp::processing::data::version_converters
{

template<typename T>
constexpr Status toOldStruct(const T& value, uint32_t targetVersion, context::SData& ctx);
template<typename T>
constexpr Status fromOldStruct(context::DData& ctx, uint32_t targetVersion, T& value);

template<typename T>
constexpr Status toOldStruct(const T& value, uint32_t targetVersion, context::SData& ctx)
{
    return Status::kErrorNoSuchHandler;
}

template<typename T>
constexpr Status fromOldStruct(context::DData& ctx, uint32_t targetVersion, T& value)
{
    return Status::kErrorNoSuchHandler;
}

template<typename T>
constexpr Status toOldStructIfNeed(const T& value, context::SData& ctx)
{
    if constexpr (NotISerializableBased<T>)
        return Status::kErrorInvalidType;
    else
    {
        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::kNoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInternal;
        else
            return toOldStruct(value, targetVersion, ctx);
    }
}

template<typename T>
constexpr Status fromOldStructIfNeed(context::DData& ctx, T& value)
{
    if constexpr (NotISerializableBased<T>)
        return Status::kErrorInvalidType;
    else
    {
        uint32_t targetVersion = traits::getBestCompatInterfaceVersion<T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::kNoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::kErrorInternal;
        else
            return fromOldStruct(ctx, targetVersion, value);
    }
}

template<typename... NextTo>
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
    template<typename From>
    Status convertOnHeap(const From& from, context::SData& ctx) noexcept
    {
        return Status::kErrorInternal;
    }

    template<typename From>
    Status convertOnStack(const From& from, context::SData& ctx) noexcept
    {
        return Status::kErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};

template<ISerializableBased To, typename... NextTo>
class ToVersion<To, NextTo...> : public ToVersion<NextTo...>
{
public:
    explicit ToVersion(interface_version_t targetVersion)
        : ToVersion<NextTo...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableBased From>
    Status convert(const From& from, context::SData& ctx)
    {
        return ctx.isHeapUsedForTemp() ? convertOnHeap(from, ctx) : convertOnStack(from, ctx);
    }

protected:
    using base_class = ToVersion<NextTo...>;

    template<ISerializableBased From>
    Status convertOnHeap(const From& from, context::SData& ctx)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<To, GenericAllocatorHelper<To, ConstructorNoexceptAllocator<To>>>(1))
            return Status::kErrorNoMemory;

        CS_RUN(pointerKeeper.get<To>()->init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnHeap(*pointerKeeper.get<To>(), ctx);
        else
            return BodyProcessor::serialize(*pointerKeeper.get<To>(), ctx);
    }

    template<ISerializableBased From>
    Status convertOnStack(const From& from, context::SData& ctx)
    {
        To to;
        CS_RUN(to.init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnStack(to, ctx);
        else
            return BodyProcessor::serialize(to, ctx);
    }

    static constexpr interface_version_t privateVersion = To::getLatestPrivateVersion();
};

template<typename... NextFrom>
struct FromVersion;

template<>
class FromVersion<>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    template<ISerializableBased To>
    Status convert(context::DData& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    constexpr interface_version_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    using from_type = Dummy; // placeholder

    static constexpr interface_version_t privateVersion = traits::kInterfaceVersionUndefined;

    template<ISerializableBased To>
    Status convertToUpperVersionOnHeap(const from_type& from, context::DData& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    template<ISerializableBased To>
    Status convertToUpperVersionOnStack(const from_type& from, context::DData& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};


template<ISerializableBased From, typename... NextFrom>
class FromVersion<From, NextFrom...> : public FromVersion<NextFrom...>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : FromVersion<NextFrom...>(targetVersion)
    { }

    constexpr interface_version_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableBased To>
    Status convert(context::DData& ctx, To& to)
    {
        // Skip versions that are older than serialized one
        if (base_class::privateVersion <= getTargetVersion())
            return base_class::convert(ctx, to);
        else
            return ctx.isHeapUsedForTemp() ? convertOnHeap(ctx, to) : convertOnStack(ctx, to);
    }

protected:
    using base_class = FromVersion<NextFrom...>;
    using from_type = From;
    using base_from = typename base_class::from_type;

    static constexpr interface_version_t privateVersion = From::getLatestPrivateVersion();

    template<ISerializableBased To>
    Status convertOnHeap(context::DData& ctx, To& to)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<From, GenericAllocatorHelper<From, ConstructorNoexceptAllocator<From>>>(1))
            return Status::kErrorNoMemory;

        CS_RUN(BodyProcessor::deserialize(ctx, *pointerKeeper.get<From>()));

        return convertToUpperVersionOnHeap(*pointerKeeper.get<From>(), ctx, to);
    }

    template<ISerializableBased To>
    Status convertOnStack(context::DData& ctx, To& to)
    {
        From from;
        CS_RUN(BodyProcessor::deserialize(ctx, from));

        return convertToUpperVersionOnStack(from, ctx, to);
    }

    template<ISerializableBased To>
    Status convertToUpperVersionOnHeap(const From& from, context::DData& ctx, To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            GenericPointerKeeper pointerKeeper;
            if (!pointerKeeper.allocateAndConstruct<base_from, GenericAllocatorHelper<base_from, ConstructorNoexceptAllocator<base_from>>>(1))
                return Status::kErrorNoMemory;

            if constexpr (InitableBySpecialClass<To, base_from>)
                CS_RUN(pointerKeeper.get<base_from>()->init(from))
            else
                return Status::kErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnHeap(*pointerKeeper.get<base_from>(), ctx, to);
        }
        else if constexpr (InitableBySpecialClass<To, From>)
            return to.init(from);
        else
            return Status::kErrorNoSuchHandler;
    }

    template<ISerializableBased To>
    Status convertToUpperVersionOnStack(const From& from, context::DData& ctx, To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            base_from bFrom;

            if constexpr (InitableBySpecialClass<To, base_from>)
                CS_RUN(bFrom.init(from))
            else
                return Status::kErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnStack(bFrom, ctx, to);
        }
        else if constexpr (InitableBySpecialClass<To, From>)
            return to.init(from);
        else
            return Status::kErrorNoSuchHandler;
    }
};

} // namespace common_serialization::csp::processing::data::version_converters
