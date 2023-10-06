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

namespace common_serialization
{

enum class SerializableMessageType : uint_fast32_t
{
    kData = 0x0,                            // default message type
                                            //
                                            // message body format:
                                            //
                                            // {
                                            //     uint32_t interfaceVersion;
                                            //     uint64_t structNameHash;
                                            //     uint8_t serializedData[anysize]; // rest of the data varies by SerializationFlags that was set
                                            //                                      // and data that was serialized
                                            // }
                                            //
    kNotSupportedProtocol = 0x1,            // sends when serialization protocol is not supported
                                            //
                                            // message body format:
                                            //
                                            // {
                                            //     uint8_t supportedProtocolsVersionsSize;
                                            //     uint8_t supportedProtocolsVersions[supportedProtocolsVersionsSize];
                                            // }
                                            //
    kNotSupportedInterfaceVersion = 0x2,    // sends when low supported version is bigger or
                                            // high version is lesser than in serialized data
                                            // 
                                            // message body format:
                                            // 
                                            // {
                                            //     uint64_t structNameHash;
                                            //     uint32_t minimumSupportedInterfaceVersion;
                                            //     uint32_t maximalSupportedInterfaceVersion;
                                            // }
                                            // 
    kMismatchOfInterfaceVersions = 0x2,     // sends when its primal (latest) version not
                                            // equal to version of input data, but version of input data
                                            // is supported and SerializationFlags::interfaceVersionsNotMatch is false
                                            // 
                                            // message body format
                                            // 
                                            // {
                                            //     uint64_t structHashName;
                                            // }
                                            //                                      
};

struct StructNameHashAndVersion
{
    uint64_t nameHash{ 0 };
    uint32_t versionThis{ 0 };
};

// Do not add here any member fields or methods
class SerializableVersionsTraits
{
public:
    static constexpr uint8_t kSupportedProtocolVersionsSize = 1;
    static constexpr uint8_t kSupportedProtocolVersions[kSupportedProtocolVersionsSize] = { 1 };
    static constexpr uint8_t kProtocolVersionMax = 0xff;
    static constexpr uint32_t kInterfaceVersionMax = 0xffffffff;

    static constexpr bool isProtocolVersionSameAsLatestOur(uint8_t foreignProtocolVersion)
    {
        return kSupportedProtocolVersions[0] == foreignProtocolVersion;
    }

    static constexpr bool isProtocolVersionSupported(uint8_t foreignProtocolVersion) noexcept
    {
        for (uint8_t i = 0; i < kProtocolVersionMax; ++i)
            if (foreignProtocolVersion == kSupportedProtocolVersions[i])
                return true;

        return false;
    }

    static constexpr bool isInterfaceVersionCompatible(uint32_t ourInterfaceVersion
        , uint32_t minimumSupportedInterfaceVersion, uint32_t foreignInterfaceVersion) noexcept
    {
        return foreignInterfaceVersion >= minimumSupportedInterfaceVersion && foreignInterfaceVersion <= ourInterfaceVersion;
    }
};


// ISerializable base must have permanent size regardless of platform alignment
#pragma pack(push, 1)

template<typename T>
class ISerializable : public SerializableVersionsTraits
{
public:
    using instance_type = T;

    static constexpr Status serializeHeader(Vector<uint8_t>& output, SerializationFlags flags = SerializationFlags{}, SerializableMessageType messageType = SerializableMessageType::kData) noexcept;

    // first function to call on new serialize operation
    constexpr Status serialize(Vector<uint8_t>& output, SerializationFlags flags = SerializationFlags{}, SerializableMessageType messageType = SerializableMessageType::kData) const noexcept;

    // first function to call on new serialize operation
    //constexpr Status serializeCompat(uint32_t compatVersionProtocol, uint32_t compatVersionInterface, Vector<uint8_t>& output, SerializationFlags flags) const noexcept;

    // calling in subsequent serializations of nested fields/types or if one need to serialize another struct
    // and there is no changes in interface version after previous serialize call
    constexpr Status serializeData(Vector<uint8_t>& output, SerializationFlags flags = SerializationFlags{}) const noexcept;

    static constexpr Status deserializeHeader(Walker<uint8_t>& input, SerializationFlags& flags, SerializableMessageType& messageType) noexcept;
    static constexpr Status deserializeHeaderCompat(Walker<uint8_t>& input, uint8_t& foreignProtocolVersion, SerializationFlags& flags, SerializableMessageType& messageType) noexcept;

    constexpr Status deserialize(Walker<uint8_t>& input);
    constexpr Status deserializeCompat(Walker<uint8_t>& input, uint8_t minimumSupportedProtocolVersion, uint32_t minimumSupportedInterfaceVersion);

    constexpr Status deserializeData(Walker<uint8_t>& input, SerializationFlags flags);
    constexpr Status deserializeDataCompat(Walker<uint8_t>& input, SerializationFlags flags, uint32_t minimumSupportedInterfaceVersion);
    
    [[nodiscard]] static constexpr uint64_t* getAncestors() noexcept;
    [[nodiscard]] static constexpr uint64_t* getMembers() noexcept;
    [[nodiscard]] static constexpr uint64_t getNameHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getThisVersion() noexcept;
    [[nodiscard]] static constexpr uint32_t getInterfaceVersion() noexcept;
    [[nodiscard]] static constexpr StructNameHashAndVersion* getVersionsHierarchy() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionsHierarchySize() noexcept;
};

#pragma pack(pop)

template<typename T>
constexpr Status ISerializable<T>::serializeHeader(Vector<uint8_t>& output, SerializationFlags flags, SerializableMessageType messageType) noexcept
{
    RUN(output.pushBackArithmeticValue(static_cast<uint32_t>(kSupportedProtocolVersions[0]) | (static_cast<uint32_t>(flags) << 8)));
    RUN(output.pushBackArithmeticValue(messageType));

    return Status::kNoError;
}

template<typename T>
constexpr Status ISerializable<T>::serialize(Vector<uint8_t>& output, SerializationFlags flags, SerializableMessageType messageType) const noexcept
{
    RUN(serializeHeader(output, flags, messageType));

    if (messageType == SerializableMessageType::kData)
        return serializeData(output, flags);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
constexpr Status ISerializable<T>::serializeData(Vector<uint8_t>& output, SerializationFlags flags) const noexcept
{
    RUN(output.pushBackArithmeticValue(getInterfaceVersion()));
    RUN(output.pushBackArithmeticValue(getNameHash()));

    if (!flags)
        return SerializationProcessor::serializeData(static_cast<const T&>(*this), output);
    /*else if (flags != SerializationFlags::CheckOfCyclicReferences)
        return serializeThis(static_cast<const T&>(*this), output, flags);*/
    else
    {
        return Status::kErrorInvalidArgument;
    }
}

template<typename T>
constexpr Status ISerializable<T>::deserializeHeader(Walker<uint8_t>& input, SerializationFlags& flags, SerializableMessageType& messageType) noexcept
{
    uint32_t versionAndFlags = 0;
    RUN(input.readArithmeticValue(versionAndFlags));

    uint8_t foreignProtocolVersion = versionAndFlags & 0xff;
    if (!isProtocolVersionSameAsLatestOur(foreignProtocolVersion))
        return Status::kErrorMismatchOfSerializationProtocolVersions;

    flags = versionAndFlags >> 8;
    RUN(input.readArithmeticValue(messageType));
    
    return Status::kNoError;
}

template<typename T>
constexpr Status ISerializable<T>::deserializeHeaderCompat(Walker<uint8_t>& input, uint8_t& foreignProtocolVersion, SerializationFlags& flags, SerializableMessageType& messageType) noexcept
{
    uint32_t versionAndFlags = 0;
    RUN(input.readArithmeticValue(versionAndFlags));

    foreignProtocolVersion = versionAndFlags & 0xff;
    if (!isProtocolVersionSameAsLatestOur(foreignProtocolVersion))
    {
        if (!isProtocolVersionSupported(foreignProtocolVersion))
            return Status::kErrorNotSupportedSerializationProtocolVersion;

        // here shall be processing for legacy protocol versions
    }
    else
    {
        flags = versionAndFlags >> 8;
        RUN(input.readArithmeticValue(messageType));
    }

    return Status::kNoError;
}

template<typename T>
constexpr Status ISerializable<T>::deserialize(Walker<uint8_t>& input)
{
    SerializationFlags flags{ 0 };
    SerializableMessageType messageType{ 0 };

    RUN(deserializeHeader(input, flags, messageType));

    if (messageType == SerializableMessageType::kData)
        return deserializeData(input, flags);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
constexpr Status ISerializable<T>::deserializeCompat(Walker<uint8_t>& input, uint8_t minimumSupportedProtocolVersion, uint32_t minimumSupportedInterfaceVersion)
{
    uint8_t foreignProtocolVersion{ 0 };
    SerializationFlags flags{ 0 };
    SerializableMessageType messageType{ 0 };

    RUN(deserializeHeaderCompat(input, foreignProtocolVersion, flags, messageType));

    if (!isProtocolVersionSameAsLatestOur(foreignProtocolVersion))
    {
        if (messageType == SerializableMessageType::kData)
            return SerializationProcessor::deserializeDataCompat(input, flags, minimumSupportedInterfaceVersion);
        else
            return Status::kErrorInvalidArgument;
    }
    else
    {
        // here shall be processing for legacy protocol versions
        return Status::kErrorInvalidArgument;
    }
}

template<typename T>
constexpr Status ISerializable<T>::deserializeData(Walker<uint8_t>& input, SerializationFlags flags)
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
constexpr Status ISerializable<T>::deserializeDataCompat(Walker<uint8_t>& input, SerializationFlags flags, uint32_t minimumSupportedInterfaceVersion)
{
    uint32_t inputInterfaceVersion = 0;
    RUN(input.readArithmeticValue(inputInterfaceVersion));

    if (getInterfaceVersion() != inputInterfaceVersion)
    {
        if (!isInterfaceVersionCompatible(getInterfaceVersion(), minimumSupportedInterfaceVersion, inputInterfaceVersion))
            return Status::kErrorNotSupportedSerializationInterfaceVersion;

        if (!flags.interfaceVersionsNotMatch)
            return Status::kErrorMismatchOfSerializationInterfaceVersions;
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
    return T::kThisVersion;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getInterfaceVersion() noexcept
{
    return T::kInterfaceVersion;
}

template<typename T>
[[nodiscard]] constexpr StructNameHashAndVersion* ISerializable<T>::getVersionsHierarchy() noexcept
{
    if constexpr (getVersionsHierarchySize() != 0)
        return T::kVersionsHierarchy;
    else
        return nullptr;
}

template<typename T>
[[nodiscard]] constexpr uint32_t ISerializable<T>::getVersionsHierarchySize() noexcept
{
    return T::kVersionsHierarchySize;
}

} // namespace common_serialization
