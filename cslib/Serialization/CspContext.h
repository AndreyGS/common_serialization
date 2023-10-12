/**
 * @file CspContext.h
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

#include "CspTraits.h"
#include "CspFlags.h"
#include "CspMessageType.h"
#include "SerializationConcepts.h"
#include "../Containers/HashMap.h"

namespace common_serialization
{

class CspContextHeader
{
public:
    constexpr CspContextHeader() noexcept {}
    constexpr CspContextHeader(CspFlags flags, CspMessageType messageType, uint8_t protocolVersion) noexcept
        : m_flags(flags), m_messageType(messageType), m_protocolVersion(protocolVersion)
    { }

    constexpr CspFlags getFlags() const noexcept { return m_flags; }
    constexpr void setFlags(CspFlags flags) { m_flags = flags; }

    constexpr CspMessageType getMessageType() const noexcept { return m_messageType; }
    constexpr void setMessageType(CspMessageType messageType) { m_messageType = messageType; }

    constexpr uint8_t getProtocolVersion() const noexcept { return m_protocolVersion; }
    constexpr void setProtocolVersion(uint8_t protocolVersion) { m_protocolVersion = protocolVersion; }

private:
    CspFlags m_flags;
    CspMessageType m_messageType = CspMessageType::kData;
    uint8_t m_protocolVersion{ CspTraits::getLatestProtocolVersion() };
};

template<serialization_concepts::IPointersMap PM = HashMap>
class CspContextDataExtension : public CspContextHeader
{
public:
    constexpr CspContextDataExtension() {}

    constexpr CspContextDataExtension(const CspContextDataExtension<PM>& rhs) noexcept
        : CspContextHeader(rhs.getFlags(), CspMessageType::kData, rhs.getProtocolVersion())
        , m_interfaceVersion(rhs.m_interfaceVersion), m_pPointersMap(rhs.m_pPointersMap)
    { }

    constexpr CspContextDataExtension(CspFlags flags, uint8_t protocolVersion, uint32_t interfaceVersion, PM* pPointersMap = nullptr) noexcept
        : CspContextHeader(flags, CspMessageType::kData, protocolVersion), m_interfaceVersion(interfaceVersion), m_pPointersMap(pPointersMap)
    { }

    constexpr explicit operator bool() const noexcept
    {
        return bool(getFlags());
    }

    constexpr uint32_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(uint32_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }

private:
    uint32_t m_interfaceVersion{ CspTraits::kInterfaceVersionMax };
    PM* m_pPointersMap{ nullptr };
};

template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM = HashMap>
class CspContextSerializeData
{
public:
    constexpr explicit CspContextSerializeData(S& output) noexcept
        : m_output(output)
    { }

    constexpr CspContextSerializeData(S& output, const CspContextDataExtension<PM>& extension) noexcept
        : m_output(output), m_extension(extension)
    { }

    constexpr CspContextSerializeData(S& output, CspFlags flags, uint8_t protocolVersion, uint32_t interfaceVersion, PM* pPointersMap = nullptr) noexcept
        : m_output(output), m_extension(flags, protocolVersion, interfaceVersion, pPointersMap)
    { }

    constexpr S& getOutput() noexcept { return m_output; }
    constexpr const S& getOutput() const noexcept { return m_output; }

    constexpr CspContextDataExtension<PM>& getExtension() noexcept { return m_extension; }
    constexpr const CspContextDataExtension<PM>& getExtension() const noexcept { return m_extension; }

private:
    S& m_output;
    CspContextDataExtension<PM> m_extension;
};

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM = HashMap>
class CspContextDeserializeData
{
public:
    constexpr explicit CspContextDeserializeData(D& input) noexcept
        : m_input(input)
    { }

    constexpr CspContextDeserializeData(D& input, const CspContextDataExtension<PM>& extension)
        : m_input(input), m_extension(extension)
    { }

    constexpr CspContextDeserializeData(D& input, CspFlags flags, uint8_t protocolVersion, uint32_t interfaceVersion, PM* pPointersMap = nullptr) noexcept
        : m_input(input), m_extension(flags, protocolVersion, interfaceVersion, pPointersMap)
    { }

    constexpr D& getInput() noexcept { return m_input; }
    constexpr const D& getInput() const noexcept { return m_input; }

    constexpr CspContextDataExtension<PM>& getExtension() noexcept { return m_extension; }
    constexpr const CspContextDataExtension<PM>& getExtension() const noexcept { return m_extension; }

private:
    D& m_input;
    CspContextDataExtension<PM> m_extension;
};

} // namespace common_serialization
