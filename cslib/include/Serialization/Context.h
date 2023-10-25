/**
 * @file Context.h
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

#include "Traits.h"
#include "ContextFlags.h"
#include "ContextMessage.h"
#include "SerializationConcepts.h"
#include "../Containers/HashMap.h"

namespace common_serialization
{

namespace csp
{

namespace context
{

template<typename Container>
    requires   serialization_concepts::ISerializationCapableContainer<Container>
            || serialization_concepts::IDeserializationCapableContainer<Container>
class Common
{
protected:
    constexpr Common(Container& binaryData, uint8_t protocolVersion, Flags flags, Message messageType) noexcept
        : m_binaryData(binaryData), m_protocolVersion(protocolVersion), m_flags(flags), m_messageType(messageType)
    {
        if constexpr (serialization_concepts::ISerializationCapableContainer<Container>)
            m_binaryData.reserve(256);
    }

public:
    [[nodiscard]] constexpr Container& getBinaryData() noexcept { return m_binaryData; }
    [[nodiscard]] constexpr const Container& getBinaryData() const noexcept { return m_binaryData; }

    [[nodiscard]] constexpr Flags getFlags() noexcept { return m_flags; }
    constexpr void setFlags(Flags flags) { m_flags = flags; }

    [[nodiscard]] constexpr Message getMessageType() const noexcept { return m_messageType; }
    constexpr void setMessageType(Message messageType) { m_messageType = messageType; }

    [[nodiscard]] constexpr uint8_t getProtocolVersion() const noexcept { return m_protocolVersion; }
    constexpr void setProtocolVersion(uint8_t protocolVersion) { m_protocolVersion = protocolVersion; }

    void resetToDefaultsExceptDataContents() noexcept
    {
        if constexpr (serialization_concepts::IDeserializationCapableContainer<Container>)
            m_binaryData.seek(0);
        m_protocolVersion = traits::getLatestProtocolVersion();
        m_flags = Flags{};
        m_messageType = Message::kData;
    }

    void clear() noexcept
    {
        resetToDefaultsExceptDataContents();
        m_binaryData.clear();
    }

private:
    Container& m_binaryData;
    uint8_t m_protocolVersion{ traits::getLatestProtocolVersion() };
    Flags m_flags = Flags{};
    Message m_messageType = Message::kData;
};

// unordered_map shall be replaced by common_serialization::HashMap when it would be ready
template<typename Container, bool serialize = true, serialization_concepts::IPointersMap PM 
        = std::conditional_t<serialize, std::unordered_map<const void*, uint64_t>, std::unordered_map<uint64_t, void*>>>
    requires   serialization_concepts::ISerializationCapableContainer<Container>
            || serialization_concepts::IDeserializationCapableContainer<Container>
class Data : public Common<Container>
{
public:
    constexpr Data(Container& container) noexcept
        : Common<Container>(container, traits::getLatestProtocolVersion(), Flags{}, Message::kData)
    { }

    constexpr Data(const Common<Container>& common) noexcept
        : Common<Container>(common.getBinaryData(), common.getProtocolVersion(), common.getFlags(), Message::kData)
    { }

    constexpr Data(const Data<Container, serialize, PM>& rhs) noexcept
        : Common<Container>(rhs.getBinaryData(), rhs.getProtocolVersion(), rhs.getFlags(), Message::kData)
        , m_interfaceVersion(rhs.m_interfaceVersion), m_pPointersMap(rhs.m_pPointersMap)
    { }

    constexpr Data(Container& binaryData, uint8_t protocolVersion, Flags flags, uint32_t interfaceVersion, PM* pPointersMap = nullptr) noexcept
        : Common<Container>(binaryData, protocolVersion, flags, Message::kData), m_interfaceVersion(interfaceVersion), m_pPointersMap(pPointersMap)
    { }

    [[nodiscard]] constexpr uint32_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(uint32_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }
    constexpr void setPointersMap(PM& pointersMap) noexcept { m_pPointersMap = &pointersMap; }

    void resetToDefaultsExceptDataContents() noexcept
    {
        Common<Container>::resetToDefaultsExceptDataContents();
        m_interfaceVersion = traits::kInterfaceVersionMax;
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

    void clear() noexcept
    {
        Common<Container>::clear();
        m_interfaceVersion = traits::kInterfaceVersionMax;
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    uint32_t m_interfaceVersion{ traits::kInterfaceVersionMax };
    PM* m_pPointersMap{ nullptr };
};

template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SData = Data<S, true, PM>;

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DData = Data<D, false, PM>;

} // namespace context

} // namespace csp

} // namespace common_serialization
