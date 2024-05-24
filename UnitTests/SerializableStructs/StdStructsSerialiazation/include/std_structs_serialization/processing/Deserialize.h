/**
 * @file UnitTests/SerializableStructs/StdStructsSerialiazation/include/std_structs_serialization/processing/Deserialize.h
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

namespace common_serialization::csp::processing::data::templates
{

template<typename T, typename Traits, typename Allocator, typename X>
Status deserialize(X& ctx, std::basic_string<T, Traits, Allocator>& value)
{
    using size_type = typename std::basic_string<T, Traits, Allocator>::size_type;

    assert(sizeof(size_type) <= sizeof(size_t));

    value.clear();
    size_type size = 0;
    CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));
    value.resize(size);
    CS_RUN(BodyProcessor::deserialize(ctx, size + 1, value.data()));

    return Status::NoError;
}

template<typename T, typename Allocator, typename X>
Status deserialize(X& ctx, std::vector<T, Allocator>& value)
{
    using size_type = typename std::vector<T, Allocator>::size_type;

    assert(sizeof(size_type) <= sizeof(size_t));

    value.clear();
    size_type size = 0;
    CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));
    value.resize(size);
    CS_RUN(BodyProcessor::deserialize(ctx, size, value.data()));

    return Status::NoError;
}

template<typename T1, typename T2, typename X>
Status deserialize(X& ctx, std::pair<T1, T2>& value)
{
    CS_RUN(BodyProcessor::deserialize(ctx, value.first));
    CS_RUN(BodyProcessor::deserialize(ctx, value.second));

    return Status::NoError;
}

template<typename K, typename V, class Compare, class Allocator, typename X>
Status deserialize(X& ctx, std::map<K, V, Compare, Allocator>& value)
{
    using size_type = std::map<K, V, Compare, Allocator>::size_type;

    assert(sizeof(size_type) <= sizeof(size_t));

    value.clear();
    size_type size = 0;
    CS_RUN(BodyProcessor::deserializeSizeT(ctx, size));

    for (size_type i = 0; i < size; ++i)
    {
        std::pair<K, V> element;
        CS_RUN(BodyProcessor::deserialize(ctx, element));
        value.insert(std::move(element));
    }

    return Status::NoError;
}

template<typename Tuple, typename X, size_t... Is>
Status deserializeTuple(X& ctx, std::index_sequence<Is...>, Tuple& value)
{
    Status status = Status::NoError;

    // There is a warning that statusSuccess return value is discarded,
    // but this is not the case. I can transform this expression
    // in lambda that return this expression but I don't think that
    // we should be lead by analyzer mistakes.
    (statusSuccess(status = BodyProcessor::deserialize(ctx, std::get<Is>(value))) && ...);

    return status;
}

template<typename X, typename... Ts>
Status deserialize(X& ctx, std::tuple<Ts...>& value)
{
    return deserializeTuple(ctx, std::make_index_sequence<sizeof...(Ts)>{}, value);
}

} // namespace common_serialization::csp::processing::data::templates