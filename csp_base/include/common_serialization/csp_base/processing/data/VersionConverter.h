/**
 * @file common_serialization/csp_base/processing/data/VersionConverter.h
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

#include <common_serialization/csp_base/processing/data/BodyProcessor.h>

namespace common_serialization::csp::processing::data
{

class VersionConverter
{
public:
    template<typename _T>
    static constexpr Status toOldStruct(const _T& value, uint32_t targetVersion, context::SData& ctx);
    template<typename _T>
    static constexpr Status fromOldStruct(context::DData& ctx, uint32_t targetVersion, _T& value);

    template<typename _T>
    static constexpr Status toOldStructIfNeed(const _T& value, context::SData& ctx);
    template<typename _T>
    static constexpr Status fromOldStructIfNeed(context::DData& ctx, _T& value);

    template<ISerializableImpl... _NextTo>
    struct ToVersion;

    template<ISerializableImpl... _NextFrom>
    struct FromVersion;

    // Using to find index of version in versions hierarchy of struct to which we must serialize/deserialize
    template<typename _T>
    static constexpr [[nodiscard]] interface_version_t getBestCompatInterfaceVersion(interface_version_t compatInterfaceVersion) noexcept;
};

template<typename _T>
constexpr Status VersionConverter::toOldStruct(const _T& value, uint32_t targetVersion, context::SData& ctx)
{
    return Status::ErrorNoSuchHandler;
}

template<typename _T>
constexpr Status VersionConverter::fromOldStruct(context::DData& ctx, uint32_t targetVersion, _T& value)
{
    return Status::ErrorNoSuchHandler;
}

template<typename _T>
constexpr Status VersionConverter::toOldStructIfNeed(const _T& value, context::SData& ctx)
{
    if constexpr (!ISerializableImpl<_T>)
        return Status::ErrorInvalidType;
    else
    {
        uint32_t targetVersion = getBestCompatInterfaceVersion<_T>(ctx.getInterfaceVersion());

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
constexpr Status VersionConverter::fromOldStructIfNeed(context::DData& ctx, _T& value)
{
    if constexpr (!ISerializableImpl<_T>)
        return Status::ErrorInvalidType;
    else
    {
        uint32_t targetVersion = getBestCompatInterfaceVersion<_T>(ctx.getInterfaceVersion());

        if (targetVersion == value.getLatestPrivateVersion())
            return Status::NoError;
        // Normaly, next condition shall never succeed
        else if (targetVersion == traits::kInterfaceVersionUndefined)
            return Status::ErrorInternal;
        else
            return fromOldStruct(ctx, targetVersion, value);
    }
}

template<>
class VersionConverter::ToVersion<>
{
public:
    explicit ToVersion(uint32_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    template<ISerializableImpl _From>
    Status convertOnHeap(const _From& from, context::SData& ctx) noexcept
    {
        return Status::ErrorInternal;
    }

    template<ISerializableImpl _From>
    Status convertOnStack(const _From& from, context::SData& ctx) noexcept
    {
        return Status::ErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};

template<ISerializableImpl _To, ISerializableImpl... _NextTo>
class VersionConverter::ToVersion<_To, _NextTo...> : public VersionConverter::ToVersion<_NextTo...>
{
public:
    explicit ToVersion(interface_version_t targetVersion)
        : ToVersion<_NextTo...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableImpl _From>
    Status convert(const _From& from, context::SData& ctx)
    {
        return ctx.isHeapUsedForTemp() ? convertOnHeap(from, ctx) : convertOnStack(from, ctx);
    }

protected:
    using base_class = ToVersion<_NextTo...>;

    template<ISerializableImpl _From>
    Status convertOnHeap(const _From& from, context::SData& ctx)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<CtorGenAllocationManagerT<_To>>(1))
            return Status::ErrorNoMemory;

        AGS_CS_RUN(pointerKeeper.get<_To>()->init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnHeap(*pointerKeeper.get<_To>(), ctx);
        else
            return BodyProcessor::serialize(*pointerKeeper.get<_To>(), ctx);
    }

    template<ISerializableImpl _From>
    Status convertOnStack(const _From& from, context::SData& ctx)
    {
        _To to;
        AGS_CS_RUN(to.init(from));

        if (privateVersion > getTargetVersion())
            return base_class::convertOnStack(to, ctx);
        else
            return BodyProcessor::serialize(to, ctx);
    }

    static constexpr interface_version_t privateVersion = _To::getLatestPrivateVersion();
};

template<>
class VersionConverter::FromVersion<>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    template<ISerializableImpl _To>
    Status convert(context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

    constexpr interface_version_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    using from_type = Dummy; // placeholder

    static constexpr interface_version_t privateVersion = traits::kInterfaceVersionUndefined;

    template<ISerializableImpl _To>
    Status convertToUpperVersionOnHeap(from_type&& from, context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

    template<ISerializableImpl _To>
    Status convertToUpperVersionOnStack(const from_type& from, context::DData& ctx, _To& to) noexcept
    {
        return Status::ErrorInternal;
    }

private:
    interface_version_t m_targetVersion{ traits::kInterfaceVersionUndefined };
};


template<ISerializableImpl _From, ISerializableImpl... _NextFrom>
class VersionConverter::FromVersion<_From, _NextFrom...> : public VersionConverter::FromVersion<_NextFrom...>
{
public:
    explicit FromVersion(interface_version_t targetVersion)
        : FromVersion<_NextFrom...>(targetVersion)
    { }

    constexpr interface_version_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<ISerializableImpl _To>
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

    template<ISerializableImpl _To>
    Status convertOnHeap(context::DData& ctx, _To& to)
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<CtorGenAllocationManagerT<_From>>(1))
            return Status::ErrorNoMemory;

        AGS_CS_RUN(BodyProcessor::deserialize(ctx, *pointerKeeper.get<_From>()));

        return convertToUpperVersionOnHeap(std::move(*pointerKeeper.get<_From>()), ctx, to);
    }

    template<ISerializableImpl _To>
    Status convertOnStack(context::DData& ctx, _To& to)
    {
        _From from;
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, from));

        return convertToUpperVersionOnStack(from, ctx, to);
    }

    template<ISerializableImpl _To>
    Status convertToUpperVersionOnHeap(_From&& from, context::DData& ctx, _To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            GenericPointerKeeper pointerKeeper;
            if (!pointerKeeper.allocateAndConstruct<CtorGenAllocationManagerT<base_from>>(1))
                return Status::ErrorNoMemory;

            if constexpr (InitableBySpecialArgs<_To, base_from>)
                AGS_CS_RUN(pointerKeeper.get<base_from>()->init(std::move(from)))
            else
                return Status::ErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnHeap(std::move(*pointerKeeper.get<base_from>()), ctx, to);
        }
        else if constexpr (InitableBySpecialArgs<_To, _From>)
            return to.init(std::move(from));
        else
            return Status::ErrorNoSuchHandler;
    }

    template<ISerializableImpl _To>
    Status convertToUpperVersionOnStack(const _From& from, context::DData& ctx, _To& to)
    {
        if (base_class::privateVersion != traits::kInterfaceVersionUndefined)
        {
            base_from bFrom;

            if constexpr (InitableBySpecialArgs<_To, base_from>)
                AGS_CS_RUN(bFrom.init(from))
            else
                return Status::ErrorNoSuchHandler;

            return base_class::convertToUpperVersionOnStack(bFrom, ctx, to);
        }
        else if constexpr (InitableBySpecialArgs<_To, _From>)
            return to.init(from);
        else
            return Status::ErrorNoSuchHandler;
    }
};

template<typename _T>
constexpr interface_version_t VersionConverter::getBestCompatInterfaceVersion(interface_version_t compatInterfaceVersion) noexcept
{
    const interface_version_t* pPrivateVersions = _T::getPrivateVersions();

    for (interface_version_t i = 0; i < _T::getPrivateVersionsCount(); ++i)
        if (pPrivateVersions[i] <= compatInterfaceVersion)
            return pPrivateVersions[i];

    return traits::kInterfaceVersionUndefined;
}

} // namespace common_serialization::csp::processing::data
