/**
 * @file SerializableDeclaration.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

#include "Containers/RawData.h"

#include <unordered_map> // temporary header

namespace common_serialization
{

template<typename T>
class Serializable
{
public:
    ptrdiff_t serialize(RawData& output) const noexcept { return 0; }
    ptrdiff_t deserialize(const RawData& input) { return 0; }
    ptrdiff_t serialize_next(RawData& output, std::unordered_map<void*, void*>& pointersMap) const noexcept { return 0; }
    ptrdiff_t deserialize_next(const RawData& input, std::unordered_map<void*, void*>& pointersMap) { return 0; }

    [[nodiscard]] static constexpr Vector<uint64_t, StrategicAllocatorHelper<uint8_t, RawNoexceptAllocator<uint8_t>>>& getAncestors() noexcept;
    [[nodiscard]] static constexpr Vector<uint64_t, StrategicAllocatorHelper<uint8_t, RawNoexceptAllocator<uint8_t>>>& getMembers() noexcept;
    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionThis() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionThisAndDependencies() noexcept;

};

template<typename T>
[[nodiscard]] constexpr Vector<uint64_t, StrategicAllocatorHelper<uint8_t, RawNoexceptAllocator<uint8_t>>>& Serializable<T>::getAncestors() noexcept
{
    return T::ancestors;
}

template<typename T>
[[nodiscard]] constexpr Vector<uint64_t, StrategicAllocatorHelper<uint8_t, RawNoexceptAllocator<uint8_t>>>& Serializable<T>::getMembers() noexcept
{
    return T::members;
}

template<typename T>
[[nodiscard]] constexpr uint64_t Serializable<T>::getNameHash() noexcept
{
    return T::nameHash;
}

template<typename T>
[[nodiscard]] constexpr uint32_t Serializable<T>::getVersionThis() noexcept
{
    return T::versionThis;
}

template<typename T>
[[nodiscard]] constexpr uint32_t Serializable<T>::getVersionThisAndDependencies() noexcept
{
    return T::versionThisAndDependencies;
}

} // namespace common_serialization
