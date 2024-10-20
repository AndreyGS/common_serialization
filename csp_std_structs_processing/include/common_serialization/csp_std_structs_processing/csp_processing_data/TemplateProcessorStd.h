/**
 * @file common_serialization/csp_std_structs_processing/csp_processing_data/TemplateProcessorStd.h
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

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <common_serialization/csp_base/processing/data/BodyProcessor.h>
#include <common_serialization/csp_base/processing/data/TemplateProcessor.h>

namespace common_serialization::csp::processing::data
{

template<typename T, typename Traits, typename Allocator>
class TemplateProcessor<std::basic_string<T, Traits, Allocator>, T, Traits, Allocator>
{
public:
    static Status serialize(const std::basic_string<T, Traits, Allocator>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serializeSizeT(value.length(), ctx));
        AGS_CS_RUN(BodyProcessor::serialize(value.c_str(), value.length() + 1, ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, std::basic_string<T, Traits, Allocator>& value)
    {
        using size_type = typename std::basic_string<T, Traits, Allocator>::size_type;

        assert(sizeof(size_type) <= sizeof(size_t));

        value.clear();
        size_type size = 0;
        AGS_CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));
        value.resize(size);
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, size + 1, value.data()));

        return Status::NoError;
    }
};

template<typename T, typename Allocator>
class TemplateProcessor<std::vector<T, Allocator>, T, Allocator>
{
public:
    static Status serialize(const std::vector<T, Allocator>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serializeSizeT(value.size(), ctx));
        AGS_CS_RUN(BodyProcessor::serialize(value.data(), value.size(), ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, std::vector<T, Allocator>& value)
    {
        using size_type = typename std::vector<T, Allocator>::size_type;

        assert(sizeof(size_type) <= sizeof(size_t));

        value.clear();
        size_type size = 0;
        AGS_CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));
        value.resize(size);
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, size, value.data()));

        return Status::NoError;
    }
};

template<typename T1, typename T2>
class TemplateProcessor<std::pair<T1, T2>, T1, T2>
{
public:
    static Status serialize(const std::pair<T1, T2>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serialize(value.first, ctx));
        AGS_CS_RUN(BodyProcessor::serialize(value.second, ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, std::pair<T1, T2>& value)
    {
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, value.first));
        AGS_CS_RUN(BodyProcessor::deserialize(ctx, value.second));

        return Status::NoError;
    }
};

template<typename K, typename V, class Compare, class Allocator>
class TemplateProcessor<std::map<K, V, Compare, Allocator>, K, V, Compare, Allocator>
{
public:
    static Status serialize(const std::map<K, V, Compare, Allocator>& value, context::SData& ctx)
    {
        AGS_CS_RUN(BodyProcessor::serializeSizeT(value.size(), ctx));

        for (auto& pair : value)
            AGS_CS_RUN(BodyProcessor::serialize(pair, ctx));

        return Status::NoError;
    }

    static Status deserialize(context::DData& ctx, std::map<K, V, Compare, Allocator>& value)
    {
        using size_type = std::map<K, V, Compare, Allocator>::size_type;

        assert(sizeof(size_type) <= sizeof(size_t));

        value.clear();
        size_type size = 0;
        AGS_CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));

        for (size_type i = 0; i < size; ++i)
        {
            std::pair<K, V> element;
            AGS_CS_RUN(BodyProcessor::deserialize(ctx, element));
            value.insert(std::move(element));
        }

        return Status::NoError;
    }
};

template<typename... Ts>
class TemplateProcessor<std::tuple<Ts...>, Ts...>
{
public:
    static Status serialize(const std::tuple<Ts...>& value, context::SData& ctx)
    {
        return serializeTupleHelper(value, std::make_index_sequence<sizeof...(Ts)>{}, ctx);
    }

    template<size_t... Is>
    static Status serializeTupleHelper(const std::tuple<Ts...>& value, std::index_sequence<Is...>, context::SData& ctx)
    {
        Status status = Status::NoError;

        // There is a warning that statusSuccess return value is discarded,
        // but this is not the case. I can transform this expression
        // in lambda that return this expression but I don't think that
        // we should be lead by analyzer mistakes.
        (statusSuccess(status = BodyProcessor::serialize(std::get<Is>(value), ctx)) && ...);

        return status;
    }

    static Status deserialize(context::DData& ctx, std::tuple<Ts...>& value)
    {
        return deserializeTupleHelper(ctx, std::make_index_sequence<sizeof...(Ts)>{}, value);
    }

    template<size_t... Is>
    static Status deserializeTupleHelper(context::DData& ctx, std::index_sequence<Is...>, std::tuple<Ts...>& value)
    {
        Status status = Status::NoError;

        // There is a warning that statusSuccess return value is discarded,
        // but this is not the case. I can transform this expression
        // in lambda that return this expression but I don't think that
        // we should be lead by analyzer mistakes.
        (statusSuccess(status = BodyProcessor::deserialize(ctx, std::get<Is>(value))) && ...);

        return status;
    }
};

} // namespace common_serialization::csp::processing::data
