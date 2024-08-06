/**
 * @file cslib/include/common_serialization/CspBase/ISerializable.h
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

#include <common_serialization/CspBase/processing/common/ContextProcessor.h>
#include <common_serialization/CspBase/processing/data/BodyProcessor.h>
#include <common_serialization/CspBase/processing/data/ContextProcessor.h>

namespace common_serialization::csp
{

template<typename T>
concept HasAddtionalMandatoryDataFlags = requires(T t)
{
    { T::kAddtionalMandatoryDataFlags } -> std::same_as<const context::DataFlags&>;
};

template<typename T>
concept HasAddtionalForbiddenDataFlags = requires(T t)
{
    { T::kAddtionalForbiddenDataFlags } -> std::same_as<const context::DataFlags&>;
};

/// @brief Common interface for simplifying data serialization process.
/// @note Only classes that inherits this interface are supports versioning
///     and may be used as top structs in CSP data messaging.
/// @remark Implements CTRP pattern as base class.
/// 
/// @tparam T This is most derived class.
///     Must have a few static fields:
///     - Id kId;
///         using as unique id of type (UUID v4)
///     - interface_version_t kInterfaceVersion;
///         latest struct interface version, which computes as 
///         max(private_ver, parent1_interface_ver ... parentn_interface_ver, member1_interface_ver ... membern_interface_ver)
///         updated every time when one of versions in 'max()' parameters are changed 
///     - interface_version_t kPrivateVersions[];
///         holds private versions of struct, from highest to lowest
///         every time when in this class or its parent class adds or removes member
///         copy of defenition of T class is placed to legacy structs header
///         and its name is appended by _Version{kInterfaceVersion}
///         meanwhile here this array populates with interface version on time of change,
///         and kInterfaceVersion is also updated;
///         but when no member in this class or its parent are added or deleted
///         and there are changes in one or more of its members no action take place here
template<typename T>
class ISerializable
{
public:
    using instance_type = T;

    /// @brief It is a shortcut method for serialize this struct.
    /// @remark Use it when no context::DataFlags is need.
    /// @param output Output container
    /// @return Status of operation
    CS_ALWAYS_INLINE constexpr Status serialize(BinVectorT& output) const;
    
    /// @brief It is a shortcut method for serialize this struct using custom context
    /// @param ctx Context that will be using in serialization process
    /// @return Status of operation
    constexpr Status serialize(context::SData& ctx) const;

    /// @brief It is a shortcut method for deserialize in this struct from input binary data
    /// @param input Input container
    /// @return Status of operation
    CS_ALWAYS_INLINE constexpr Status deserialize(BinWalkerT& input);

    /// @brief It is a shortcut method for deserialize in this struct from input binary data using custom context
    /// @param ctx Context that will be using in deserialization process
    /// @return Status of operation
    constexpr Status deserialize(context::DData& ctx);

    /// @brief Get instance ID
    /// @return Instance ID
    [[nodiscard]] static consteval Id getId() noexcept;

    /// @brief Get latest private version
    /// @return Latest private version
    [[nodiscard]] static consteval interface_version_t getLatestPrivateVersion() noexcept;

    /// @brief Get latest interface version
    /// @return Latest interface version
    [[nodiscard]] static consteval interface_version_t getLatestInterfaceVersion() noexcept;

    /// @brief Get origin private version.
    ///     This is the interface version that was when class was first time defined.
    /// @return Origin private version
    [[nodiscard]] static consteval interface_version_t getOriginPrivateVersion() noexcept;

    /// @brief Get private versions array
    /// @return All private versions
   [[nodiscard]] CS_ALWAYS_INLINE static constexpr const interface_version_t* getPrivateVersions() noexcept;

    /// @brief Get array size of private versions
    /// @return Array size of private versions
    [[nodiscard]] static consteval interface_version_t getPrivateVersionsCount() noexcept;

    /// @brief Get properties of interface to which current struct beholds
    /// @return Interface properties
    [[nodiscard]] static consteval const Interface& getInterface() noexcept;

    /// @brief Get additional (to interace defined) mandatory data flags with which
    ///     current struct must be serialized
    /// @return Addtional mandatory DataFlags
    [[nodiscard]] static consteval context::DataFlags getAddtionalMandatoryDataFlags() noexcept;

    /// @brief Get additional (to interace defined) forbidden data flags with which
    ///     current struct must be serialized
    /// @return Addtional forbidden DataFlags
    [[nodiscard]] static consteval context::DataFlags getAddtionalForbiddenDataFlags() noexcept;

    /// @brief Get effective (with interace defined) mandatory data flags with which
    ///     current struct must be serialized
    /// @return Effective mandatory DataFlags
    [[nodiscard]] static consteval context::DataFlags getEffectiveMandatoryDataFlags() noexcept;

    /// @brief Get effective (with interace defined) forbidden data flags with which
    ///     current struct must be serialized
    /// @return AddtioEffectivenal forbidden DataFlags
    [[nodiscard]] static consteval context::DataFlags getEffectiveForbiddenDataFlags() noexcept;

    /// @brief Default comparison operator to allow classes 
    ///     that implements ISerializable use it freely
    [[nodiscard]] constexpr auto operator<=>(const ISerializable&) const = default;
};

template<typename T>
CS_ALWAYS_INLINE constexpr Status ISerializable<T>::serialize(BinVectorT& output) const
{
    context::SData ctx{ output, {}, {}, true, this->getLatestInterfaceVersion() };

    return serialize(ctx);
}

template<typename T>
constexpr Status ISerializable<T>::serialize(context::SData& ctx) const
{
    if (ctx.getInterfaceVersion() == traits::kInterfaceVersionUndefined)
        ctx.setInterfaceVersion(this->getLatestInterfaceVersion());

    CS_RUN(processing::common::ContextProcessor::serialize(ctx));
    CS_RUN(processing::data::ContextProcessor::serialize<T>(ctx));

    // If user is not supplied pointers map we need to use temporary one
    if (ctx.checkRecursivePointers() && !ctx.getPointersMap())
    {
        context::SPointersMap pointersMap;
        Status status = processing::data::BodyProcessor::serialize(static_cast<const T&>(*this), ctx.setPointersMap(&pointersMap));
        ctx.setPointersMap(nullptr);
        return status;
    }
    else
        return processing::data::BodyProcessor::serialize(static_cast<const T&>(*this), ctx);
}

template<typename T>
CS_ALWAYS_INLINE constexpr Status ISerializable<T>::deserialize(BinWalkerT& input)
{
    context::DData ctx{ input, {}, {}, true, this->getOriginPrivateVersion() };
    return deserialize(ctx);
}

template<typename T>
constexpr Status ISerializable<T>::deserialize(context::DData& ctx)
{
    CS_RUN(processing::common::ContextProcessor::deserialize(ctx));

    Id id;
    uint32_t minimumInterfaceVersion = ctx.getInterfaceVersion() == traits::kInterfaceVersionUndefined ? this->getOriginPrivateVersion() : ctx.getInterfaceVersion();

    CS_RUN(processing::data::ContextProcessor::deserializeNoChecks(ctx, id));
    CS_RUN(processing::data::ContextProcessor::deserializePostprocessId<T>(id));
    CS_RUN(processing::data::ContextProcessor::deserializePostprocessRest<T>(ctx, minimumInterfaceVersion));
    
    // If user is not supplied pointers map we need to use temporary one
    if (ctx.checkRecursivePointers() && !ctx.getPointersMap())
    {
        context::DPointersMap pointersMap;
        Status status = processing::data::BodyProcessor::deserialize(ctx.setPointersMap(&pointersMap), static_cast<T&>(*this));
        ctx.setPointersMap(nullptr);
        return status;
    }
    else
        return processing::data::BodyProcessor::deserialize(ctx, static_cast<T&>(*this));
}

template<typename T>
[[nodiscard]] consteval Id ISerializable<T>::getId() noexcept
{
    return T::kId;
}

template<typename T>
[[nodiscard]] consteval interface_version_t ISerializable<T>::getLatestPrivateVersion() noexcept
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
[[nodiscard]] CS_ALWAYS_INLINE constexpr const interface_version_t* ISerializable<T>::getPrivateVersions() noexcept
{
    return T::kPrivateVersions;
}

template<typename T>
[[nodiscard]] consteval interface_version_t ISerializable<T>::getPrivateVersionsCount() noexcept
{
    return std::size(T::kPrivateVersions);
}

template<typename T>
[[nodiscard]] consteval const Interface& ISerializable<T>::getInterface() noexcept
{
    return T::getInterface();
}

template<typename T>
[[nodiscard]] consteval context::DataFlags ISerializable<T>::getAddtionalMandatoryDataFlags() noexcept
{
    if constexpr (HasAddtionalMandatoryDataFlags<T>)
        return T::kAddtionalMandatoryDataFlags;
    else
        return context::DataFlags{};
}

template<typename T>
[[nodiscard]] consteval context::DataFlags ISerializable<T>::getAddtionalForbiddenDataFlags() noexcept
{
    if constexpr (HasAddtionalForbiddenDataFlags<T>)
        return T::kAddtionalForbiddenDataFlags;
    else
        return context::DataFlags{};
}

template<typename T>
[[nodiscard]] consteval context::DataFlags ISerializable<T>::getEffectiveMandatoryDataFlags() noexcept
{
    return getAddtionalMandatoryDataFlags() | getInterface().mandatoryDataFlags;
}

template<typename T>
[[nodiscard]] consteval context::DataFlags ISerializable<T>::getEffectiveForbiddenDataFlags() noexcept
{
    return getAddtionalForbiddenDataFlags() | getInterface().forbiddenDataFlags;
}

/// @brief Shortcut to get type interface version in template contexts
/// @tparam T Type for which interface version is requested
/// @return Latest version of interface if T implements ISerializable and 0 otherwise
template<typename T>
consteval interface_version_t getLatestInterfaceVersion()
{
    if constexpr (ISerializableImpl<T>)
        return T::getLatestInterfaceVersion();
    else
        return 0;
}

} // namespace common_serialization::csp
