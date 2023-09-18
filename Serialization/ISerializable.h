/**
 * @file ISerializable.h
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

#include "Containers/Vector.h"
#include "SerializableConcepts.h"
#include "SerializeCommon.h"

#include <unordered_map> // temporary header

namespace common_serialization
{

template<typename T>
class ISerializable
{
public:
    enum class ProcessingFlags
    {
        kFastAndSimple = 0x00,
        kCompatabilityMode = 0x01
    };

    // first function to call on new serialize operation
    template<serializable_concepts::ISerializationCapableContainer S>
    constexpr int serialize(S& output) const noexcept;

    // calling in subsequent serializations of nested fields/types or if one need to serialize another struct
    // and there is no changes in interface version after previous serialize call
    template<serializable_concepts::ISerializationCapableContainer S>
    constexpr int serializeNext(S& output) const noexcept;

    template<serializable_concepts::IDeserializationCapableContainer D>
    constexpr int deserialize(const D& input) { return 0; }
    template<serializable_concepts::IDeserializationCapableContainer D>
    constexpr int deserializeNext(const D& input, std::unordered_map<void*, void*>& pointersMap) { return 0; }
    
    [[nodiscard]] static constexpr uint64_t* getAncestors() noexcept;
    [[nodiscard]] static constexpr uint64_t* getMembers() noexcept;
    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getThisVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getInterfaceVersion() noexcept;

    [[nodiscard]] static constexpr uint32_t getProtocolVersion() noexcept;

private:
    static constexpr uint8_t kProtocolVersion = 1;

    uint32_t m_convertedFromVersion = 0;    // 0 is for "no coversion was performed"
};

template<typename T>
template<serializable_concepts::ISerializationCapableContainer S>
constexpr int ISerializable<T>::serialize(S& output) const noexcept
{
    output.pushBackArithmeticValue(getProtocolVersion());   // here we don't add flags, because this function uses only kFastAndSimple
    output.pushBackArithmeticValue(getInterfaceVersion());
    return serializeNext(output);
}

template<typename T>
template<serializable_concepts::ISerializationCapableContainer S>
constexpr int ISerializable<T>::serializeNext(S& output) const noexcept
{
    output.pushBackArithmeticValue(getNameHash());
    return serializeThis(static_cast<const T&>(*this), output);
}

template<typename T>
[[nodiscard]] constexpr uint64_t* ISerializable<T>::getAncestors() noexcept
{
    return T::kAncestors;
}

template<typename T>
[[nodiscard]] constexpr uint64_t* ISerializable<T>::getMembers() noexcept
{
    return T::kMembers;
}

template<typename T>
[[nodiscard]] constexpr uint64_t ISerializable<T>::getNameHash() noexcept
{
    return T::kNameHash;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getThisVersion() noexcept
{
    return T::kVersionThis;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getInterfaceVersion() noexcept
{
    return T::kVersionInterface;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getProtocolVersion() noexcept
{
    return kProtocolVersion;
}

} // namespace common_serialization
