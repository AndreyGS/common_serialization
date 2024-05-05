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
    explicit constexpr Common(Container& binaryData, protocol_version_t protocolVersion = traits::getLatestProtocolVersion()
        , Message messageType = Message::Data, CommonFlags commonFlags = {}
    ) noexcept
        : m_binaryData(binaryData)
        , m_protocolVersion(protocolVersion)
        , m_protocolVersionsNotMatch(traits::getLatestProtocolVersion() != protocolVersion)
        , m_messageType(messageType)
    {
        setCommonFlags(commonFlags);
        m_endiannessNotMatch = bigEndianFormat() != helpers::isBigEndianPlatform();

        if constexpr (ISerializationCapableContainer<Container>)
            m_binaryData.reserve(256);
    }

    virtual ~Common() {}

    /// @brief Get reference to container that holds processed data in binary
    /// @return Container with binary data
    [[nodiscard]] CS_ALWAYS_INLINE constexpr Container& getBinaryData() noexcept { return m_binaryData; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr const Container& getBinaryData() const noexcept { return m_binaryData; }

    /// @brief Get type of CSP message that holds by this context
    /// @return Type of CSP message
    [[nodiscard]] CS_ALWAYS_INLINE constexpr Message getMessageType() const noexcept { return m_messageType; }

    /// @brief Set CSP message type to current context
    /// @param messageType Type of CSP message
    CS_ALWAYS_INLINE constexpr Common& setMessageType(Message messageType) noexcept { m_messageType = messageType; return *this; }

    /// @brief Get CSP version that is using in this context
    /// @return CSP version
    [[nodiscard]] CS_ALWAYS_INLINE constexpr protocol_version_t getProtocolVersion() const noexcept { return m_protocolVersion; }

    /// @brief Set CSP version that will be used by this context
    /// @param protocolVersion CSP version
    CS_ALWAYS_INLINE constexpr Common& setProtocolVersion(protocol_version_t protocolVersion) noexcept
    { 
        m_protocolVersion = protocolVersion; 
        m_protocolVersionsNotMatch = traits::isProtocolVersionSameAsLatestOur(m_protocolVersion);
        return *this;
    }

    /// @brief Is target protocol version same as the latest our
    /// @return Flag indicating that protocol versions not match
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool isProtocolVersionsNotMatch() const noexcept { return m_protocolVersionsNotMatch; }

    /// @brief Is target endianness differs from current platform
    /// @note "Endianness not match" is not the same as "endiann difference". The first one is simply
    ///     precalculated value to determining the need of primitives bytes order change, and the last one
    ///     is the mode that drops off most of optimizations in processing so that there is an opportunity
    ///     to deserialize binary data on platform with any endianness.
    /// @return Flag indicating that endianness of current platform differs from target one
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool isEndiannessNotMatch() const noexcept { return m_endiannessNotMatch; }

    /// @brief Get Common Flags that are using in this context
    /// @return Common Flags
    [[nodiscard]] constexpr CommonFlags getCommonFlags() const noexcept
    { 
        return CommonFlags
            { 
                  (m_bitness32 ? CommonFlags::kBitness32 : 0)
                | (m_bigEndianFormat ? CommonFlags::kBigEndianFormat : 0)
                | (m_endiannessDifference ? CommonFlags::kEndiannessDifference : 0)
            };
    }

    /// @brief Set Common Flags values that will be used by this context
    /// @param commonFlags Common Flags
    constexpr Common& setCommonFlags(CommonFlags commonFlags) noexcept
    { 
        m_bitness32 = commonFlags.bitness32();
        m_bigEndianFormat = commonFlags.bigEndianFormat();
        m_endiannessDifference = commonFlags.endiannessDifference();
        m_endiannessNotMatch = bigEndianFormat() != helpers::isBigEndianPlatform();
        return *this;
    }

    CS_ALWAYS_INLINE constexpr Common& setCommonFlags(uint32_t commonFlags) noexcept
    {
        return setCommonFlags(static_cast<CommonFlags>(commonFlags));
    }

    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool bitness32() const noexcept { return m_bitness32; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool bigEndianFormat() const noexcept { return m_bigEndianFormat; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool endiannessDifference() const noexcept { return m_endiannessDifference; }

    /// @brief Reset all fields to their default values, but leaves binary data and common flags unchanged
    /// @note Common flags are not resets to false because because they are 
    ///     rather environment tool option instead of struct/operation specific.
    virtual Common& resetToDefaultsExceptDataContents() noexcept
    {
        if constexpr (IDeserializationCapableContainer<Container>)
            m_binaryData.seek(0);
        m_protocolVersion = traits::getLatestProtocolVersion();
        m_messageType = Message::Data;
        return *this;
    }

    /// @brief Reset all fields to their default values and clears binary data container
    /// @note Common flags are not resets to false because because they are 
    ///     rather environment tool option instead of struct/operation specific.
    virtual Common& clear() noexcept
    {
        resetToDefaultsExceptDataContents();
        m_binaryData.clear();
        return *this;
    }

private:
    Container& m_binaryData;
    Message m_messageType{ Message::Data };
    protocol_version_t m_protocolVersion{ traits::getLatestProtocolVersion() };
    bool m_protocolVersionsNotMatch{ false };
    bool m_endiannessNotMatch{ false };
    bool m_bitness32{ false };
    bool m_bigEndianFormat{ false };
    bool m_endiannessDifference{ false };
};

} // namespace common_serialization::csp::context
