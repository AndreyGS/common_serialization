/**
 * @file cslib/include/common_serialization/NotCspInterfaceProcessing/Others/Deserialize.h
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

#include <common_serialization/csp/Traits.h>
#include <common_serialization/csp/processing/DataBodyProcessor.h>

namespace common_serialization::csp::processing::data
{

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, Id& value)
{
    CS_RUN(deserialize(ctx, value.id));

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, context::DataFlags& value)
{
    uint32_t dataFlags{ 0 };
    CS_RUN(deserialize(ctx, dataFlags));
    value = dataFlags;

    return Status::NoError;
}

template<>
constexpr Status BodyProcessor::deserialize(context::DData& ctx, Interface& value)
{
    CSP_DESERIALIZE_ANY_SIMPLY_ASSIGNABLE(ctx, value)

    CS_RUN(deserialize(ctx, value.id));
    CS_RUN(deserialize(ctx, value.version));
    CS_RUN(deserialize(ctx, value.mandatoryDataFlags));
    CS_RUN(deserialize(ctx, value.forbiddenDataFlags));

    return Status::NoError;
}

} // namespace common_serialization::csp::processing::data
