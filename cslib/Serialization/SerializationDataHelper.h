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

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(const T* p, size_t n, S& output)
{
    return SerializationProcessor::serializeData(p, n, output);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(const T* p, size_t n, CsProtocolFlags flags, S& output)
{
    return SerializationProcessor::serializeData(p, n, flags, output);
}

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataCompatHelper(const T* p, size_t n, CsProtocolFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, S& output)
{
    return SerializationProcessor::serializeDataCompat(p, n, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(const T(&arr)[N], S& output)
{
    return SerializationProcessor::serializeData(arr, output);
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(const T(&arr)[N], CsProtocolFlags flags, S& output)
{
    return SerializationProcessor::serializeData(arr, flags, output);
}

template<typename T, size_t N, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataCompatHelper(const T(&arr)[N], CsProtocolFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, S& output)
{
    return SerializationProcessor::serializeDataCompat(arr, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);
}

template<typename T, typename S>
concept HasFreeSerializeProcessorFunction = serialization_concepts::ISerializationCapableContainer<S> && requires(const T& t)
{
    { serializeData(t, *(new S)) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(T value, S& output)
{
    if constexpr (HasFreeSerializeProcessorFunction<T, S>)
        return serializeData(value, output);
    else
        return SerializationProcessor::serializeData(value, output);
}

template<typename T, typename S>
concept HasFreeSerializeWithFlagsProcessorFunction = serialization_concepts::ISerializationCapableContainer<S> && requires(const T& t)
{
    { serializeData(t, *(new CsProtocolFlags), *(new S)) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::ISerializationCapableContainer S>
constexpr Status serializeDataHelper(T value, CsProtocolFlags flags, S& output)
{
    if constexpr (HasFreeSerializeWithFlagsProcessorFunction<T, S>)
        return serializeData(value, flags, output);
    else
        return SerializationProcessor::serializeData(value, flags, output);
}

template<typename T, typename S, typename PM>
concept HasFreeSerializeCompatProcessorFunction = serialization_concepts::ISerializationCapableContainer<S> && requires(const T& t)
{
    { serializeDataCompat(t, *(new CsProtocolFlags), *(new uint32_t), *(new PM), *(new S)) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status serializeDataCompatHelper(T value, CsProtocolFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, PM& pointersMap, S& output)
{
    if constexpr (HasFreeSerializeCompatProcessorFunction<T, S, PM>)
        return serializeDataCompat(value, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);
    else
        return SerializationProcessor::serializeDataCompat(value, flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, size_t n, T* p)
{
    return SerializationProcessor::deserializeData(input, n, p);
}

template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, size_t n, CsProtocolFlags flags, T* p)
{
    return SerializationProcessor::deserializeData(input, n, flags, p);
}

template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, T(&arr)[N])
{
    return SerializationProcessor::deserializeData(input, arr);
}

template<typename T, size_t N, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, CsProtocolFlags flags, T(&arr)[N])
{
    return SerializationProcessor::deserializeData(input, flags, arr);
}

template<typename T, typename D>
concept HasFreeDeserializeProcessorFunction = serialization_concepts::IDeserializationCapableContainer<D> && requires(T t)
{
    { deserializeData(*(new D), t) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, T& value)
{
    if constexpr (HasFreeDeserializeProcessorFunction<T, D>)
        return deserializeData(input, value);
    else
        return SerializationProcessor::deserializeData(input, value);
}

template<typename T, typename D>
concept HasFreeDeserializeWithFlagsProcessorFunction = serialization_concepts::IDeserializationCapableContainer<D> && requires(T t)
{
    { deserializeData(*(new D), t) } -> std::same_as<Status>;
};

template<typename T, serialization_concepts::IDeserializationCapableContainer D>
constexpr Status deserializeDataHelper(D& input, CsProtocolFlags flags, T& value)
{
    if constexpr (HasFreeDeserializeWithFlagsProcessorFunction<T, D>)
        return deserializeData(input, flags, value);
    else
        return SerializationProcessor::deserializeData(input, flags, value);
}

} // namespace common_serialization
