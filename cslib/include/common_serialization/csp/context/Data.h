/**
 * @file cslib/include/common_serialization/csp/context/Data.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

/// @brief Auxillary class for holding pointers map to prevent infinite recursive pointers serialization
/// @tparam PM Map that implements ISerializationPointersMap interface.
///     We cannot direct use more restricted concept here
///     because std::conditional_t evaluating all traits in instantiation of Data class template.
template<IPointersMap PM>
class SerializeExtendedPointersProcessing
{
public:
    /// @brief Default constructor
    SerializeExtendedPointersProcessing()
        : m_pPointersMap(nullptr)
    { }

    /// @brief Constructor from IPointersMap
    /// @param pPointersMap Pointer to map that implements ISerializationPointersMap interface
    SerializeExtendedPointersProcessing(PM* pPointersMap)
        : m_pPointersMap(pPointersMap)
    { }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map (could be nullptr)
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }

    /// @brief Set map that holding pointers and their relative offsets in binary data.
    /// @note When we set map to SerializeExtendedPointersProcessing 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Pointer to pointers map (could be nullptr)
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }

    /// @brief Clears contents of pointers map
    void clear()
    {
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    PM* m_pPointersMap{ nullptr };
};

/// @brief Auxillary class for holding pointers containers using in some deserialization scenarios
/// @tparam PC Container that implements IGenericPointersKeeperContainer interface.
///     Using for keeping additional free pointers that creates deserialization process.
///     When pointers are no longer needed it should be freed.
/// @tparam PM Container that implements IGenericPointersKeeperContainer interface.
///     Using for keeping additional free pointers that creates deserialization process.
///     When pointers are no longer needed it should be freed.
/// @tparam Allocator Stateless Allocator that would be used for free pointers allocation
///     (should be construction capable)
/// @tparam AllocatorHelperStateless AllocatorHelper that would be used for free pointers allocation help
///     (should be construction capable)
template<IGenericPointersKeeperContainer PC, IPointersMap PM
    , template<typename> typename Allocator = ConstructorNoexceptAllocator, template<typename, typename, typename> typename AllocatorHelper = GenericAllocatorHelper>
class DeserializeExtendedPointersProcessing
{
public:
    /// @brief Default constuctor
    DeserializeExtendedPointersProcessing()
        : m_pAddedPointers(nullptr), m_pPointersMap(nullptr)
    { }
    
    /// @brief Constructor
    /// @param pAddedPonters Pointer to additional free pointers container
    /// @param pPointersMap Pointer to map that implements IDeserializationPointersMap interface
    DeserializeExtendedPointersProcessing(PC* pAddedPonters, PM* pPointersMap)
        : m_pAddedPointers(pAddedPonters), m_pPointersMap(pPointersMap)
    { }

    /// @brief Get pointer to added free pointers container
    /// @return Pointer to added free pointers container
    [[nodiscard]] constexpr PC* getAddedPointers() noexcept { return m_pAddedPointers; }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept { return m_pAddedPointers; }

    /// @brief Set pointer to added pointers container
    /// @note When we set map to DeserializeExtendedPointersProcessing 
    ///     it will not owns this container and does not clears its contents on destruction.
    /// @param pAddedPointers Pointer to added free pointers container
    constexpr void setAddedPointers(PC* pAddedPointers) noexcept { m_pAddedPointers = pAddedPointers; }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_pPointersMap; }

    /// @brief Set map that holding pointers and their relative offsets in binary data.
    /// @note When we set map to DeserializeExtendedPointersProcessing 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Set pointer map to this
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }
    
    /// @brief Allocates memory for type T and costructs default T-object,
    ///     and then places it to container of added free pointers.
    /// @tparam T Type of object to allocate and construct
    /// @return Pointer of costructed object
    template<typename T>
        requires IConstructorAllocator<Allocator<T>>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept
    {
        if (!m_pAddedPointers)
            assert(false); // this situation shall never exists

        GenericPointerKeeper pointerKeeper;
        T* p = pointerKeeper.allocateAndConstruct<T, AllocatorHelper<T, Allocator<T>, Dummy>>(1);

        if (p)
            if (!statusSuccess(m_pAddedPointers->pushBack(std::move(pointerKeeper))))
                p = nullptr;

        return p;
    }

    /// @brief Clears contents of holding containers
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

/// @brief Holder of context in processing message of type Data
/// @tparam Container Container capable to serialization/deserialization
/// @tparam serialize Type of scenario (serialization|deserialization) for specialization
/// @tparam PM Map that implements ISerializationPointersMap or IDeserializationPointersMap interface.
///     (std::unordered_map shall be replaced by common_serialization::HashMap when it would be ready)
/// @tparam PC Container that implements IGenericPointersKeeperContainer interface
/// @tparam EPP SerializeExtendedPointersProcessing or DeserializeExtendedPointersProcessing (depends on serialize flag)
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
    /// @brief Constructor
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    constexpr Data(
          Container& binaryData
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Container>(binaryData, traits::getLatestProtocolVersion(), Message::kData, commonFlags)
        , m_flags(dataFlags), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// @brief Constructor
    /// @param common Reference on Common
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    constexpr Data(
          Common<Container>& common
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Container>(common), m_flags(dataFlags), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }
    
    /// @brief Copy constructor
    /// @param rhs Another instance
    constexpr Data(Data<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Common<Container>(rhs)
        , m_flags(rhs.m_flags), m_epp(rhs.m_epp), m_interfaceVersion(rhs.m_interfaceVersion), m_auxUsingHeapAllocation(rhs.m_auxUsingHeapAllocation)
    { }

    /// @brief Constructor
    /// @remark availible only on serialization mode
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param protocolVersion Protocol version that would be used in process
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    /// @param pointersMap Pointer to map that implements ISerializationPointersMap interface
    constexpr Data(
          Container& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
        , PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
            : Common<Container>(binaryData, protocolVersion, Message::kData, commonFlags), m_flags(dataFlags)
            , m_epp(pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// @brief Constructor
    /// @remark availible only on deserialization mode
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param protocolVersion Protocol version that would be used in process
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    /// @param pAddedPointers Pointer to additional free pointers container
    /// @param pointersMap Pointer to map that implements ISerializationPointersMap interface
    constexpr Data(
          Container& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}, bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
        , PC* pAddedPointers = nullptr
        , PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
            : Common<Container>(binaryData, protocolVersion, Message::kData, commonFlags), m_flags(dataFlags)
            , m_epp(pAddedPointers, pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { }

    /// @brief Get data processing flags
    /// @return Data processing flags
    [[nodiscard]] constexpr DataFlags getDataFlags() noexcept { return m_flags; }

    /// @brief Set data processing flags
    /// @param dataFlags Data processing flags
    constexpr void setDataFlags(DataFlags dataFlags) { m_flags = dataFlags; }

    /// @brief Get target interface version
    /// @return Target interface version
    [[nodiscard]] constexpr interface_version_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    constexpr void setInterfaceVersion(interface_version_t interfaceVersion) { m_interfaceVersion = interfaceVersion; }

    /// @brief Is target interface version differs of latest version of top struct that is processed
    /// @return Flag indicating that interface versions are not match
    [[nodiscard]] constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsNotMatch; }

    /// @brief If target interface version is differs from top struct interface version, true must be set.
    /// @note Should be used before start of data processing.
    /// @param interfaceVersionsNotMatch Flag indicating that interface versions are not match
    constexpr void setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsNotMatch = interfaceVersionsNotMatch; }

    /// @brief Test if allocation of temp data would be used on heap instead of stack
    /// @return Is allocation of temp data would be used on heap instead of stack
    [[nodiscard]] constexpr bool isAuxUsingHeapAllocation() const noexcept { return m_auxUsingHeapAllocation; }
    
    /// @brief Set that allocation of temp data should use heap or stack
    /// @param auxUsingHeapAllocation Flag indicating type of temp allocation
    constexpr void setAuxUsingHeapAllocation(bool auxUsingHeapAllocation) { m_auxUsingHeapAllocation = auxUsingHeapAllocation; }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map
    [[nodiscard]] constexpr PM* getPointersMap() noexcept { return m_epp.getPointersMap(); }
    [[nodiscard]] constexpr const PM* getPointersMap() const noexcept { return m_epp.getPointersMap(); }

    /// @brief Set holding pointers map.
    ///     You should notice that when we set map to Data 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Set pointer map to this
    constexpr void setPointersMap(PM* pPointersMap) noexcept { m_epp.setPointersMap(pPointersMap); }

    /// @brief Get pointer to added free pointers container.
    /// @remark availible only on deserialization mode
    /// @return Pointer to added free pointers container
    [[nodiscard]] constexpr PC* getAddedPointers() noexcept requires !serialize { return m_epp.getAddedPointers(); }
    [[nodiscard]] constexpr const PC* getAddedPointers() const noexcept requires !serialize { return m_epp.getAddedPointers(); }

    /// @brief Set holding pointer to added pointers container
    /// @note When we set map to Data it will not owns it
    ///     and does not clears its contents on destruction.
    /// @remark availible only on deserialization mode
    /// @param pAddedPointers Pointer to added free pointers container
    constexpr void setAddedPointers(PC* pAddedPointers) noexcept requires !serialize { m_epp.setAddedPointers(pAddedPointers); }

    /// @brief Allocates memory for type T and costructs default T-object,
    ///     and then places it to container of added free pointers.
    /// @remark availible only on deserialization mode
    /// @tparam T Type of object to allocate and construct
    /// @return Pointer of costructed object
    template<typename T>
    [[nodiscard]] T* allocateAndDefaultConstruct() const noexcept requires !serialize { return m_epp.allocateAndDefaultConstruct<T>(); }

    /// @brief Reset all fields to their default values, but leaves processed binary data unchanged.
    /// @note Flag of using heap allocation also not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
    void resetToDefaultsExceptDataContents() noexcept override
    {
        Common<Container>::resetToDefaultsExceptDataContents();
        m_flags = DataFlags{};
        m_interfaceVersion = traits::kInterfaceVersionUndefined;
        m_interfaceVersionsNotMatch = false;
        m_epp.clear();
    }

    /// @brief Reset all fields to their default values and clears binary data container
    /// @note Flag of using heap allocation not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
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

template<ISerializationCapableContainer S = BinVector, ISerializationPointersMap PM = std::unordered_map<const void*, size_t>>
using SData = Data<S, true, PM>;

template<IDeserializationCapableContainer D = BinWalker, IDeserializationPointersMap PM = std::unordered_map<size_t, void*>>
using DData = Data<D, false, PM>;

} // namespace common_serialization::csp::context
