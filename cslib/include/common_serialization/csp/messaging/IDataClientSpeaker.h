/**
 * @file cslib/include/common_serialization/csp/messaging/IDataClientSpeaker.h
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

#include "common_serialization/csp/context/InOutData.h"
#include "common_serialization/Containers/Walker.h"

namespace common_serialization::csp::messaging
{

/// @brief Settings of a CSP party
struct CspPartySettings
{
    /// @brief Default CSP version to interact with party
    protocol_version_t mandatoryProtocolVersion{ traits::getLatestProtocolVersion() };
    /// @brief Minimum CSP version that allowed to interact with party
    protocol_version_t minimumProtocolVersion{ 1 };

    /// @brief Mandatory Common Flags in interactions with party
    context::CommonFlags mandatoryCommonFlags{ helpers::isModuleIsBigEndian() };
    /// @brief Forbidden Common Flags on party
    context::CommonFlags forbiddenCommonFlags;

    /// @brief Mandatory Data Flags in interactions with party
    context::DataFlags mandatoryDataFlags;
    /// @brief Forbidden Data Flags on party
    context::DataFlags forbiddenDataFlags;

    /// @brief List of availible party interfaces and their settings
    Vector<traits::Interface> availableInterfaces;
};

class IDataClientSpeaker
{
public:
    /// @brief Constructor
    /// @param serverSettings CSP Settings of particular server
    constexpr IDataClientSpeaker(const CspPartySettings& serverSettings) noexcept;

    constexpr Status checkSettings(context::SInOutData<>& ctx);

    /// @brief Method for sending to and receiving from server binary data
    /// @details This method must not make assumptions on what binary input and ouput data is.
    ///     It must be implemented as transport function from client to server and vice versa.
    ///     For example, it may be function that sends and receives data to and from socket.
    /// @param binInput Data that is prepared by handleData method
    /// @param binOutput Data that should be returned for processing by handleData method
    /// @return Status of operation
    virtual Status speak(BinVector& binInput, BinWalker& binOutput) = 0;

    [[nodiscard]] constexpr const CspPartySettings& getServerCspSettings() const noexcept;
    [[nodiscard]] constexpr void setServerCspSettings(const CspPartySettings& serverSettings) noexcept;

private:
    CspPartySettings m_serverSettings;
};

constexpr IDataClientSpeaker::IDataClientSpeaker(const CspPartySettings& serverSettings) noexcept
    : m_serverSettings(serverSettings)
{ }

[[nodiscard]] constexpr const CspPartySettings& IDataClientSpeaker::getServerCspSettings() const noexcept
{
    return m_serverSettings;
}

[[nodiscard]] constexpr void IDataClientSpeaker::setServerCspSettings(const CspPartySettings& serverSettings) noexcept
{
     m_serverSettings = serverSettings;
}




} // namespace common_serialization::csp::messaging
