/**
 * @file Incarnator.h
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

#include "../Subscribers/Subscriber.h"

namespace common_serialization
{

namespace csp
{

class Incarnator
{
public:
    template<typename InputType, serialization_concepts::ISerializationCapableContainer S>
    Status initDataMessage(const InputType& input, S& output, interface_version_t minimumInterfaceVersion = InputType::getInterfaceVersion)
    {
        if constexpr (IsCspStatusMessage<InputType>)
        {

        }
        return Status::kNoError;
    }

    template<typename InputType, serialization_concepts::ISerializationCapableContainer S>
    Status initStatusMessage(const InputType& input, S& output, v)

    Status receiveMessage(Walker<uint8_t>& input, Vector<uint8_t>& output)
    {
        return Status::kNoError;
    }
};

} // namespace csp

} // namespace common_serialization