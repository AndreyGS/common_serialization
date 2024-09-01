/**
 * @file common_serialization/csp_base/service_structs/Structs.h
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

#include <common_serialization/csp_base/ISerializable.h>
#include <common_serialization/csp_messaging/service_structs/Interface.h>
#include <common_serialization/csp_base/processing/data/VersionConverter.h>

namespace common_serialization::csp::messaging::service_structs
{

/// @brief Special type - placeholder for Input-Output operations that have no need in ISerializable Output struct
/// @remark Interfaces which are using that struct as an Output will always receive the Message::Status as result
struct ISerializableDummy final : public ISerializable<ISerializableDummy>
{
public:
    using instance_type = ISerializableDummy;
    using empty_type_tag = std::true_type;

    static constexpr Id kId{ 0x60b0dd3e, 0x7d2e, 0x42d9, 0xb00d, 0x90f7ecd19d25 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const Interface& getInterface() noexcept { return properties; }
};

/// @brief Struct for request of interface properties with specific Id
/// @tparam _T Derived class
template<typename _T = Dummy>
struct GetInterface : public csp::ISerializable<GetCrtpMainType<GetInterface<_T>, _T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterface<_T>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr Id kId{ 0x08c68657, 0x4fa7, 0x4419, 0x8c13, 0x66aec2b06cb0 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const Interface& getInterface() noexcept { return properties; }

    Id m_id;
};

/// @brief Struct for response on GetInterface
/// @tparam _T Derived class
template<typename _T = Dummy>
struct OutGetInterface : public csp::ISerializable<GetCrtpMainType<OutGetInterface<_T>, _T>>
{
public:
    using instance_type = GetCrtpMainType<OutGetInterface<_T>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr Id kId{ 0x22bd67db, 0x65a0, 0x42f4, 0xb28b, 0x63c6181aebe1 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const Interface& getInterface() noexcept { return service_structs::properties; }

    Interface m_properties;
};

#pragma pack(push, 1)

/// @brief Interface Id and Version holder
/// @tparam _T Derived class
template<typename _T = Dummy>
struct InterfaceVersion : public csp::ISerializable<GetCrtpMainType<InterfaceVersion<_T>, _T>>
{
public:
    using instance_type = GetCrtpMainType<InterfaceVersion<_T>, _T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr Id kId{ 0xdf1cb40c, 0x9a72, 0x426b, 0xa801, 0xb0993fe76a46 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const Interface& getInterface() noexcept { return properties; }

    InterfaceVersion() = default;
    InterfaceVersion(const Id& id, interface_version_t version) noexcept : m_id(id), m_version(version) { }
    explicit InterfaceVersion(const Interface& interface_) noexcept : m_id(interface_.m_id), m_version(interface_.m_version) { }

    template<typename _T>
    constexpr [[nodiscard]] bool operator<(const InterfaceVersion<_T>& rhs) const noexcept
    {
        return m_id < rhs.m_id || m_version < rhs.m_version;
    }

    template<typename _T>
    constexpr [[nodiscard]] bool operator==(const InterfaceVersion<_T>& rhs) const noexcept
    {
        return m_id == rhs.m_id && m_version == rhs.m_version;;
    }

    Id m_id{ kNullUuid };
    interface_version_t m_version{ traits::kInterfaceVersionUndefined };
};

#pragma pack(pop)

/// @brief Settings of a CSP party
template<typename _T = Dummy>
class CspPartySettings : public csp::ISerializable<GetCrtpMainType<CspPartySettings<_T>, _T>>
{
public:
    using instance_type = GetCrtpMainType<CspPartySettings<_T>, _T>;

    static constexpr Id kId{ 0xbf8c27e8, 0xfe6a, 0x4492, 0x91cb, 0xe4cf411e1236 };
    static constexpr interface_version_t kInterfaceVersion = 1;
    static constexpr interface_version_t kPrivateVersions[] = { 1 };
    static consteval const Interface& getInterface() noexcept { return properties; }

    CspPartySettings() = default;

    AGS_CS_ALWAYS_INLINE CspPartySettings(const CspPartySettings& rhs)
    {
        init(rhs);
    }

    CspPartySettings& operator=(const CspPartySettings& rhs)
    {
        init(rhs);
        return *this;
    }

    CspPartySettings(CspPartySettings&& rhs) noexcept
    {
        init(std::move(rhs));
    }

    CspPartySettings& operator=(CspPartySettings&& rhs) noexcept
    {
        init(std::move(rhs));
        return *this;
    }

    CspPartySettings(
          const RawVectorT<protocol_version_t>& protocolVersions
        , context::CommonFlags mandatoryCommonFlags
        , context::CommonFlags forbiddenCommonFlags
        , const RawVectorT<InterfaceVersion<>>& interfaces)
    {
        init(protocolVersions, mandatoryCommonFlags, forbiddenCommonFlags, interfaces);
    }

    Status init(const CspPartySettings& rhs) noexcept
    {
        if (this == &rhs)
            return Status::NoError;

        AGS_CS_RUN(m_protocolVersions.init(rhs.m_protocolVersions));
        m_mandatoryCommonFlags = rhs.m_mandatoryCommonFlags;
        m_forbiddenCommonFlags = rhs.m_forbiddenCommonFlags;
        AGS_CS_RUN(m_interfaces.init(rhs.m_interfaces));

        return Status::NoError;
    }

    Status init(CspPartySettings&& rhs) noexcept
    {
        if (this == &rhs)
            return Status::NoError;

        AGS_CS_RUN(m_protocolVersions.init(std::move(rhs.m_protocolVersions)));
        m_mandatoryCommonFlags = rhs.m_mandatoryCommonFlags;
        m_forbiddenCommonFlags = rhs.m_forbiddenCommonFlags;
        AGS_CS_RUN(m_interfaces.init(std::move(rhs.m_interfaces)));

        return Status::NoError;
    }

    Status init(
          const RawVectorT<protocol_version_t>& protocolVersions
        , context::CommonFlags mandatoryCommonFlags
        , context::CommonFlags forbiddenCommonFlags
        , const RawVectorT<InterfaceVersion<>>& interfaces)
    {
        AGS_CS_RUN(m_protocolVersions.init(protocolVersions));
        m_mandatoryCommonFlags = mandatoryCommonFlags;
        m_forbiddenCommonFlags = forbiddenCommonFlags;
        AGS_CS_RUN(m_interfaces.init(interfaces));

        return Status::NoError;
    }

    template<typename _T>
    constexpr [[nodiscard]] bool operator==(const CspPartySettings<_T>& rhs) const noexcept
    {
        return m_protocolVersions == rhs.m_protocolVersions
            && m_mandatoryCommonFlags == rhs.m_mandatoryCommonFlags
            && m_forbiddenCommonFlags == rhs.m_forbiddenCommonFlags
            && m_interfaces == rhs.m_interfaces;
    }

    constexpr bool isValid() const noexcept
    {
        return
               m_protocolVersions.size() > 0
            && !static_cast<bool>(m_mandatoryCommonFlags & m_forbiddenCommonFlags)
            && m_interfaces.size() > 0;
    }

    Status getCompatibleSettings(const CspPartySettings<>& lhs, const CspPartySettings<>& rhs) noexcept
    {
        for (auto lhsVersion : lhs.m_protocolVersions)
            for (auto rhsVersion : rhs.m_protocolVersions)
                if (lhsVersion == rhsVersion)
                {
                    AGS_CS_RUN(m_protocolVersions.pushBack(lhsVersion));
                    break;
                }

        if (m_protocolVersions.size() == 0)
            return Status::ErrorNotSupportedProtocolVersion;

        m_mandatoryCommonFlags = lhs.m_mandatoryCommonFlags | rhs.m_mandatoryCommonFlags;
        m_forbiddenCommonFlags = lhs.m_forbiddenCommonFlags | rhs.m_forbiddenCommonFlags;

        if (m_mandatoryCommonFlags & m_forbiddenCommonFlags)
            return Status::ErrorNotCompatibleCommonFlagsSettings;

        for (const auto& lhsInterface : lhs.m_interfaces)
            for (const auto& rhsInterface : rhs.m_interfaces)
                if (lhsInterface.m_id == rhsInterface.m_id)
                {
                    AGS_CS_RUN(m_interfaces.pushBack({ lhsInterface.m_id, lhsInterface.m_version < rhsInterface.m_version ? lhsInterface.m_version : rhsInterface.m_version }));
                    break;
                }

        if (m_interfaces.size() == 0)
            return Status::ErrorNoSupportedInterfaces;

        return Status::NoError;
    }

    void clear() noexcept
    {
        m_protocolVersions.clear();
        m_mandatoryCommonFlags = context::CommonFlags{};
        m_forbiddenCommonFlags = context::CommonFlags{};
        m_interfaces.clear();
    }

    AGS_CS_ALWAYS_INLINE constexpr const RawVectorT<protocol_version_t>& getProtocolVersions() const
    {
        return m_protocolVersions;
    }

    constexpr protocol_version_t getLatestProtocolVersion() const
    {
        return m_protocolVersions.size() ? m_protocolVersions[0] : traits::kProtocolVersionUndefined;
    }

    constexpr protocol_version_t getOldestProtocolVersion() const
    {
        return m_protocolVersions.size() ? m_protocolVersions[m_protocolVersions.size() - 1] : traits::kProtocolVersionUndefined;
    }

    AGS_CS_ALWAYS_INLINE constexpr context::CommonFlags getMandatoryCommonFlags() const
    {
        return m_mandatoryCommonFlags;
    }

    AGS_CS_ALWAYS_INLINE context::CommonFlags getForbiddenCommonFlags() const
    {
        return m_forbiddenCommonFlags;
    }

    AGS_CS_ALWAYS_INLINE constexpr const RawVectorT<InterfaceVersion<>>& getInterfaces() const
    {
        return m_interfaces;
    }

private:
    /// @brief List of all supported CSP versions in decreasing order
    RawVectorT<protocol_version_t> m_protocolVersions;

    /// @brief Mandatory Common Flags in interactions with party
    context::CommonFlags m_mandatoryCommonFlags;
    /// @brief Forbidden Common Flags on party
    context::CommonFlags m_forbiddenCommonFlags;

    /// @brief List of availible party interfaces and their settings
    RawVectorT<InterfaceVersion<>> m_interfaces;

    friend processing::data::BodyProcessor;
};

} // namespace common_serialization::csp::messaging::service_structs
