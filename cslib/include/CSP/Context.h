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
#include "../Containers/ContainersConcepts.h"
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

class InterfaceVersionsProcessing
{
public:
    InterfaceVersionsProcessing() {}
    InterfaceVersionsProcessing(const InterfaceVersionsProcessing& rhs) 
        : m_auxUsingHeapAllocation(rhs.m_auxUsingHeapAllocation), m_interfaceVersion(rhs.m_interfaceVersion), m_interfaceVersionsNotMatch(rhs.m_interfaceVersionsNotMatch)
    { }
    InterfaceVersionsProcessing(bool auxUsingHeapAllocation, uint32_t interfaceVersion, bool interfaceVersionsNotMatch = false)
        : m_auxUsingHeapAllocation(auxUsingHeapAllocation), m_interfaceVersion(interfaceVersion), m_interfaceVersionsNotMatch(interfaceVersionsNotMatch)
    { }

    [[nodiscard]] constexpr bool isAuxUsingHeapAllocation() const noexcept { return m_auxUsingHeapAllocation; }
    constexpr void setAuxUsingHeapAllocation(bool auxUsingHeapAllocation) { m_auxUsingHeapAllocation = auxUsingHeapAllocation; }

    [[nodiscard]] constexpr uint32_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(uint32_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    [[nodiscard]] constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsNotMatch; }
    constexpr void setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsNotMatch = interfaceVersionsNotMatch; }

    void clear()
    {
        m_interfaceVersion = traits::kInterfaceVersionMax;
        m_interfaceVersionsNotMatch = false;
        // m_auxUsingHeapAllocation is not resets to false, cause it's rather environment tool option instead of struct/operation specific
    }

private:
    uint32_t m_interfaceVersion{ traits::kInterfaceVersionMax };
    bool m_interfaceVersionsNotMatch = false;
    bool m_auxUsingHeapAllocation = false;  // flag indicates that auxillary created structs in conversion functions must be placed in heap instead of stack
};

// Because of std::conditional_t evaluation traits in instantiation of template Data
// we cannot use more restricted concept here (ISerializationPointersMap)
// Note that you shall manualy clear map container after use (you can use clear method if you wish)
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

    void release()
    {
        m_pPointersMap = nullptr;
    }

private:
    PM* m_pPointersMap{ nullptr };
};

// Because of std::conditional_t evaluation traits in instantiation of template Data
// we cannot use more restricted concept here (IDeserializationPointersMap).
// Note that you shall manualy clear all containers after use (you can use clear method if you wish)
template<IGenericPointersKeeperContainer PC, serialization_concepts::IPointersMap PM
    , template<typename> typename Allocator = ConstructorNoexceptAllocator, template<typename, typename> typename AllocatorHelper = GenericAllocatorHelper>
class DeserializeExtendedPointersProcessing
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
        requires IConstructorAllocator<Allocator<T>>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept
    {
        if (!m_pAddedPointers)
            assert(false); // this situation shall never exists

        GenericPointerKeeper pointerKeeper;
        T* p = pointerKeeper.allocateAndConstruct<T, AllocatorHelper<T, Allocator<T>>>(1);

        if (p)
            if (!statusSuccess(m_pAddedPointers->pushBack(std::move(pointerKeeper))))
                p = nullptr;

        return p;
    }

    void clear()
    {
        if (m_pAddedPointers)
            m_pAddedPointers->clear();

        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

    void release()
    {
        m_pAddedPointers = nullptr;
        m_pPointersMap = nullptr;
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
        , IGenericPointersKeeperContainer PC = Vector<GenericPointerKeeper>
        , typename EPP = std::conditional_t<serialize, SerializeExtendedPointersProcessing<PM>, DeserializeExtendedPointersProcessing<PC, PM>>
>
    requires   serialization_concepts::ISerializationCapableContainer<Container>
            || serialization_concepts::IDeserializationCapableContainer<Container>
class Data : public Common<Container>
{
public:
    constexpr Data(Container& container, bool auxUsingHeapAllocation = true, uint32_t interfaceVersion = traits::kInterfaceVersionMax) noexcept
        : Common<Container>(container, traits::getLatestProtocolVersion(), Message::kData)
        , m_interfaceVersionsProcessing(auxUsingHeapAllocation, interfaceVersion)
    { }

    constexpr Data(const Common<Container>& common, bool auxUsingHeapAllocation = true, uint32_t interfaceVersion = traits::kInterfaceVersionMax) noexcept
        : Common<Container>(common.getBinaryData(), common.getProtocolVersion(), Message::kData)
        , m_interfaceVersionsProcessing(auxUsingHeapAllocation, interfaceVersion)
    { }

    constexpr Data(const Data<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Common<Container>(rhs.getBinaryData(), rhs.getProtocolVersion(), rhs.getFlags(), Message::kData)
        , m_flags(rhs.m_flags), m_interfaceVersionsProcessing(rhs.m_interfaceVersionsProcessing), m_epp(rhs.m_epp)
    { }

    constexpr Data(Container& binaryData, uint8_t protocolVersion, DataFlags flags, bool auxUsingHeapAllocation = false
        , uint32_t interfaceVersion = traits::kInterfaceVersionMax, PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
    : Common<Container>(binaryData, protocolVersion, flags, Message::kData), m_flags(flags)
        , m_interfaceVersionsProcessing(auxUsingHeapAllocation, interfaceVersion), m_epp(pPointersMap)
    { }

    constexpr Data(Container& binaryData, uint8_t protocolVersion, DataFlags flags, bool auxUsingHeapAllocation = false
        , uint32_t interfaceVersion = traits::kInterfaceVersionMax, PC* pPointersContainer = nullptr, PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
            : Common<Container>(binaryData, protocolVersion, flags, Message::kData), m_flags(flags)
            , m_interfaceVersionsProcessing(auxUsingHeapAllocation, interfaceVersion), m_epp(pPointersContainer, pPointersMap)
    { }

    [[nodiscard]] constexpr DataFlags getFlags() noexcept { return m_flags; }
    constexpr void setFlags(DataFlags flags) { m_flags = flags; }

    template<typename Flag>
    constexpr void setFlag(Flag flag, bool value) { m_flags.*flag = value; }

    [[nodiscard]] constexpr uint32_t getInterfaceVersion() const noexcept { return m_interfaceVersionsProcessing.getInterfaceVersion(); }
    constexpr void setInterfaceVersion(uint32_t interfaceVersion) { m_interfaceVersionsProcessing.setInterfaceVersion(interfaceVersion); }

    [[nodiscard]] constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsProcessing.isInterfaceVersionsNotMatch(); }
    constexpr void setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsProcessing.setInterfaceVersionsNotMatch(interfaceVersionsNotMatch); }

    [[nodiscard]] constexpr bool isAuxUsingHeapAllocation() const noexcept { return m_interfaceVersionsProcessing.isAuxUsingHeapAllocation(); }
    constexpr void setAuxUsingHeapAllocation(bool auxUsingHeapAllocation) { m_interfaceVersionsProcessing.setAuxUsingHeapAllocation(auxUsingHeapAllocation); }

    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_epp.getPointersMap(); }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_epp.getPointersMap(); }
    constexpr void setPointersMap(PM& pointersMap) noexcept { m_epp.setPointersMap(pointersMap); }

    [[nodiscard]] constexpr PC* getAddedPointers() noexcept requires !serialize { return m_epp.getAddedPointers(); }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept requires !serialize { return m_epp.getAddedPointers(); }
    constexpr void setAddedPointers(PC& addedPointers) noexcept requires !serialize { m_epp.setAddedPointers(addedPointers); }

    template<typename T>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept requires !serialize { return m_epp.allocateAndDefaultConstruct<T>(); }

    void resetToDefaultsExceptDataContents() noexcept
    {
        Common<Container>::resetToDefaultsExceptDataContents();
        m_flags = DataFlags{};
        m_interfaceVersionsProcessing.clear();
        m_epp.clear();
    }

    void clear() noexcept
    {
        Common<Container>::clear();
        m_flags = DataFlags{};
        m_interfaceVersionsProcessing.clear();
        m_epp.clear();
    }

private:
    DataFlags m_flags = DataFlags{};
    InterfaceVersionsProcessing m_interfaceVersionsProcessing;
    EPP  m_epp;
};

template<serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SData = Data<S, true, PM>;

template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DData = Data<D, false, PM>;

} // namespace context

} // namespace csp

} // namespace common_serialization