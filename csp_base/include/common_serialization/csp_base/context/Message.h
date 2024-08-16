/**
 * @file common_serialization/csp_base/context/Message.h
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
enum class Message : uint16_t
{
    /// @brief Status message
    /// @details
    ///     Format of Private parts of message:
    /// 
    ///     {
    ///         struct
    ///         {
    ///             Status status;
    ///         } privateContext;
    ///
    ///         struct
    ///         {
    ///             uint8_t serializedData[anysize];
    ///         } body;
    ///     }
    /// 
    ///     Format of message body depends on status code
    ///     
    ///     Status == ErrorNotSupportedProtocolVersion
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
    Status = 0x0,
                                            
    /// @brief Default message type
    /// 
    ///     Format of Private parts of message:
    ///
    ///     {
    ///         struct
    ///         {
    ///             uuid inStructId;
    ///             DataFlags dataFlags;
    ///             uint32_t interfaceVersion;
    ///         } privateContext;
    ///         
    ///         struct
    ///         {
    ///             uint8_t serializedData[anysize];
    ///         } body; // varies by DataFlags that was set and struct that was serialized
    ///     }
    Data = 0x1,                            

    /// @brief Request for settings that need for comunication. Sends from Client to Server
    /// @details
    ///     Format of Private parts of message:
    /// 
    ///     {
    ///     }
    /// 
    ///     In response server must send Data message without any DataFlags applied.
    ///     This Data message shall contain csp::service_structs::CspPartySettings
    ///     struct with servers mandatory settings
    GetSettings = 0x2
};

} // namespace common_serialization::csp::context
