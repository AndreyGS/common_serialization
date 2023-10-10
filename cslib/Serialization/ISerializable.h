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

#include "SerializationProcessor.h"
#include "CsProtocol.h"

namespace common_serialization
{

// ISerializable base must have permanent size regardless of platform alignment
#pragma pack(push, 1)

template<typename T>
class ISerializable : public CsProtocol
{
public:
    using instance_type = T;

    // first function to call on new serialize operation
    template<serialization_concepts::ISerializationCapableContainer S>
    constexpr Status serialize(S& output, CsProtocolFlags flags = CsProtocolFlags{}, CsProtocolMessageType messageType = CsProtocolMessageType::kData) const noexcept;
    // first function to call on new serialize operation
    template<serialization_concepts::ISerializationCapableContainer S>
    constexpr Status serializeCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat, uint32_t interfaceVersionCompat
        , CsProtocolMessageType messageType = CsProtocolMessageType::kData) const noexcept;

    // calling in subsequent serializations of nested fields/types or if one need to serialize another struct
    // and there is no changes in interface version after previous serialize call
    template<serialization_concepts::ISerializationCapableContainer S>
    constexpr Status serializeData(S& output, CsProtocolFlags flags = CsProtocolFlags{}) const noexcept;
    // IPointersMap default type is temporary - later there will be common_serialization map container
    template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM = std::unordered_map<const void*, size_t>>
    constexpr Status serializeDataCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat, uint32_t interfaceVersionCompat) const noexcept;

    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserialize(D& input);
    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserializeCompat(D& input, uint8_t minimumSupportedProtocolVersion, uint32_t minimumSupportedInterfaceVersion);

    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserializeData(D& input, CsProtocolFlags flags);
    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserializeDataCompat(D& input, CsProtocolFlags flags, uint8_t foreignProtocolVersion, uint32_t minimumSupportedInterfaceVersion);
    
    [[nodiscard]] static constexpr uint64_t* getMembers() noexcept;
    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getThisVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getInterfaceVersion() noexcept;
    [[nodiscard]] static constexpr const StructNameHashAndVersion* getVersionsHierarchy() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionsHierarchySize() noexcept;
};

#pragma pack(pop)

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status ISerializable<T>::serialize(S& output, CsProtocolFlags flags, CsProtocolMessageType messageType) const noexcept
{
    RUN(serializeHeader(output, flags, messageType));

    if (messageType == CsProtocolMessageType::kData)
        return serializeData(output, flags);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status ISerializable<T>::serializeCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat
    , uint32_t interfaceVersionCompat, CsProtocolMessageType messageType) const noexcept
{
    RUN(serializeHeaderCompat(output, flags, protocolVersionCompat, messageType));

    if (messageType == CsProtocolMessageType::kData)
        return serializeDataCompat(output, flags, protocolVersionCompat, interfaceVersionCompat);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S>
constexpr Status ISerializable<T>::serializeData(S& output, CsProtocolFlags flags) const noexcept
{
    RUN(output.pushBackArithmeticValue(getInterfaceVersion()));
    RUN(output.pushBackArithmeticValue(getNameHash()));

    if (!flags)
        return SerializationProcessor::serializeData(static_cast<const T&>(*this), output);
    /*else if (!flags.extendedPointerProcessing)
        return SerializationProcessor::serializeData(static_cast<const T&>(*this), flags, output);*/
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status ISerializable<T>::serializeDataCompat(S& output, CsProtocolFlags flags, uint32_t protocolVersionCompat, uint32_t interfaceVersionCompat) const noexcept
{
    if (interfaceVersionCompat > getInterfaceVersion() || interfaceVersionCompat < getVersionsHierarchy()[getVersionsHierarchySize()-1].thisVersion)
        return Status::kErrorNotSupportedSerializationInterfaceVersion;
    else if (interfaceVersionCompat != getInterfaceVersion() && !flags.interfaceVersionsNotMatch)
        return Status::kErrorInvalidArgument;

    RUN(output.pushBackArithmeticValue(getInterfaceVersion()));
    RUN(output.pushBackArithmeticValue(getNameHash()));

    PM pointersMap;

    return SerializationProcessor::serializeDataCompat(static_cast<const T&>(*this), flags, protocolVersionCompat, interfaceVersionCompat, pointersMap, output);
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserialize(D& input)
{
    CsProtocolFlags flags{ 0 };
    CsProtocolMessageType messageType{ 0 };

    RUN(deserializeHeader(input, flags, messageType));

    if (messageType == CsProtocolMessageType::kData)
        return deserializeData(input, flags);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserializeCompat(D& input, uint8_t minimumSupportedProtocolVersion, uint32_t minimumSupportedInterfaceVersion)
{
    uint8_t foreignProtocolVersion{ 0 };
    CsProtocolFlags flags{ 0 };
    CsProtocolMessageType messageType{ 0 };

    RUN(deserializeHeaderCompat(input, foreignProtocolVersion, flags, messageType));

    if (messageType == CsProtocolMessageType::kData)
        return SerializationProcessor::deserializeDataCompat(input, flags, foreignProtocolVersion, minimumSupportedInterfaceVersion);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserializeData(D& input, CsProtocolFlags flags)
{
    uint32_t inputInterfaceVersion = 0;
    RUN(input.readArithmeticValue(inputInterfaceVersion));

    if (getInterfaceVersion() != inputInterfaceVersion)
        return Status::kErrorMismatchOfSerializationInterfaceVersions;

    uint64_t nameHash = 0;
    RUN(input.readArithmeticValue(nameHash));

    if (getNameHash() != nameHash)
        return Status::kErrorMismatchOfStructNameHash;

    if (!flags)
        return SerializationProcessor::deserializeData(input, static_cast<T&>(*this));

    // temporary dummy
    return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserializeDataCompat(D& input, CsProtocolFlags flags, uint8_t foreignProtocolVersion, uint32_t minimumSupportedInterfaceVersion)
{
    uint32_t foreignInterfaceVersion = 0;
    RUN(input.readArithmeticValue(foreignInterfaceVersion));

    if (getInterfaceVersion() != foreignInterfaceVersion)
    {
        if (!isInterfaceVersionCompat(getInterfaceVersion(), minimumSupportedInterfaceVersion, foreignInterfaceVersion))
            return Status::kErrorNotSupportedSerializationInterfaceVersion;

        if (!flags.interfaceVersionsNotMatch)
            return Status::kErrorMismatchOfSerializationInterfaceVersions;

        return SerializationProcessor::deserializeDataCompat(input, flags, foreignProtocolVersion, foreignInterfaceVersion, static_cast<T&>(*this));
    }

    /*
    if (getInterfaceVersion() < inputInterfaceVersion)
        return Status::kErrorNotSupportedSerializationInterfaceVersion;
    else if (getInterfaceVersion() > inputInterfaceVersion)
        flags.interfaceVersionsNotMatch = true;*/

    if (!flags)
        return SerializationProcessor::deserializeData(input, static_cast<T&>(*this));

    // temporary dummy
    return Status::kErrorInvalidArgument;
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
    return T::kThisVersion;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getInterfaceVersion() noexcept
{
    return T::kInterfaceVersion;
}

template<typename T>
[[nodiscard]] constexpr const StructNameHashAndVersion* ISerializable<T>::getVersionsHierarchy() noexcept
{
    return T::kVersionsHierarchy;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getVersionsHierarchySize() noexcept
{
    return std::size(T::kVersionsHierarchy);
}

} // namespace common_serialization
