/**
 * @file UnitTests/SerializableStructs/StdStructsSerialiazation/include/std_structs_serialization/processing/Serialize.h
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
Status serialize(const std::basic_string<T, Traits, Allocator>& value, X& ctx)
{
    CS_RUN(BodyProcessor::serializeSizeT(value.length(), ctx));
    CS_RUN(BodyProcessor::serialize(value.c_str(), value.length() + 1, ctx));

    return Status::kNoError;
}

template<typename T, typename Allocator, typename X>
Status serialize(const std::vector<T, Allocator>& value, X& ctx)
{
    CS_RUN(BodyProcessor::serializeSizeT(value.size(), ctx));
    CS_RUN(BodyProcessor::serialize(value.data(), value.size(), ctx));

    return Status::kNoError;
}

template<typename T1, typename T2, typename X>
Status serialize(const std::pair<T1, T2>& value, X& ctx)
{
    CS_RUN(BodyProcessor::serialize(value.first, ctx));
    CS_RUN(BodyProcessor::serialize(value.second, ctx));

    return Status::kNoError;
}

template<typename K, typename V, class Compare, class Allocator, typename X>
Status serialize(const std::map<K, V, Compare, Allocator>& value, X& ctx)
{
    CS_RUN(BodyProcessor::serializeSizeT(value.size(), ctx));

    for (auto& pair : value)
        CS_RUN(serialize(pair, ctx));

    return Status::kNoError;
}

template<typename Tuple, typename X, size_t... Is>
Status serializeTuple(const Tuple& value, std::index_sequence<Is...>, X& ctx)
{
    Status status = Status::kNoError;

    // There is a warning that statusSuccess return value is discarded,
    // but this is not the case. I can transform this expression
    // in lambda that return this expression but I don't think that
    // we should be lead by analyzer mistakes.
    (statusSuccess(status = BodyProcessor::serialize(std::get<Is>(value), ctx)) && ...);

    return status;
}

template<typename X, typename... Ts>
Status serialize(const std::tuple<Ts...>& value, X& ctx)
{
    return serializeTuple(value, std::make_index_sequence<sizeof...(Ts)>{}, ctx);
}

} // namespace common_serialization::csp::processing::data::templates
