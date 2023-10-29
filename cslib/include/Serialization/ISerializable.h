/**
 * @file ISerializable.h
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

#include "Processing.h"
#include "ProcessingDataProcessor.h"

namespace common_serialization
{

namespace csp
{

// ISerializable base must have permanent size regardless of platform alignment
#pragma pack(push, 1)

template<typename T>
class ISerializable
{
public:
    using instance_type = T;

    template<serialization_concepts::ISerializationCapableContainer S>
    constexpr Status serialize(S& output) const noexcept;
    template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    constexpr Status serialize(context::SData<S, PM>& ctx) const noexcept;

    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserialize(D& input);
    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    constexpr Status deserialize(context::DData<D, PM>& ctx);

    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getThisVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getInterfaceVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getMinimumInterfaceVersion() noexcept;
    [[nodiscard]] static constexpr const uint32_t* getVersionsHierarchy() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionsHierarchySize() noexcept;
};

#pragma pack(pop)

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status ISerializable<T>::serialize(S& output) const noexcept
{
    context::SData<S> ctx(output, getInterfaceVersion());

    return serialize(ctx);
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
constexpr Status ISerializable<T>::serialize(context::SData<S, PM>& ctx) const noexcept
{
    if (ctx.getInterfaceVersion() == traits::kInterfaceVersionMax)
        ctx.setInterfaceVersion(getInterfaceVersion());

    RUN(processing::serializeHeaderContext(ctx));
    RUN(processing::serializeDataContext<T>(ctx));

    return processing::DataProcessor::serializeData(static_cast<const T&>(*this), ctx);
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserialize(D& input)
{
    context::DData<D> ctx(input, getMinimumInterfaceVersion());

    return deserialize(ctx);
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
constexpr Status ISerializable<T>::deserialize(context::DData<D, PM>& ctx)
{
    RUN(processing::deserializeHeaderContext(ctx));

    uint64_t nameHash = 0;
    uint32_t minimumInterfaceVersion = ctx.getInterfaceVersion() == traits::kInterfaceVersionMax ? getMinimumInterfaceVersion() : ctx.getInterfaceVersion();

    RUN(processing::deserializeDataContext(ctx, nameHash));
    RUN(processing::deserializeDataContextPostprocess<T>(ctx, nameHash, minimumInterfaceVersion));
    
    return processing::DataProcessor::deserializeData(ctx, static_cast<T&>(*this));
}

template<typename T>
[[nodiscard]] constexpr uint64_t ISerializable<T>::getNameHash() noexcept
{
    return T::kNameHash;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getThisVersion() noexcept
{
    return T::kVersionsHierarchy[0];
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getInterfaceVersion() noexcept
{
    return T::kInterfaceVersion;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getMinimumInterfaceVersion() noexcept
{
    return getVersionsHierarchy()[getVersionsHierarchySize() - 1];
}

template<typename T>
[[nodiscard]] constexpr const uint32_t* ISerializable<T>::getVersionsHierarchy() noexcept
{
    return T::kVersionsHierarchy;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getVersionsHierarchySize() noexcept
{
    return std::size(T::kVersionsHierarchy);
}

} // namespace csp

} // namespace common_serialization
