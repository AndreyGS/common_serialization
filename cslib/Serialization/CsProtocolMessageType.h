/**
 * @file CsProtocolMessageType.h
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

enum class CsProtocolMessageType : uint_fast32_t
{
    kData = 0x0,                            // default message type
    //
    // message body format:
    //
    // {
    //     uint32_t interfaceVersion;
    //     uint64_t structNameHash;
    //     uint8_t serializedData[anysize]; // rest of the data varies by CsProtocolFlags that was set
    //                                      // and data that was serialized
    // }
    //
    kNotSupportedProtocol = 0x1,            // sends when serialization protocol is not supported
    //
    // message body format:
    //
    // {
    //     uint8_t supportedProtocolsVersionsSize;
    //     uint8_t supportedProtocolsVersions[supportedProtocolsVersionsSize];
    // }
    //
    kNotSupportedInterfaceVersion = 0x2,    // sends when low supported version is bigger or
    // high version is lesser than in serialized data
    // 
    // message body format:
    // 
    // {
    //     uint64_t structNameHash;
    //     uint32_t minimumSupportedInterfaceVersion;
    //     uint32_t maximalSupportedInterfaceVersion;
    // }
    // 
    kMismatchOfInterfaceVersions = 0x2,     // sends when its primal (latest) version not
    // equal to version of input data, but version of input data
    // is supported and CsProtocolFlags::interfaceVersionsNotMatch is false
    // 
    // message body format
    // 
    // {
    //     uint64_t structHashName;
    // }
    //                                      
};

