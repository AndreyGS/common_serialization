/**
 * @file CspBase.h
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

#include "CspContext.h"

namespace common_serialization
{

namespace csp
{

namespace stages
{



// Do not add here any member fields or methods
class CspBase : public CspTraits
{
public:
    template<serialization_concepts::ISerializationCapableContainer S>
    static constexpr Status serializeHeader(S& output, const CspContextHeader& context) noexcept;

    template<serialization_concepts::IDeserializationCapableContainer D>
    static constexpr Status deserializeHeader(D& input, const CspContextHeader& context) noexcept;
};

template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status CspBase::serializeHeader(S& output, const CspContextHeader& context) noexcept
{
    if (!isProtocolVersionSupported(context.getProtocolVersion()))
        return Status::kErrorNotSupportedSerializationProtocolVersion;

    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(context.getProtocolVersion()) | (static_cast<uint32_t>(context.getFlags()) << 8)));
    RUN(output.pushBackArithmeticValue(context.getMessageType()));

    return Status::kNoError;
}

template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status CspBase::deserializeHeader(D& input, const CspContextHeader& context) noexcept
{
    uint32_t versionAndFlags = 0;
    CspMessageType messageType = CspMessageType::kData;

    RUN(input.readArithmeticValue(versionAndFlags));

    context.setProtocolVersion(versionAndFlags & 0xff);
    context.setFlags(versionAndFlags >> 8);

    RUN(input.readArithmeticValue(messageType));

    context.setMessageType(messageType);

    return Status::kNoError;
}

} // namespace stages

} // namespace csp

} // namespace common_serialization
