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
#include "SerializationFlags.h"
#include "SerializableConcepts.h"
#include "SerializeCommon.h"

#include <unordered_map> // temporary header

namespace common_serialization
{

inline constexpr uint8_t kSerializationProtocolVersion = 1;

template<typename T>
class ISerializable
{
public:


    // first function to call on new serialize operation
    template<serializable_concepts::ISerializationCapableContainer S>
    constexpr int serialize(S& output, SerializationFlags flags = SerializationFlags{}) const noexcept;

    // calling in subsequent serializations of nested fields/types or if one need to serialize another struct
    // and there is no changes in interface version after previous serialize call
    template<serializable_concepts::ISerializationCapableContainer S>
    constexpr int serializeNext(S& output, SerializationFlags flags = SerializationFlags{}) const noexcept;

    template<serializable_concepts::IDeserializationCapableContainer D>
    constexpr int deserialize(const D& input);
    template<serializable_concepts::IDeserializationCapableContainer D>
    constexpr int deserializeNext(const D& input, SerializationFlags flags);
    
    [[nodiscard]] static constexpr uint64_t* getAncestors() noexcept;
    [[nodiscard]] static constexpr uint64_t* getMembers() noexcept;
    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getThisVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getInterfaceVersion() noexcept;

private:
    uint32_t m_convertedFromVersion = 0;    // 0 is for "no coversion was performed"
};

template<typename T>
template<serializable_concepts::ISerializationCapableContainer S>
constexpr int ISerializable<T>::serialize(S& output, SerializationFlags flags) const noexcept
{
    output.pushBackArithmeticValue(static_cast<uint32_t>(kSerializationProtocolVersion) | (static_cast<uint32_t>(flags) << 8));
    output.pushBackArithmeticValue(getInterfaceVersion());
    return serializeNext(output, flags);
}

template<typename T>
template<serializable_concepts::ISerializationCapableContainer S>
constexpr int ISerializable<T>::serializeNext(S& output, SerializationFlags flags) const noexcept
{
    output.pushBackArithmeticValue(getNameHash());

    if (!flags)
        return serializeThis(static_cast<const T&>(*this), output);
    /*else if (flags != SerializationFlags::CheckOfCyclicReferences)
        return serializeThis(static_cast<const T&>(*this), output, flags);*/
    else
    {
        return 0;
    }
}

template<typename T>
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr int ISerializable<T>::deserialize(const D& input)
{
    uint32_t versionAndFlags = 0;
    input.readArithmeticValue(versionAndFlags);
    if ((versionAndFlags & 0xff) != kSerializationProtocolVersion)
        return 1;

    SerializationFlags flags(versionAndFlags >> 8);

    uint32_t inputInterfaceVersion = 0;
    input.readArithmeticValue(inputInterfaceVersion);
    if (getInterfaceVersion() < inputInterfaceVersion)
        return 1;
    else if (getInterfaceVersion() > inputInterfaceVersion)
        flags.interfaceVersionsNotMatch = true;

    return deserializeNext(input, flags);
}

template<typename T>
template<serializable_concepts::IDeserializationCapableContainer D>
constexpr int ISerializable<T>::deserializeNext(const D& input, SerializationFlags flags)
{
    uint64_t inputNameHash = 0;
    input.readArithmeticValue(inputNameHash);
    if (getNameHash() != inputNameHash)
        return 1;

    if (!flags)
        return deserializeThis(input, static_cast<T&>(*this));
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

} // namespace common_serialization
