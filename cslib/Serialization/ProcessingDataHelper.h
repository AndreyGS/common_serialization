/**
 * @file SerializationDataHelper.h
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

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataHelper(const T* p, size_t n, context::Data<S, PM>& context)
{
    return DataProcessor::serializeData(p, n, context);
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataHelper(const T(&arr)[N], context::Data<S, PM>& output)
{
    return DataProcessor::serializeData(arr, output);
}

template<typename T, typename S, typename PM>
concept HasFreeSerializeProcessorFunction 
    = serialization_concepts::ISerializationCapableContainer<S> 
    && serialization_concepts::IPointersMap<PM>
    && requires(const T& t)
{
    { serializeData(t, *(new context::Data<S, PM>())) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataHelper(const T& value, context::Data<S, PM>& context)
{
    if constexpr (HasFreeSerializeProcessorFunction<T, S, PM>)
        return serializeData(value, context);
    else
        return DataProcessor::serializeData(value, context);
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeDataHelper(context::Data<D, PM>& context, size_t n, T* p)
{
    return DataProcessor::deserializeData(context, n, p);
}

template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeDataHelper(context::Data<D, PM>& context, T(&arr)[N])
{
    return DataProcessor::deserializeData(context, arr);
}

template<typename T, typename D, typename PM>
concept HasFreeDeserializeProcessorFunction 
    = serialization_concepts::IDeserializationCapableContainer<D> 
    && serialization_concepts::IPointersMap<PM>
    && requires(const T& t)
{
    { deserializeData(t, *(new context::Data<D, PM>())) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status deserializeDataHelper(context::Data<D, PM>& context, T& value)
{
    if constexpr (HasFreeDeserializeProcessorFunction<T, D, PM>)
        return deserializeData(context, value);
    else
        return DataProcessor::deserializeData(context, value);
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
