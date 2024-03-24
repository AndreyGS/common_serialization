/**
 * @file cslib/include/common_serialization/csp/processing/DataCsStructs.h
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

#include "common_serialization/csp/traits.h"
#include "common_serialization/csp/processing/DataProcessor.h"

namespace common_serialization::csp::processing
{

template<>
constexpr Status DataProcessor::serializeData(const Id& value, context::SData<>& ctx)
{
    CS_RUN(serializeData(value.id, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, Id& value)
{
    CS_RUN(deserializeData(ctx, value.id));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const context::DataFlags& value, context::SData<>& ctx)
{
    CS_RUN(serializeData(static_cast<uint32_t>(value), ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, context::DataFlags& value)
{
    uint32_t dataFlags{ 0 };
    CS_RUN(deserializeData(ctx, dataFlags));
    value = dataFlags;

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::serializeData(const traits::Interface& value, context::SData<>& ctx)
{
    CS_RUN(serializeData(value.id, ctx));
    CS_RUN(serializeData(value.version, ctx));
    CS_RUN(serializeData(value.mandatoryDataFlags, ctx));
    CS_RUN(serializeData(value.forbiddenDataFlags, ctx));

    return Status::kNoError;
}

template<>
constexpr Status DataProcessor::deserializeData(context::DData<>& ctx, traits::Interface& value)
{
    CS_RUN(deserializeData(ctx, value.id));
    CS_RUN(deserializeData(ctx, value.version));
    CS_RUN(deserializeData(ctx, value.mandatoryDataFlags));
    CS_RUN(deserializeData(ctx, value.forbiddenDataFlags));

    return Status::kNoError;
}

} // namespace common_serialization::csp::processing
