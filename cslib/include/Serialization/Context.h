/**
 * @file Context.h
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

#include "Traits.h"
#include "ContextDataFlags.h"
#include "ContextMessage.h"
#include "SerializationConcepts.h"
#include "../Containers/HashMap.h"

namespace common_serialization
{

namespace csp
{

namespace context
{

template<typename Container>
    requires   serialization_concepts::ISerializationCapableContainer<Container>
            || serialization_concepts::IDeserializationCapableContainer<Container>
class Common
{
protected:
    constexpr Common(Container& binaryData, uint8_t protocolVersion, Message messageType) noexcept
        : m_binaryData(binaryData), m_protocolVersion(protocolVersion)
        , m_protocolVersionsNotMatch(traits::getLatestProtocolVersion() != protocolVersion), m_messageType(messageType)
    {
        if constexpr (serialization_concepts::ISerializationCapableContainer<Container>)
            m_binaryData.reserve(256);
    }

public:
    [[nodiscard]] constexpr Container& getBinaryData() noexcept { return m_binaryData; }
    [[nodiscard]] constexpr const Container& getBinaryData() const noexcept { return m_binaryData; }

    [[nodiscard]] constexpr Message getMessageType() const noexcept { return m_messageType; }
    constexpr void setMessageType(Message messageType) { m_messageType = messageType; }

    [[nodiscard]] constexpr uint8_t getProtocolVersion() const noexcept { return m_protocolVersion; }
    constexpr void setProtocolVersion(uint8_t protocolVersion)
    { 
        m_protocolVersion = protocolVersion; 
        m_protocolVersionsNotMatch = traits::isProtocolVersionSameAsLatestOur(m_protocolVersion);
    }

    void resetToDefaultsExceptDataContents() noexcept
    {
        if constexpr (serialization_concepts::IDeserializationCapableContainer<Container>)
            m_binaryData.seek(0);
        m_protocolVersion = traits::getLatestProtocolVersion();
        m_messageType = Message::kData;
    }

    void clear() noexcept
    {
        resetToDefaultsExceptDataContents();
        m_binaryData.clear();
    }

private:
    Container& m_binaryData;
    uint8_t m_protocolVersion{ traits::getLatestProtocolVersion() };
    bool m_protocolVersionsNotMatch = false;
    Message m_messageType = Message::kData;
};

// Because of std::conditional_t evaluation traits in instantiation of template Data
// we cannot use more restricted concept here (ISerializationPointersMap)
template<serialization_concepts::IPointersMap PM>
class SerializeExtendedPointersProcessing
{
public:
    SerializeExtendedPointersProcessing()
        : m_pPointersMap(nullptr)
    { }

    SerializeExtendedPointersProcessing(PM* pointersMap)
        : m_pPointersMap(pointersMap)
    { }

    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }
    constexpr void setPointersMap(PM& pointersMap) noexcept { m_pPointersMap = &pointersMap; }

    void clear()
    {
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    PM* m_pPointersMap{ nullptr };
};

// Because of std::conditional_t evaluation traits in instantiation of template Data
// we cannot use more restricted concept here (IDeserializationPointersMap)
template<serialization_concepts::IPointersContainer PC, serialization_concepts::IPointersMap PM
    , template<typename> typename Allocator = ConstructorNoexceptAllocator, template<typename, typename> typename AllocatorHelper = GenericAllocatorHelper>
struct DeserializeExtendedPointersProcessing
{
public:
    DeserializeExtendedPointersProcessing()
        : m_pAddedPointers(nullptr), m_pPointersMap(nullptr)
    { }

    DeserializeExtendedPointersProcessing(PC* addedPonters, PM* pointersMap)
        : m_pAddedPointers(addedPonters), m_pPointersMap(pointersMap)
    { }

    [[nodiscard]] constexpr PC* getAddedPointers() noexcept { return m_pAddedPointers; }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept { return m_pAddedPointers; }
    constexpr void setAddedPointers(PC& addedPointers) noexcept { m_pAddedPointers = &addedPointers; }

    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }
    constexpr void setPointersMap(PM& pointersMap) noexcept { m_pPointersMap = &pointersMap; }
    
    // allocates memory for type T and costructs default object
    // only stateless AllocatorHelpers are allowed
    template<typename T>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept
    {
        if (!m_pAddedPointers)
            assert(false); // this situation shall never exists

        AllocatorHelper<T, Allocator<T>> allocatorHelper{};
        T* p = allocatorHelper.allocateAndConstruct(1, nullptr);
        if (p)
            m_pAddedPointers->pushBack(
                { p, reinterpret_cast<typename PointerAndDestructorDeallocator::DestroyAndDeallocateFunc>(&destroyAndDeallocatePointer<T>) }
            );

        return p;
    }

    void destroyAndDeallocateAddedPointer(size_t i) noexcept
    {
        if (m_pAddedPointers && i < (*m_pAddedPointers).size()) // we not assert here, cause this method should be used by users code
        {
            PointerAndDestructorDeallocator padd = (*m_pAddedPointers)[i];
            padd.destructorAndDeallocatorFunction(padd.pointer);
        }
    }
    
    void destroyAndDeallocateAllAddedPointers() noexcept
    {
        if (m_pAddedPointers) // we not assert here, cause this method should be used by users code
            for (auto& pointerAndDestructorDeallocator : *m_pAddedPointers)
                pointerAndDestructorDeallocator.destructorAndDeallocatorFunction(pointerAndDestructorDeallocator.pointer);
    }

    void clear()
    {
        if (m_pAddedPointers)
            m_pAddedPointers->clear();

        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

    ~DeserializeExtendedPointersProcessing()
    {
        if (m_pAddedPointers)
            destroyAndDeallocateAllAddedPointers();
    }

private:
    template<typename T>
    static void destroyAndDeallocatePointer(T* p)
    {
        AllocatorHelper<T, Allocator<T>>().destroyAndDeallocate(p, 1);
    }

    PC* m_pAddedPointers{ nullptr };
    PM* m_pPointersMap{ nullptr };
};

// unordered_map shall be replaced by common_serialization::HashMap when it would be ready
template< 
          typename Container
        , bool serialize = true
        , serialization_concepts::IPointersMap PM = std::conditional_t<serialize, std::unordered_map<const void*, uint64_t>, std::unordered_map<uint64_t, void*>>
        , serialization_concepts::IPointersContainer PC = Vector<PointerAndDestructorDeallocator>
        , typename EPP = std::conditional_t<serialize, SerializeExtendedPointersProcessing<PM>, DeserializeExtendedPointersProcessing<PC, PM>>
>
    requires   serialization_concepts::ISerializationCapableContainer<Container>
            || serialization_concepts::IDeserializationCapableContainer<Container>
class Data : public Common<Container>
{
public:
    constexpr Data(Container& container, uint32_t interfaceVersion = traits::kInterfaceVersionMax) noexcept
        : Common<Container>(container, traits::getLatestProtocolVersion(), Message::kData)
        , m_interfaceVersion(interfaceVersion)
    { }

    constexpr Data(const Common<Container>& common, uint32_t interfaceVersion = traits::kInterfaceVersionMax) noexcept
        : Common<Container>(common.getBinaryData(), common.getProtocolVersion(), Message::kData)
        , m_interfaceVersion(interfaceVersion)
    { }

    constexpr Data(const Data<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Common<Container>(rhs.getBinaryData(), rhs.getProtocolVersion(), rhs.getFlags(), Message::kData)
        , m_flags(rhs.m_flags), m_interfaceVersion(rhs.m_interfaceVersion), m_epp(rhs.m_epp)
    { }

    constexpr Data(Container& binaryData, uint8_t protocolVersion, DataFlags flags, uint32_t interfaceVersion = traits::kInterfaceVersionMax
        , PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
    : Common<Container>(binaryData, protocolVersion, flags, Message::kData), m_flags(flags)
        , m_interfaceVersion(interfaceVersion), m_interfaceVersionsNotMatch(false), m_epp(pPointersMap)
    { }

    constexpr Data(Container& binaryData, uint8_t protocolVersion, DataFlags flags, uint32_t interfaceVersion = traits::kInterfaceVersionMax
        , PC* pPointersContainer = nullptr, PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
            : Common<Container>(binaryData, protocolVersion, flags, Message::kData), m_flags(flags)
            , m_interfaceVersion(interfaceVersion), m_interfaceVersionsNotMatch(false), m_epp(pPointersContainer, pPointersMap)
    { }

    [[nodiscard]] constexpr DataFlags getFlags() noexcept { return m_flags; }
    constexpr void setFlags(DataFlags flags) { m_flags = flags; }

    template<typename Flag>
    constexpr void setFlag(Flag flag, bool value) { m_flags.*flag = value; }

    [[nodiscard]] constexpr uint32_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(uint32_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    [[nodiscard]] constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsNotMatch; }
    constexpr void setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsNotMatch = interfaceVersionsNotMatch; }

    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_epp.getPointersMap(); }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_epp.getPointersMap(); }
    constexpr void setPointersMap(PM& pointersMap) noexcept { m_epp.setPointersMap(pointersMap); }

    [[nodiscard]] constexpr PC* getAddedPointers() noexcept requires !serialize { return m_epp.getAddedPointers(); }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept requires !serialize { return m_epp.getAddedPointers(); }
    constexpr void setAddedPointers(PC& addedPointers) noexcept requires !serialize { m_epp.setAddedPointers(addedPointers); }

    template<typename T>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept requires !serialize { return m_epp.allocateAndDefaultConstruct<T>(); }
    void destroyAndDeallocateAllAddedPointers() noexcept requires !serialize { return m_epp.destroyAndDeallocateAllAddedPointers(); }
    void destroyAndDeallocateAddedPointer(size_t i) noexcept requires !serialize{ return m_epp.destroyAndDeallocateAddedPointer(i); }

    void resetToDefaultsExceptDataContents() noexcept
    {
        Common<Container>::resetToDefaultsExceptDataContents();
        m_flags = DataFlags{};
        m_interfaceVersion = traits::kInterfaceVersionMax;
        m_interfaceVersionsNotMatch = false;
        m_epp.clear();
    }

    void clear() noexcept
    {
        Common<Container>::clear();
        m_flags = DataFlags{};
        m_interfaceVersion = traits::kInterfaceVersionMax;
        m_interfaceVersionsNotMatch = false;
        m_epp.clear();
    }

private:
    DataFlags m_flags = DataFlags{};
    uint32_t m_interfaceVersion{ traits::kInterfaceVersionMax };
    bool m_interfaceVersionsNotMatch = false;
    EPP  m_epp;
};

template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SData = Data<S, true, PM>;

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DData = Data<D, false, PM>;

} // namespace context

} // namespace csp

} // namespace common_serialization
