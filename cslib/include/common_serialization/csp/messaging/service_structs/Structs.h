/**
 * @file cslib/include/common_serialization/csp/messaging/service_structs/Structs.h
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

#include "common_serialization/csp/ISerializable.h"

// Service structs not support version conversion and must always be the same
// Set is completely depend on protocol version

namespace common_serialization::csp::messaging::service_structs
{

constexpr traits::Interface properties(Uuid{ 0xe47be322, 0x232e, 0x4d66, 0x9175, 0x06eed2110b4b }, 1);

/// @brief Special type - placeholder for Input-Output operations that have no need in ISerializable Output struct
/// @remark Interfaces which are using that struct as an Output will always receive the Message::kStatus as result
/// @tparam T Derived class
template<typename T = Dummy>
struct ISerializableDummy : public ISerializable<ISerializableDummy<Dummy>>
{
public:
    using empty_type_tag = std::true_type;

    static constexpr Id kId{ 0x60b0dd3e, 0x7d2e, 0x42d9, 0xb00d, 0x90f7ecd19d25 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }
};

/// @brief Struct for request of interface properties with specific Id
/// @tparam T Derived class
template<typename T = Dummy>
struct GetInterface : public csp::ISerializable<GetCrtpMainType<GetInterface<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterface<T>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr Id kId{ 0x08c68657, 0x4fa7, 0x4419, 0x8c13, 0x66aec2b06cb0 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }

    Id id;
};

/// @brief Struct for response on GetInterface
/// @tparam T Derived class
template<typename T = Dummy>
struct OutGetInterface : public csp::ISerializable<GetCrtpMainType<OutGetInterface<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<OutGetInterface<T>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr Id kId{ 0x22bd67db, 0x65a0, 0x42f4, 0xb28b, 0x63c6181aebe1 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }

    traits::Interface properties;
};

/// @brief Struct for request of all supported interfaces list
/// @tparam T Derived class
template<typename T = Dummy>
struct GetInterfacesList : public csp::ISerializable<GetCrtpMainType<GetInterfacesList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfacesList<T>, T>;
    using empty_type_tag = std::true_type;

    static constexpr Id kId{ 0xd5b61816, 0xcc92, 0x4e18, 0xb287, 0xfcc4ed1e9f2a };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }
};

/// @brief Struct contain list of all supported interfaces
/// @tparam T Derived class
template<typename T = Dummy>
struct InterfacesList : public csp::ISerializable<GetCrtpMainType<InterfacesList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfacesList<T>, T>;

    static constexpr Id kId{ 0xb5346ff9, 0xe557, 0x40af, 0x8fb9, 0x5563fa97700e };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }

    bool operator==(const InterfacesList& rhs) const noexcept
    {
        return list == rhs.list;
    }

    Vector<traits::Interface> list;
};


/// @brief Struct contain list of all supported interfaces
/// @tparam T Derived class
template<typename T = Dummy>
struct InterfaceVersion : public csp::ISerializable<GetCrtpMainType<InterfaceVersion<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfaceVersion<T>, T>;

    static constexpr Id kId{ 0xdf1cb40c, 0x9a72, 0x426b, 0xa801, 0xb0993fe76a46 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }

    bool operator==(const InterfacesList& rhs) const noexcept
    {
        return list == rhs.list;
    }

    Id id{ kNullUuid };
    interface_version_t version{ traits::kInterfaceVersionUndefined };
};

/// @brief Settings of a CSP party
template<typename T = Dummy>
struct CspPartySettings : public csp::ISerializable<GetCrtpMainType<CspPartySettings<T>, T>>
{
    using instance_type = GetCrtpMainType<CspPartySettings<T>, T>;

    static constexpr Id kId{ 0xbf8c27e8, 0xfe6a, 0x4492, 0x91cb, 0xe4cf411e1236 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const traits::Interface& getInterfaceProperties() noexcept { return properties; }

    /// @brief List of all supported CSP versions begining in decreasing order
    Vector<protocol_version_t> supportedCspVersions;

    /// @brief Mandatory Common Flags in interactions with party
    context::CommonFlags mandatoryCommonFlags{ helpers::isModuleIsBigEndian() };
    /// @brief Forbidden Common Flags on party
    context::CommonFlags forbiddenCommonFlags;

    /// @brief List of availible party interfaces and their settings
    Vector<InterfaceVersion<>> interfaces;

    Status init(const CspPartySettings& rhs) noexcept
    {
        if (this == &rhs)
            return Status::kNoError;

        clear();

        RUN(supportedCspVersions.init(rhs.supportedCspVersions));
        mandatoryCommonFlags = rhs.mandatoryCommonFlags;
        forbiddenCommonFlags = rhs.forbiddenCommonFlags;
        RUN(interfaces.init(rhs.interfaces));

        return Status::kNoError;
    }

    Status init(CspPartySettings&& rhs) noexcept
    {
        if (this == &rhs)
            return Status::kNoError;

        clear();

        RUN(supportedCspVersions.init(std::move(rhs.supportedCspVersions)));
        mandatoryCommonFlags = rhs.mandatoryCommonFlags;
        forbiddenCommonFlags = rhs.forbiddenCommonFlags;
        RUN(interfaces.init(std::move(rhs.interfaces)));

        return Status::kNoError;
    }

    constexpr bool isValid() const noexcept
    {
        return
               supportedCspVersions.size() > 0
            && !static_cast<bool>(mandatoryCommonFlags & forbiddenCommonFlags)
            && interfaces.size() > 0;
    }

    static Status getCompatibleSettings(const CspPartySettings<>& lhs, const CspPartySettings<>& rhs, CspPartySettings<>& output) noexcept
    {
        for (auto lhsVersion : lhs.supportedCspVersions)
            for (auto rhsVersion : rhs.supportedCspVersions)
                if (lhsVersion == rhsVersion)
                {
                    RUN(output.supportedCspVersions.pushBack(lhsVersion));
                    break;
                }

        output.mandatoryCommonFlags = lhs.mandatoryCommonFlags | rhs.mandatoryCommonFlags;
        output.forbiddenCommonFlags = lhs.forbiddenCommonFlags | rhs.forbiddenCommonFlags;

        for (const auto& lhsInterface : lhs.interfaces)
            for (const auto& rhsInterface : rhs.interfaces)
                if (lhsInterface.id == rhsInterface.id)
                {
                    RUN(output.interfaces.pushBack(lhsInterface.version < rhsInterface.version ? lhsInterface.version : rhsInterface.version));
                    break;
                }
    }

    void clear() noexcept
    {
        supportedCspVersions.clear();
        mandatoryCommonFlags = context::CommonFlags{};
        forbiddenCommonFlags = context::CommonFlags{};
        interfaces.clear();
    }
};

} // namespace common_serialization::csp::messaging::service_structs
