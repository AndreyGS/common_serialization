/**
 * @file cslib/include/common_serialization/csp/context/Common.h
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

#include "common_serialization/csp/Traits.h"
#include "common_serialization/csp/context/Message.h"
#include "common_serialization/csp/context/CommonFlags.h"
#include "common_serialization/csp/Concepts.h"
#include "common_serialization/Containers/Concepts.h"

namespace common_serialization::csp::context
{

/// @brief This is holder of common parameters of serialization process context
/// @tparam Container Container capable to serialization/deserialization
template<typename Container>
    requires   ISerializationCapableContainer<Container>
            || IDeserializationCapableContainer<Container>
class Common
{
public:
    /// @brief Constructor
    /// @param binaryData Container that hold or would hold binary data of processing
    /// @param protocolVersion Protocol version that would be used in process (can be changed later)
    /// @param messageType Type of message that should be processed (can be changed later)
    constexpr Common(Container& binaryData, protocol_version_t protocolVersion = traits::getLatestProtocolVersion()
        , Message messageType = Message::kData, CommonFlags commonFlags = CommonFlags{}
    ) noexcept
        : m_binaryData(binaryData)
        , m_protocolVersion(protocolVersion)
        , m_protocolVersionsNotMatch(traits::getLatestProtocolVersion() != protocolVersion)
        , m_messageType(messageType)
        , m_commonFlags(commonFlags)
    {
        if constexpr (ISerializationCapableContainer<Container>)
            m_binaryData.reserve(256);
    }

    constexpr Common(Common& rhs) noexcept
        : m_binaryData(rhs.m_binaryData)
        , m_protocolVersion(rhs.m_protocolVersion)
        , m_protocolVersionsNotMatch(rhs.m_protocolVersionsNotMatch)
        , m_commonFlags(rhs.m_commonFlags)
        , m_messageType(rhs.m_messageType)
    { }

    /// @brief Get reference to container that holds processed data in binary
    /// @return Container with binary data
    [[nodiscard]] constexpr Container& getBinaryData() noexcept { return m_binaryData; }
    [[nodiscard]] constexpr const Container& getBinaryData() const noexcept { return m_binaryData; }

    /// @brief Get type of CSP message that holds by this context
    /// @return Type of CSP message
    [[nodiscard]] constexpr Message getMessageType() const noexcept { return m_messageType; }

    /// @brief Set CSP message type to current context
    /// @param messageType Type of CSP message
    constexpr void setMessageType(Message messageType) { m_messageType = messageType; }

    /// @brief Get CSP version that is using in this context
    /// @return CSP version
    [[nodiscard]] constexpr protocol_version_t getProtocolVersion() const noexcept { return m_protocolVersion; }

    /// @brief Set CSP version that will be used by this context
    /// @param protocolVersion CSP version
    constexpr void setProtocolVersion(protocol_version_t protocolVersion) noexcept
    { 
        m_protocolVersion = protocolVersion; 
        m_protocolVersionsNotMatch = traits::isProtocolVersionSameAsLatestOur(m_protocolVersion);
    }

    /// @brief Get Common Flags that are using in this context
    /// @return Common Flags
    [[nodiscard]] constexpr CommonFlags getCommonFlags() const noexcept { return m_commonFlags; }

    /// @brief Set Common Flags that will be used by this context
    /// @param commonFlags Common Flags
    constexpr void setCommonFlags(CommonFlags commonFlags) noexcept { m_commonFlags = commonFlags; }

    /// @brief Reset all fields to their default values, but leaves binary data and common flags unchanged
    /// @note Common flags are not resets to false because because they are 
    ///     rather environment tool option instead of struct/operation specific.
    virtual void resetToDefaultsExceptDataContents() noexcept
    {
        if constexpr (IDeserializationCapableContainer<Container>)
            m_binaryData.seek(0);
        m_protocolVersion = traits::getLatestProtocolVersion();
        m_messageType = Message::kData;
    }

    /// @brief Reset all fields to their default values and clears binary data container
    /// @note Common flags are not resets to false because because they are 
    ///     rather environment tool option instead of struct/operation specific.
    virtual void clear() noexcept
    {
        resetToDefaultsExceptDataContents();
        m_binaryData.clear();
    }

private:
    Container& m_binaryData;
    protocol_version_t m_protocolVersion{ traits::getLatestProtocolVersion() };
    bool m_protocolVersionsNotMatch = false;
    Message m_messageType = Message::kData;
    CommonFlags m_commonFlags;
};

} // namespace common_serialization::csp::context
