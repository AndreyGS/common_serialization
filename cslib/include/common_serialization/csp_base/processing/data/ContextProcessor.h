/**
 * @file cslib/include/common_serialization/csp_base/processing/data/ContextProcessor.h
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

#include <common_serialization/csp_base/Concepts.h>
#include <common_serialization/csp_base/context/Data.h>
#include <common_serialization/csp_base/processing/Rw.h>

namespace common_serialization::csp::processing::data
{

class ContextProcessor
{
public:
    template<typename _T>
    static CS_ALWAYS_INLINE constexpr [[nodiscard]] Status testDataFlagsCompatibility(context::DataFlags dataFlags);

    // This function does not check presence of pointer map.
    // It is library code responsibility to provide it if it is absent.
    template<ISerializableImpl _T>
    static constexpr Status serialize(context::SData& ctx);
    template<ISerializableImpl _T>
    static constexpr Status serializeNoChecks(context::SData& ctx);

    static constexpr Status deserializeNoChecks(context::DData& ctx, Id& id) noexcept;
    template<ISerializableImpl _T>
    static CS_ALWAYS_INLINE constexpr Status deserializePostprocessId(const Id& id) noexcept;
    template<ISerializableImpl _T>
    static constexpr Status deserializePostprocessRest(context::DData& ctx, interface_version_t minimumSupportedInterfaceVersion) noexcept;
};

template<typename _T>
constexpr [[nodiscard]] Status ContextProcessor::testDataFlagsCompatibility(context::DataFlags dataFlags)
{
    if constexpr (!StructHaveDataFlags<_T>)
        return Status::NoError;
    else
    {
        constexpr context::DataFlags effectiveMandatoryDataFlags = _T::getEffectiveMandatoryDataFlags();
        constexpr context::DataFlags forbiddenDataFlags = _T::getEffectiveForbiddenDataFlags() | context::DataFlags::kForbiddenFlagsMask;

        return !(dataFlags & forbiddenDataFlags) && (dataFlags & effectiveMandatoryDataFlags) == effectiveMandatoryDataFlags
            ? Status::NoError
            : Status::ErrorNotCompatibleDataFlagsSettings;
    }
}

// This function does not check presence of pointer map.
// It is library code responsibility to provide it if it's absent.
template<ISerializableImpl _T>
constexpr Status ContextProcessor::serialize(context::SData& ctx)
{
    Id id = _T::getId();

    CS_RUN(writeRawData(&id, 1, ctx));

    constexpr interface_version_t interfaceVersion = _T::getInterface().version;

    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), _T::getOriginPrivateVersion(), interfaceVersion))
        return Status::ErrorNotSupportedInterfaceVersion;

    CS_RUN(writePrimitive(ctx.getInterfaceVersion(), ctx));

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < _T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);
    
    if (ctx.checkRecursivePointers() && !ctx.allowUnmanagedPointers())
        return Status::ErrorNotCompatibleDataFlagsSettings;

    context::DataFlags dataFlags = ctx.getDataFlags();

    CS_RUN(testDataFlagsCompatibility<_T>(dataFlags));

    CS_RUN(writePrimitive(static_cast<uint32_t>(dataFlags), ctx));

    return Status::NoError;
}

template<ISerializableImpl _T>
constexpr Status ContextProcessor::serializeNoChecks(context::SData& ctx)
{
    Id id = _T::getId();

    CS_RUN(writeRawData(&id, 1, ctx));
    CS_RUN(writePrimitive(ctx.getInterfaceVersion(), ctx));

    // Only set interface versions not match when some conversion will be need
    if (ctx.getInterfaceVersion() < _T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    CS_RUN(writePrimitive(static_cast<uint32_t>(ctx.getDataFlags()), ctx));
    
    return Status::NoError;
}

constexpr Status ContextProcessor::deserializeNoChecks(context::DData& ctx, Id& id) noexcept
{
    CS_RUN(readRawData(ctx, 1, &id));

    interface_version_t inputInterfaceVersion = 0;
    CS_RUN(readPrimitive(ctx, inputInterfaceVersion));
    ctx.setInterfaceVersion(inputInterfaceVersion);

    uint32_t intFlags = 0;
    CS_RUN(readPrimitive(ctx, intFlags));
    context::DataFlags dataFlags(intFlags);
    ctx.setDataFlags(dataFlags);

    return Status::NoError;
}

template<ISerializableImpl _T>
constexpr Status ContextProcessor::deserializePostprocessId(const Id& id) noexcept
{
    Id tUuid = _T::getId();
    return tUuid == id ? Status::NoError : Status::ErrorMismatchOfStructId;
}

// This function does not check presence of pointer map.
// It is library code responsibility to provide it if it's absent.
template<ISerializableImpl _T>
constexpr Status ContextProcessor::deserializePostprocessRest(context::DData& ctx, interface_version_t minimumSupportedInterfaceVersion) noexcept
{
    constexpr interface_version_t interfaceVersion = _T::getInterface().version;

    // minimumSupportedInterfaceVersion should be getOriginPrivateVersion value by default
    // however for some special cases you may override it by
    // value that is higher than minimum defined in interface version
    if (!traits::isInterfaceVersionSupported(ctx.getInterfaceVersion(), minimumSupportedInterfaceVersion, interfaceVersion))
        return Status::ErrorNotSupportedInterfaceVersion;
    else if (ctx.getInterfaceVersion() < _T::getLatestInterfaceVersion())
        ctx.setInterfaceVersionsNotMatch(true);

    CS_RUN(testDataFlagsCompatibility<_T>(ctx.getDataFlags()));

    if (ctx.allowUnmanagedPointers() && ctx.getAddedPointers() == nullptr)
        return Status::ErrorInvalidArgument;
    
    if (ctx.checkRecursivePointers() && !ctx.allowUnmanagedPointers())
        return Status::ErrorNotCompatibleDataFlagsSettings;

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
