/**
 * @file cslib/include/common_serialization/csp/messaging/IClientSpeaker.h
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

namespace common_serialization::csp::messaging
{

/// @brief Interface for CSP Client to speak with CSP Server
class IClientSpeaker
{
public:
    /// @brief Method for sending to and receiving from server binary data
    /// @details This method must not make assumptions on what binary input and ouput data is.
    ///     It must be implemented as transport function from client to server and vice versa.
    ///     For example, it may be function that sends and receives data to and from socket.
    /// @param binInput Data that is prepared by handleData method
    /// @param binOutput Data that should be returned for processing by handleData method
    /// @return Status of operation
    virtual Status speak(BinVectorT& binInput, BinWalkerT& binOutput) = 0;
};

} // namespace common_serialization::csp::messaging
