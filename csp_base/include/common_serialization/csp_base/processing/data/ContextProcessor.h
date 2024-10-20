/**
 * @file common_serialization/csp_base/processing/data/ContextProcessor.h
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

#include <common_serialization/csp_base/context/Data.h>
#include <common_serialization/csp_base/processing/rw.h>

namespace common_serialization::csp::processing::data
{

template<typename T>
concept StructHaveDataFlags = requires
{
    { T::getEffectiveMandatoryDataFlags() } -> std::same_as<context::DataFlags>;
    { T::getEffectiveForbiddenDataFlags() } -> std::same_as<context::DataFlags>;
};

class ContextProcessor
{
public:
    template<typename T>
    static AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] Status testDataFlagsCompatibility(context::DataFlags dataFlags);

    // This function does not check presence of pointer map.
    // It is library code responsibility to provide it if it is absent.
    template<ISerializableImpl T>
    static constexpr Status serialize(context::SData& ctx);
    template<ISerializableImpl T>
    static constexpr Status serializeNoChecks(context::SData& ctx);

    static constexpr Status deserializeNoChecks(context::DData& ctx, Id& id) noexcept;
    template<ISerializableImpl T>
    static AGS_CS_ALWAYS_INLINE constexpr Status deserializePostprocessId(const Id& id) noexcept;
    template<ISerializableImpl T>
    static constexpr Status deserializePostprocessRest(context::DData& ctx, interface_version_t minimumSupportedInterfaceVersion) noexcept;
};

template<typename T>
constexpr Status ContextProcessor::testDataFlagsCompatibility(context::DataFlags dataFlags)
{
    if constexpr (!StructHaveDataFlags<T>)
        return Status::NoError;
    else
    {
        constexpr context::DataFlags effectiveMandatoryDataFlags = T::getEffectiveMandatoryDataFlags();
        constexpr context::DataFlags forbiddenDataFlags = T::getEffectiveForbiddenDataFlags() | context::DataFlags::kForbiddenFlagsMask;

        return !(dataFlags & forbiddenDataFlags) && (dataFlags & effectiveMandatoryDataFlags) == effectiveMandatoryDataFlags
            ? Status::NoError
            : Status::ErrorNotCompatibleDataFlagsSettings;
    }
}

// This function does not check presence of pointer map.
// It is library code responsibility to provide it if it's absent.
template<ISerializableImpl T>
constexpr Status ContextProcessor::serialize(context::SData& ctx)
{
    Id id = T::getId();

    AGS_CS_RUN(writeRawData(&id, 1, ctx));

    constexpr interface_version_t interfaceVersion = T::getInterface().m_version;

    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), T::getOriginPrivateVersion(), interfaceVersion))
        return Status::ErrorNotSupportedInterfaceVersion;

    AGS_CS_RUN(writePrimitive(ctx.getInterfaceVersion(), ctx));

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);
    
    if (ctx.checkRecursivePointers() && !ctx.allowUnmanagedPointers())
        return Status::ErrorNotCompatibleDataFlagsSettings;

    context::DataFlags dataFlags = ctx.getDataFlags();

    AGS_CS_RUN(testDataFlagsCompatibility<T>(dataFlags));

    AGS_CS_RUN(writePrimitive(static_cast<uint32_t>(dataFlags), ctx));

    return Status::NoError;
}

template<ISerializableImpl T>
constexpr Status ContextProcessor::serializeNoChecks(context::SData& ctx)
{
    Id id = T::getId();

    AGS_CS_RUN(writeRawData(&id, 1, ctx));
    AGS_CS_RUN(writePrimitive(ctx.getInterfaceVersion(), ctx));

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    AGS_CS_RUN(writePrimitive(static_cast<uint32_t>(ctx.getDataFlags()), ctx));
    
    return Status::NoError;
}

constexpr Status ContextProcessor::deserializeNoChecks(context::DData& ctx, Id& id) noexcept
{
    AGS_CS_RUN(readRawData(ctx, 1, &id));

    interface_version_t inputInterfaceVersion = 0;
    AGS_CS_RUN(readPrimitive(ctx, inputInterfaceVersion));
    ctx.setInterfaceVersion(inputInterfaceVersion);

    uint32_t intFlags = 0;
    AGS_CS_RUN(readPrimitive(ctx, intFlags));
    context::DataFlags dataFlags(intFlags);
    ctx.setDataFlags(dataFlags);

    return Status::NoError;
}

template<ISerializableImpl T>
constexpr Status ContextProcessor::deserializePostprocessId(const Id& id) noexcept
{
    Id tUuid = T::getId();
    return tUuid == id ? Status::NoError : Status::ErrorMismatchOfStructId;
}

// This function does not check presence of pointer map.
// It is library code responsibility to provide it if it's absent.
template<ISerializableImpl T>
constexpr Status ContextProcessor::deserializePostprocessRest(context::DData& ctx, interface_version_t minimumSupportedInterfaceVersion) noexcept
{
    constexpr interface_version_t interfaceVersion = T::getInterface().m_version;

    // minimumSupportedInterfaceVersion should be getOriginPrivateVersion value by default
    // however for some special cases you may override it by
    // value that is higher than minimum defined in interface version
    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), minimumSupportedInterfaceVersion, interfaceVersion))
        return Status::ErrorNotSupportedInterfaceVersion;
    else if (ctx.getInterfaceVersion() < T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    AGS_CS_RUN(testDataFlagsCompatibility<T>(ctx.getDataFlags()));

    if (ctx.allowUnmanagedPointers() && ctx.getAddedPointers() == nullptr)
        return Status::ErrorInvalidArgument;
    
    if (ctx.checkRecursivePointers() && !ctx.allowUnmanagedPointers())
        return Status::ErrorNotCompatibleDataFlagsSettings;

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
