/**
 * @file common_serialization/tests_csp_interface/csp_processing_data/TemplateProcessor.h
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
#include <common_serialization/csp_base/processing/data/TemplateProcessor.h>
#include <common_serialization/tests_csp_interface/structs.h>
#include <common_serialization/tests_csp_interface/structs_legacy.h>

namespace common_serialization::csp::processing::data
{

template<typename T>
class TemplateProcessor<tests_csp_interface::BigStructs<T>, T>
{
public:
    static Status serialize(const tests_csp_interface::BigStructs<T>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serialize(value.m_vector, ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, tests_csp_interface::BigStructs<T>& value)
    {
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, value.m_vector));

        return Status::NoError;
    }
};

} // namespace common_serialization::csp::processing::data
