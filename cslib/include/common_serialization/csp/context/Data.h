/**
 * @file cslib/include/common_serialization/csp/context/Data.h
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

#include "common_serialization/csp/context/Common.h"
#include "common_serialization/csp/context/DataFlags.h"
#include "common_serialization/Containers/HashMap.h"

namespace common_serialization::csp::context
{

/// <summary>
/// Auxillary class for holding pointers map to prevent infinite recursive pointers serialization.
/// </summary>
/// <typeparam name="PM">
/// Map that implements ISerializationPointersMap interface
/// We cannot direct use more restricted concept here
/// because std::conditional_t evaluating traits in instantiation of template Data.
/// </typeparam>
template<IPointersMap PM>
class SerializeExtendedPointersProcessing
{
public:

    /// <summary>
    /// Default constructor
    /// </summary>
    SerializeExtendedPointersProcessing()
        : m_pPointersMap(nullptr)
    { }

    /// <summary>
    /// Constructor from IPointersMap
    /// </summary>
    /// <param name="pointersMap">Pointer to map that implements ISerializationPointersMap interface</param>
    SerializeExtendedPointersProcessing(PM* pPointersMap)
        : m_pPointersMap(pPointersMap)
    { }

    /// <summary>
    /// Get pointer to holding pointers map
    /// </summary>
    /// <returns>Pointer to pointers map</returns>
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }

    /// <summary>
    /// Set holding pointers map.
    /// You should notice that when we set map to SerializeExtendedPointersProcessing 
    /// it will not owns this map and does not clears its contents on destruction.
    /// </summary>
    /// <param name="pointersMap">Set pointer map to this</param>
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }

    /// <summary>
    /// Clears contents of holding pointers map
    /// </summary>
    void clear()
    {
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    PM* m_pPointersMap{ nullptr };
};

/// <summary>
/// Auxillary class for holding pointers containers using in some deserialization scenarios
/// </summary>
/// <typeparam name="PC">
/// Container that implements IGenericPointersKeeperContainer interface.
/// Using for keeping additional free pointers that creates deserialization process.
/// When pointers are no longer needed it should be freed.
/// </typeparam>
/// <typeparam name="PM">
/// Map that implements IDeserializationPointersMap interface
/// We cannot direct use more restricted concept here
/// because std::conditional_t evaluating traits in instantiation of template Data.
/// </typeparam>
/// <typeparam name="Allocator">
/// Stateless Allocator that would be used for free pointers allocation
/// (should be construction capabale)
/// </typeparam>
/// <typeparam name="AllocatorHelper">
/// Stateless AllocatorHelper that would be used for free pointers allocation help
/// (should be construction capabale)
/// </typeparam>
template<IGenericPointersKeeperContainer PC, IPointersMap PM
    , template<typename> typename Allocator = ConstructorNoexceptAllocator, template<typename, typename> typename AllocatorHelper = GenericAllocatorHelper>
class DeserializeExtendedPointersProcessing
{
public:

    /// <summary>
    /// Default constuctor
    /// </summary>
    DeserializeExtendedPointersProcessing()
        : m_pAddedPointers(nullptr), m_pPointersMap(nullptr)
    { }

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="pAddedPonters">Pointer to additional free pointers container</param>
    /// <param name="pPointersMap">Pointer to map that implements IDeserializationPointersMap interface</param>
    DeserializeExtendedPointersProcessing(PC* pAddedPonters, PM* pPointersMap)
        : m_pAddedPointers(pAddedPonters), m_pPointersMap(pPointersMap)
    { }

    /// <summary>
    /// Get pointer to added free pointers container
    /// </summary>
    /// <returns>Pointer to added free pointers container</returns>
    [[nodiscard]] constexpr PC* getAddedPointers() noexcept { return m_pAddedPointers; }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept { return m_pAddedPointers; }

    /// <summary>
    /// Set holding pointer to added pointers container
    /// You should notice that when we set map to DeserializeExtendedPointersProcessing 
    /// it will not owns this container and does not clears its contents on destruction.
    /// </summary>
    /// <param name="addedPointers">Pointer to added free pointers container<</param>
    constexpr void setAddedPointers(PC* pAddedPointers) noexcept { m_pAddedPointers = pAddedPointers; }

    /// <summary>
    /// Get pointer to holding pointers map
    /// </summary>
    /// <returns>Pointer to pointers map</returns>
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }

    /// <summary>
    /// Set holding pointers map.
    /// You should notice that when we set map to DeserializeExtendedPointersProcessing 
    /// it will not owns this map and does not clears its contents on destruction.
    /// </summary>
    /// <param name="pointersMap">Set pointer map to this</param>
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }
    
    /// <summary>
    /// Allocates memory for type T and costructs default T-object,
    /// and then places it to container of added free pointers.
    /// </summary>
    /// <typeparam name="T">Type of object to allocate and construct</typeparam>
    /// <returns>Pointer of costructed object</returns>
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

    /// <summary>
    /// Clears contents of holding containers
    /// </summary>
    void clear()
    {
        if (m_pAddedPointers)
            m_pAddedPointers->clear();

        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    PC* m_pAddedPointers{ nullptr };
    PM* m_pPointersMap{ nullptr };
};

// 

/// <summary>
/// Holder of context in processing message of type Data
/// </summary>
/// <typeparam name="Container">Container capable to serialization/deserialization</typeparam>
/// <typeparam name="serialize">Type of scenario (serialization|deserialization) for specialization</typeparam>
/// <typeparam name="PM">
/// Map that implements ISerializationPointersMap or IDeserializationPointersMap interface.
/// (std::unordered_map shall be replaced by common_serialization::HashMap when it would be ready)
/// </typeparam>
/// <typeparam name="PC">Container that implements IGenericPointersKeeperContainer interface</typeparam>
/// <typeparam name="EPP">SerializeExtendedPointersProcessing or DeserializeExtendedPointersProcessing (depends on serialize flag)</typeparam>
template< 
          typename Container
        , bool serialize = true
        , IPointersMap PM = std::conditional_t<serialize, std::unordered_map<const void*, uint64_t>, std::unordered_map<uint64_t, void*>>
        , IGenericPointersKeeperContainer PC = Vector<GenericPointerKeeper>
        , typename EPP = std::conditional_t<serialize, SerializeExtendedPointersProcessing<PM>, DeserializeExtendedPointersProcessing<PC, PM>>
>
    requires   ISerializationCapableContainer<Container>
            || IDeserializationCapableContainer<Container>
class Data : public Common<Container>
{
public:

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="container">Container that hold or would hold binary data from processing</param>
    /// <param name="dataFlags">Data flags that are using in processing</param>
    /// <param name="auxUsingHeapAllocation">Should allocation of temp data be used on heap instead of stack</param>
    /// <param name="interfaceVersion">Target interface version</param>
    constexpr Data(Container& binaryData
        , DataFlags dataFlags = DataFlags{}, bool auxUsingHeapAllocation = true, interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Container>(binaryData, traits::getLatestProtocolVersion(), Message::kData)
        , m_flags(dataFlags), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="common">Reference on Common</param>
    /// <param name="dataFlags">Data flags that are using in processing</param>
    /// <param name="auxUsingHeapAllocation">Should allocation of temp data be used on heap instead of stack</param>
    /// <param name="interfaceVersion">Target interface version</param>
    constexpr Data(Common<Container>& common, DataFlags dataFlags = DataFlags{}, bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Container>(common.getBinaryData(), common.getProtocolVersion(), Message::kData)
        , m_flags(dataFlags), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }
    
    /// <summary>
    /// Copy constructor
    /// </summary>
    /// <param name="rhs">Another instance</param>
    constexpr Data(Data<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Common<Container>(rhs.getBinaryData(), rhs.getProtocolVersion(), rhs.getMessageType())
        , m_flags(rhs.m_flags), m_epp(rhs.m_epp), m_interfaceVersion(rhs.interfaceVersion), m_auxUsingHeapAllocation(rhs.auxUsingHeapAllocation)
    { }

    /// <summary>
    /// Constructor (availible only on serialization mode)
    /// </summary>
    /// <param name="binaryData">Container that hold or would hold binary data from processing</param>
    /// <param name="protocolVersion">Protocol version that would be used in process</param>
    /// <param name="dataFlags">Data flags that are using in processing</param>
    /// <param name="auxUsingHeapAllocation">Should allocation of temp data be used on heap instead of stack</param>
    /// <param name="interfaceVersion">Target interface version</param>
    /// <param name="pointersMap">Pointer to map that implements ISerializationPointersMap interface</param>
    constexpr Data(Container& binaryData, protocol_version_t protocolVersion, DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true, interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined, PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
            : Common<Container>(binaryData, protocolVersion, Message::kData), m_flags(dataFlags)
            , m_epp(pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// <summary>
    /// Constructor (availible only on deserialization mode)
    /// </summary>
    /// <param name="binaryData">Container that hold or would hold binary data from processing</param>
    /// <param name="protocolVersion">Protocol version that would be used in process</param>
    /// <param name="dataFlags">Data flags that are using in processing</param>
    /// <param name="auxUsingHeapAllocation">Should allocation of temp data be used on heap instead of stack</param>
    /// <param name="interfaceVersion">Target interface version</param>
    /// <param name="pAddedPointers">Pointer to additional free pointers container</param>
    /// <param name="pPointersMap">Pointer to map that implements IDeserializationPointersMap interface</param>
    constexpr Data(Container& binaryData, protocol_version_t protocolVersion, DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true, interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined, PC* pAddedPointers = nullptr, PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
            : Common<Container>(binaryData, protocolVersion, Message::kData), m_flags(dataFlags)
            , m_epp(pAddedPointers, pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// <summary>
    /// Get data processing flags
    /// </summary>
    /// <returns>Data processing flags</returns>
    [[nodiscard]] constexpr DataFlags getDataFlags() noexcept { return m_flags; }

    /// <summary>
    /// Set data processing flags
    /// </summary>
    /// <param name="dataFlags">Data processing flags</param>
    constexpr void setDataFlags(DataFlags dataFlags) { m_flags = dataFlags; }

    /// <summary>
    /// Get target interface version
    /// </summary>
    /// <returns>Target interface version</returns>
    [[nodiscard]] constexpr interface_version_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(interface_version_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    /// <summary>
    /// Is target interface version differs of latest version of top struct that is processed
    /// </summary>
    /// <returns>Flag indicating that interface versions are not match</returns>
    [[nodiscard]] constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsNotMatch; }

    /// <summary>
    /// If target interface version is differs from top struct interface version, true must be set.
    /// Should be used before start of data processing.
    /// </summary>
    /// <param name="interfaceVersionsNotMatch">Flag indicating that interface versions are not match</param>
    constexpr void setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsNotMatch = interfaceVersionsNotMatch; }

    /// <summary>
    /// Test if allocation of temp data would be used on heap instead of stack
    /// </summary>
    /// <returns>Is allocation of temp data would be used on heap instead of stack</returns>
    [[nodiscard]] constexpr bool isAuxUsingHeapAllocation() const noexcept { return m_auxUsingHeapAllocation; }

    /// <summary>
    /// Set that allocation of temp data should use heap or stack
    /// </summary>
    /// <param name="auxUsingHeapAllocation">Flag indicating type of temp allocation</param>
    constexpr void setAuxUsingHeapAllocation(bool auxUsingHeapAllocation) { m_auxUsingHeapAllocation = auxUsingHeapAllocation; }

    /// <summary>
    /// Get pointer to holding pointers map
    /// </summary>
    /// <returns>Pointer to pointers map</returns>
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_epp.getPointersMap(); }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_epp.getPointersMap(); }

    /// <summary>
    /// Set holding pointers map.
    /// You should notice that when we set map to Data 
    /// it will not owns this map and does not clears its contents on destruction.
    /// </summary>
    /// <param name="pointersMap">Set pointer map to this</param>
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_epp.setPointersMap(pPointersMap); }

    /// <summary>
    /// Get pointer to added free pointers container.
    /// (availible only on deserialization mode)
    /// </summary>
    /// <returns>Pointer to added free pointers container</returns>
    [[nodiscard]] constexpr PC* getAddedPointers() noexcept requires !serialize { return m_epp.getAddedPointers(); }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept requires !serialize { return m_epp.getAddedPointers(); }

    /// <summary>
    /// Set holding pointer to added pointers container
    /// You should notice that when we set map to Data 
    /// it will not owns this container and does not clears its contents on destruction.
    /// (availible only on deserialization mode)
    /// </summary>
    /// <param name="addedPointers">Pointer to added free pointers container<</param>
    constexpr void setAddedPointers(PC* pAddedPointers) noexcept requires !serialize { m_epp.setAddedPointers(pAddedPointers); }

    /// <summary>
    /// Allocates memory for type T and costructs default T-object,
    /// and then places it to container of added free pointers.
    /// (availible only on deserialization mode)
    /// </summary>
    /// <typeparam name="T">Type of object to allocate and construct</typeparam>
    /// <returns>Pointer of costructed object</returns>
    template<typename T>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept requires !serialize { return m_epp.allocateAndDefaultConstruct<T>(); }

    /// <summary>
    /// Reset all fields to their default values, but leaves processed binary data unchanged.
    /// Also notice that flag of using heap allocation too not resets to false,
    /// cause it's rather environment tool option instead of struct/operation specific.
    /// </summary>
    void resetToDefaultsExceptDataContents() noexcept override
    {
        Common<Container>::resetToDefaultsExceptDataContents();
        m_flags = DataFlags{};
        m_interfaceVersion = traits::kInterfaceVersionUndefined;
        m_interfaceVersionsNotMatch = false;
        // 
        m_epp.clear();
    }

    /// <summary>
    /// Reset all fields to their default values and clears binary data container
    /// Also notice that flag of using heap allocation too not resets to false,
    /// cause it's rather environment tool option instead of struct/operation specific.
    /// </summary>
    void clear() noexcept override
    {
        Common<Container>::clear();
        m_flags = DataFlags{};
        m_interfaceVersion = traits::kInterfaceVersionUndefined;
        m_interfaceVersionsNotMatch = false;
        m_epp.clear();
    }

private:
    DataFlags m_flags = DataFlags{};
    EPP  m_epp;
    interface_version_t m_interfaceVersion{ traits::kInterfaceVersionUndefined };
    bool m_interfaceVersionsNotMatch = false;
    bool m_auxUsingHeapAllocation = false;
};

template<ISerializationCapableContainer S = BinVector, ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SData = Data<S, true, PM>;

template<IDeserializationCapableContainer D = BinWalker, IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DData = Data<D, false, PM>;

} // namespace common_serialization::csp::context
