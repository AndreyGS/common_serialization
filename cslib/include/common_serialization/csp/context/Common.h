/**
 * @file cslib/include/common_serialization/csp/context/Common.h
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

#include "common_serialization/csp/Traits.h"
#include "common_serialization/csp/context/Message.h"
#include "common_serialization/csp/Concepts.h"
#include "common_serialization/Containers/Concepts.h"

namespace common_serialization::csp::context
{

/// <summary>
/// This is holder of common parameters of serialization process context
/// </summary>
/// <typeparam name="Container">Container capable to serialization/deserialization</typeparam>
template<typename Container>
    requires   ISerializationCapableContainer<Container>
            || IDeserializationCapableContainer<Container>
class Common
{
public:

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="binaryData">Container that hold or would hold binary data of processing</param>
    /// <param name="protocolVersion">Protocol version that would be used in process (can be changed later)</param>
    /// <param name="messageType">Type of message that should be processed (can be changed later)</param>
    constexpr Common(Container& binaryData, protocol_version_t protocolVersion = traits::getLatestProtocolVersion(), Message messageType = Message::kData) noexcept
        : m_binaryData(binaryData), m_protocolVersion(protocolVersion)
        , m_protocolVersionsNotMatch(traits::getLatestProtocolVersion() != protocolVersion), m_messageType(messageType)
    {
        if constexpr (ISerializationCapableContainer<Container>)
            m_binaryData.reserve(256);
    }

    /// <summary>
    /// Get reference to container that holds processed data in binary
    /// </summary>
    /// <returns>Container with binary data</returns>
    [[nodiscard]] constexpr Container& getBinaryData() noexcept { return m_binaryData; }
    [[nodiscard]] constexpr const Container& getBinaryData() const noexcept { return m_binaryData; }

    /// <summary>
    /// Get type of CSP message that holds by this context
    /// </summary>
    /// <returns>Type of CSP message</returns>
    [[nodiscard]] constexpr Message getMessageType() const noexcept { return m_messageType; }

    /// <summary>
    /// Set CSP message type to current context
    /// </summary>
    /// <param name="messageType">Type of CSP message</param>
    constexpr void setMessageType(Message messageType) { m_messageType = messageType; }

    /// <summary>
    /// Get CSP version that is using in this context
    /// </summary>
    /// <returns>CSP version</returns>
    [[nodiscard]] constexpr protocol_version_t getProtocolVersion() const noexcept { return m_protocolVersion; }

    /// <summary>
    /// Set CSP version that would be used by this context
    /// </summary>
    /// <param name="protocolVersion">CSP version</param>
    constexpr void setProtocolVersion(protocol_version_t protocolVersion)
    { 
        m_protocolVersion = protocolVersion; 
        m_protocolVersionsNotMatch = traits::isProtocolVersionSameAsLatestOur(m_protocolVersion);
    }

    /// <summary>
    /// Reset all fields to their default values, but leaves binary data unchanged
    /// </summary>
    virtual void resetToDefaultsExceptDataContents() noexcept
    {
        if constexpr (IDeserializationCapableContainer<Container>)
            m_binaryData.seek(0);
        m_protocolVersion = traits::getLatestProtocolVersion();
        m_messageType = Message::kData;
    }

    /// <summary>
    /// Reset all fields to their default values and clears binary data container
    /// </summary>
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
};

} // namespace common_serialization::csp::context
