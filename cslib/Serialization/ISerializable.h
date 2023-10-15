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
#include "CspBase.h"
#include "CspLayers.h"

namespace common_serialization
{

// ISerializable base must have permanent size regardless of platform alignment
#pragma pack(push, 1)

template<typename T>
class ISerializable : public CspBase
{
public:
    using instance_type = T;


    // first function to call on new serialize operation
    template<serialization_concepts::ISerializationCapableContainer S>
    constexpr Status serialize(S& output) const noexcept;
    // first function to call on new serialize operation
    template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    constexpr Status serialize(CspContextSerializeData<S, PM>& context) const noexcept;

    // calling in subsequent serializations of nested fields/types or if one need to serialize another struct
    // and there must be no changes in header relative fields after previous serialize call
    template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
    constexpr Status serializeMessageData(CspContextSerializeData<S, PM>& context) const noexcept;

    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserialize(D& input);

    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    constexpr Status deserialize(CspContextDeserializeData<D, PM>& context, CspLayers maxLayerToProcess);

    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
    constexpr Status deserializeMessageData(CspContextDeserializeData<D, PM>& context, CspLayers maxLayerToProcess);
    
    template<serialization_concepts::IDeserializationCapableContainer D>
    constexpr Status deserializeDataCompat(D& input, CspFlags flags, uint8_t foreignProtocolVersion, uint32_t minimumSupportedInterfaceVersion);
    
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
constexpr Status ISerializable<T>::serialize(S& output) const noexcept
{
    CspContextSerializeData<S> context(output);

    return serialize(context);
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status ISerializable<T>::serialize(CspContextSerializeData<S, PM>& context) const noexcept
{
    RUN(serializeHeader(context.getOutput(), static_cast<const CspContextHeader&>(context.getExtension())));

    return serializeData(context);
}

template<typename T>
template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
constexpr Status ISerializable<T>::serializeMessageData(CspContextSerializeData<S, PM>& context) const noexcept
{
    S& output = context.getOutput();

    if (CspContextDataExtension<PM>& contextExtension = context.getExtension(); contextExtension)
    {
        const CspFlags& flags = contextExtension.getFlags();

        if (flags.interfaceVersionsNotMatch
            && isInterfaceVersionSupported(contextExtension.getInterfaceVersion()
                , getVersionsHierarchy()[getVersionsHierarchySize() - 1].thisVersion, getVersionsHierarchy()[0].thisVersion))
        {
            return Status::kErrorNotSupportedSerializationInterfaceVersion;
        }

        if (flags.extendedPointersProcessing && contextExtension.getPointersMap() == nullptr)
            return Status::kErrorInvalidArgument;
    }

    RUN(output.pushBackArithmeticValue(getInterfaceVersion()));
    RUN(output.pushBackArithmeticValue(getNameHash()));

    return serializeDataHelper(static_cast<const T&>(*this), context);
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserialize(D& input)
{
    CspContextDeserializeData<D> context(input);

    return deserialize(context)
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status ISerializable<T>::deserialize(CspContextDeserializeData<D, PM>& context, CspLayers maxLayerToProcess)
{
    CspContextDataExtension& extension = context.getExtension();

    uint8_t minimalProtocolVersion = extension.getProtocolVersion();

    RUN(deserializeHeader(context.getInput(), static_cast<const CspContextHeader&>(extension)));

    if (minimalProtocolVersion > extension.getProtocolVersion())
        return Status::kErrorNotSupportedSerializationProtocolVersion;
    else if (!isProtocolVersionSameAsLatestOur(extension.getProtocolVersion))
        extension.getFlags().protocolVersionsNotMatch = true;

    if (maxLayerToProcess == CspLayers::kHeader)
        return Status::kNoError;

    if (messageType == CspMessageType::kData)
        return deserializeData(input, context);
    else
        return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
constexpr Status ISerializable<T>::deserializeMessageData(CspContextDeserializeData<D, PM>& context, CspLayers maxLayerToProcess)
{
    CspContextDataExtension& extension = context.getExtension();
    const CspFlags& flags = extension.getFlags();

    uint64_t nameHash = 0;
    RUN(input.readArithmeticValue(nameHash));

    if (getNameHash() != nameHash)
        return Status::kErrorMismatchOfStructNameHash;

    uint32_t minimalInterfaceVersion = extension.getInterfaceVersion();
    uint32_t inputInterfaceVersion = 0;

    RUN(input.readArithmeticValue(inputInterfaceVersion));

    extension.setInterfaceVersion(inputInterfaceVersion);

    if (!isInterfaceVersionSupported(extension.getInterfaceVersion(), minimalInterfaceVersion, getInterfaceVersion()))
        return Status::kErrorNotSupportedSerializationInterfaceVersion;
    else if (extension.getInterfaceVersion() != getInterfaceVersion() && !flags.interfaceVersionsNotMatch)
        return Status::kErrorMismatchOfSerializationInterfaceVersions;
            
    

    if (!flags)
        return SerializationProcessor::deserializeData(input, static_cast<T&>(*this));

    // temporary dummy
    return Status::kErrorInvalidArgument;
}

template<typename T>
template<serialization_concepts::IDeserializationCapableContainer D>
constexpr Status ISerializable<T>::deserializeDataCompat(D& input, CspFlags flags, uint8_t foreignProtocolVersion, uint32_t minimumSupportedInterfaceVersion)
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
    return T::kVersionsHierarchy[0].thisVersion;
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
