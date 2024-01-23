/**
 * @file cslib/include/common_serialization/csp/context/Message.h
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

namespace common_serialization::csp::context
{

/// @brief Message types that are using in CSP
enum class Message : uint_fast32_t
{
    /// @brief Status message
    /// @details
    ///     Format of Private parts of message:
    /// 
    ///     {
    ///         struct
    ///         {
    ///             Status status;
    ///         } privateHeader;
    ///
    ///         struct
    ///         {
    ///             uint8_t serializedData[anysize];
    ///         } body;
    ///     }
    /// 
    ///     Format of message body depends on status code
    ///     
    ///     Status == kErrorNotSupportedProtocolVersion
    ///     
    ///     sends when serialization protocol is not supported
    ///     
    ///     message body format :
    ///     
    ///     {
    ///         uint8_t supportedProtocolsVersionsSize;
    ///         uint8_t supportedProtocolsVersions[supportedProtocolsVersionsSize];
    ///     }
    ///     
    ///     Status == kErrorNotSupportedInOutInterfaceVersion
    ///     
    ///     message body format :
    /// 
    ///     {
    ///         uint32_t inMinimumSupportedInterfaceVersion;
    ///         uint32_t inMaximumSupportedInterfaceVersion;
    ///         uint32_t outMinimumSupportedInterfaceVersion;
    ///         uint32_t outMaximumSupportedInterfaceVersion;
    ///     }
    kStatus = 0x0,
                                            
    /// @brief Default message type (standard processing)
    /// @details Using in one way single struct serialization/deserialization,
    ///     that is no return (response) message is expected
    /// 
    ///     Format of Private parts of message:
    ///
    ///     {
    ///         struct
    ///         {
    ///             uuid inStructId;
    ///             DataFlags dataFlags;
    ///             uint32_t interfaceVersion;
    ///         } privateHeader;
    ///         
    ///         struct
    ///         {
    ///             uint8_t serializedData[anysize];
    ///         } body; // varies by DataFlags that was set and struct that was serialized
    ///     }
    kData = 0x1,                            
                                            
    /// @brief Request for servers protocol capabilities
    /// @details
    ///     Format of Private parts of message:
    /// 
    ///     {
    ///         struct
    ///         {
    ///             CommonCapabilities requestedCapability;
    ///         }  body;
    ///     }
    kCommonCapabilitiesRequest = 0x2,

    /// @brief Request for settings that need for comunication from other side
    ///     (other side is only server, for now)
    /// @details
    ///     Format of Private parts of message:
    /// 
    ///     {
    ///     }
    /// 
    ///     In response server will must send csp::messaging::service_structs::CspPartySettings
    ///     struct with its mandatory settings
    kGetSettings = 0x3
};

} // namespace common_serialization::csp::context
