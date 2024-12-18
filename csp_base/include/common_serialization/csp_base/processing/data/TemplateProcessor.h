/**
 * @file common_serialization/csp_base/processing/data/TemplateProcessor.h
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

#include <common_serialization/csp_base/processing/data/BodyProcessor.h>

namespace common_serialization::csp::processing::data
{

template<typename T, typename... Ts>
class TemplateProcessor<VectorT<T, Ts...>, T, Ts...>
{
public:
    static Status serialize(const VectorT<T, Ts...>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serializeSizeT(value.size(), ctx));
        AGS_CS_RUN(BodyProcessor::serialize(value.data(), value.size(), ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, VectorT<T, Ts...>& value)
    {
        value.clear();

        typename VectorT<T, Ts...>::size_type size = 0;
        AGS_CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));
        AGS_CS_RUN(value.reserve(size));
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, size, value.data()));
        value.m_dataSize = size;

        return Status::NoError;
    }
};

} // namespace common_serialization::csp::processing::data
