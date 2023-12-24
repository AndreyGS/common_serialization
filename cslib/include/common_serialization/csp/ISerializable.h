/**
 * @file cslib/include/common_serialization/csp/ISerializable.h
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

#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataProcessor.h"

namespace common_serialization::csp
{

/// <summary>
/// Common interface for simplifying data serialization process.
/// It implements CTRP pattern as base class.
/// Only classes that inherits this interface are supports versioning
///     and may be used as top structs in csp data messaging.
/// </summary>
/// <typeparam name="T">
/// This is primary class.
/// Must have a few static fields.
///     - Id kId;
///         using as unique id of type (UUID v4)
///     - interface_version_t kInterfaceVersion;
///         latest struct interface version, which computes as 
///         max(private_ver, parent1_interface_ver ... parentn_interface_ver, member1_interface_ver ... membern_interface_ver)
///         updated every time one of versions in 'max()' parameters are changed 
///     - interface_version_t kPrivateVersions[];
///         holds private versions of struct, from highest to lowest
///         every time when in this class or its parent class adds or removes member
///         this array populates with interface version on time of change,
///         but when no member in this class or its parent are added or deleted
///         and there are changes in one or more of its members no action take place here
/// </typeparam>
template<typename T>
class ISerializable
{
public:
    using instance_type = T;

    /// <summary>
    /// It is a shortcut method for serialize this struct.
    /// Use it when no context::DataFlags is need.
    /// </summary>
    /// <typeparam name="S">Container that implements ISerializationCapableContainer interface</typeparam>
    /// <param name="output">Output container</param>
    /// <returns>Status of operation</returns>
    template<ISerializationCapableContainer S>
    constexpr Status serialize(S& output) const noexcept;

    /// <summary>
    /// It is a shortcut method for serialize this struct using custom context.
    /// </summary>
    /// <typeparam name="S">Container that implements ISerializationCapableContainer interface</typeparam>
    /// <typeparam name="PM">Container that implements ISerializationPointersMap interface</typeparam>
    /// <param name="ctx">Context that will be using in serialization process</param>
    /// <returns>Status of operation</returns>
    template<ISerializationCapableContainer S, ISerializationPointersMap PM>
    constexpr Status serialize(context::SData<S, PM>& ctx) const noexcept;

    /// <summary>
    /// It is a shortcut method for deserialize in this struct from input binary data.
    /// </summary>
    /// <typeparam name="D">Container that implements IDeserializationCapableContainer interface</typeparam>
    /// <param name="input">Input container</param>
    /// <returns>Status of operation</returns>
    template<IDeserializationCapableContainer D>
    constexpr Status deserialize(D& input);

    /// <summary>
    /// It is a shortcut method for deserialize in this struct from input binary data using custom context.
    /// </summary>
    /// <typeparam name="D">Container that implements IDeserializationCapableContainer interface</typeparam>
    /// <typeparam name="PM">Container that implements IDeserializationPointersMap interface</typeparam>
    /// <param name="ctx">Context that will be using in deserialization process</param>
    /// <returns>Status of operation</returns>
    template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
    constexpr Status deserialize(context::DData<D, PM>& ctx);

    /// <summary>
    /// Get instance ID
    /// </summary>
    /// <returns>Instance ID</returns>
    [[nodiscard]] static consteval Id getId() noexcept;

    /// <summary>
    /// Get latest private version 
    /// </summary>
    /// <returns>Latest private version</returns>
    [[nodiscard]] static constexpr interface_version_t getLatestPrivateVersion() noexcept;

    /// <summary>
    /// Get latest interface version
    /// </summary>
    /// <returns>Latest interface version</returns>
    [[nodiscard]] static consteval interface_version_t getLatestInterfaceVersion() noexcept;

    /// <summary>
    /// Get origin private version.
    /// This is the interface version that was when class was first time defined.
    /// </summary>
    /// <returns>Origin private version</returns>
    [[nodiscard]] static consteval interface_version_t getOriginPrivateVersion() noexcept;

    /// <summary>
    /// Get private versions array
    /// </summary>
    /// <returns>All private versions</returns>
    [[nodiscard]] static constexpr const interface_version_t* getPrivateVersions() noexcept;

    /// <summary>
    /// Get array size of private versions
    /// </summary>
    /// <returns>Array size of private versions</returns>
    [[nodiscard]] static consteval interface_version_t getPrivateVersionsCount() noexcept;
};

template<typename T>
template<ISerializationCapableContainer S>
constexpr Status ISerializable<T>::serialize(S& output) const noexcept
{
    context::SData<S> ctx(output, context::DataFlags{}, getLatestInterfaceVersion());

    return serialize(ctx);
}

template<typename T>
template<ISerializationCapableContainer S, ISerializationPointersMap PM>
constexpr Status ISerializable<T>::serialize(context::SData<S, PM>& ctx) const noexcept
{
    if (ctx.getInterfaceVersion() == traits::kInterfaceVersionUndefined)
        ctx.setInterfaceVersion(getLatestInterfaceVersion());

    RUN(processing::serializeHeaderContext(ctx));
    RUN(processing::serializeDataContext<T>(ctx));

    return processing::DataProcessor::serializeData(static_cast<const T&>(*this), ctx);
}

template<typename T>
template<IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserialize(D& input)
{
    context::DData<D> ctx(input, context::DataFlags{}, getOriginPrivateVersion());

    return deserialize(ctx);
}

template<typename T>
template<IDeserializationCapableContainer D, IDeserializationPointersMap PM>
constexpr Status ISerializable<T>::deserialize(context::DData<D, PM>& ctx)
{
    RUN(processing::deserializeHeaderContext(ctx));

    Id id;
    uint32_t minimumInterfaceVersion = ctx.getInterfaceVersion() == traits::kInterfaceVersionUndefined ? getOriginPrivateVersion() : ctx.getInterfaceVersion();

    RUN(processing::deserializeDataContext(ctx, id));
    RUN(processing::deserializeDataContextPostprocess<T>(ctx, id, minimumInterfaceVersion));
    
    return processing::DataProcessor::deserializeData(ctx, static_cast<T&>(*this));
}

template<typename T>
[[nodiscard]] consteval Id ISerializable<T>::getId() noexcept
{
    return T::kId;
}

template<typename T>
[[nodiscard]] constexpr interface_version_t ISerializable<T>::getLatestPrivateVersion() noexcept
{
    return T::kPrivateVersions[0];
}

template<typename T>
[[nodiscard]] consteval interface_version_t ISerializable<T>::getLatestInterfaceVersion() noexcept
{
    return T::kInterfaceVersion;
}

template<typename T>
[[nodiscard]] consteval interface_version_t ISerializable<T>::getOriginPrivateVersion() noexcept
{
    return getPrivateVersions()[getPrivateVersionsCount() - 1];
}

template<typename T>
[[nodiscard]] constexpr const interface_version_t* ISerializable<T>::getPrivateVersions() noexcept
{
    return T::kPrivateVersions;
}

template<typename T>
[[nodiscard]] consteval interface_version_t ISerializable<T>::getPrivateVersionsCount() noexcept
{
    return std::size(T::kPrivateVersions);
}

} // namespace common_serialization::csp
