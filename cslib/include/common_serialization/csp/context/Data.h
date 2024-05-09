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

namespace common_serialization::csp::context
{

/// @brief Auxillary class for holding pointers map to prevent infinite recursive pointers serialization
/// @tparam _Spm Map that implements ISerializationPointersMap interface.
///     We cannot direct use more restricted concept here
///     because std::conditional_t evaluating all traits in instantiation of Data class template.
template<ISerializationPointersMap _Spm>
class SerializeExtendedPointersProcessing
{
public:
    using Spm = _Spm;

    /// @brief Default ctor
    SerializeExtendedPointersProcessing()
        : m_pPointersMap(nullptr)
    { }

    /// @brief Ctor from IPointersMap
    /// @param pPointersMap Pointer to map that implements ISerializationPointersMap interface
    explicit SerializeExtendedPointersProcessing(Spm* pPointersMap)
        : m_pPointersMap(pPointersMap)
    { }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map (could be nullptr)
    [[nodiscard]] constexpr Spm* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const Spm* getPointersMap() const noexcept { return m_pPointersMap; }

    /// @brief Set map that holding pointers and their relative offsets in binary data.
    /// @note When we set map to SerializeExtendedPointersProcessing 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Pointer to pointers map (could be nullptr)
    constexpr void setPointersMap(Spm* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }

    /// @brief Clears contents of pointers map
    void clear()
    {
        if (m_pPointersMap)
            m_pPointersMap->clear();
    }

private:
    Spm* m_pPointersMap{ nullptr };
};

/// @brief Auxillary class for holding pointers containers using in some deserialization scenarios
/// @tparam Allocator Stateless Allocator that would be used for free pointers allocation
///     (should be construction capable)
/// @tparam AllocatorHelperStateless AllocatorHelper that would be used for free pointers allocation help
///     (should be construction capable)
template<IGenericPointersKeeperContainer _Gkc, IDeserializationPointersMap _Dpm
    , template<typename> typename _Allocator = ConstructorNoexceptAllocator, template<typename, typename, typename> typename _AllocatorHelper = GenericAllocatorHelper>
class DeserializeExtendedPointersProcessing
{
public:
    using Gkc = _Gkc;
    using Dpm = _Dpm;

    template<typename _T>
    using Allocator = _Allocator<_T>;
    template<typename _T1, typename _T2, typename _T3>
    using AllocatorHelper = _AllocatorHelper<_T1, _T2, _T3>;

    /// @brief Default constuctor
    DeserializeExtendedPointersProcessing()
        : m_pAddedPointers(nullptr), m_pPointersMap(nullptr)
    { }
    
    /// @brief Constructor
    /// @param pAddedPonters Pointer to additional free pointers container
    /// @param pPointersMap Pointer to map that implements IDeserializationPointersMap interface
    DeserializeExtendedPointersProcessing(Gkc* pAddedPonters, Dpm* pPointersMap)
        : m_pAddedPointers(pAddedPonters), m_pPointersMap(pPointersMap)
    { }

    /// @brief Get pointer to added free pointers container
    /// @return Pointer to added free pointers container
    [[nodiscard]] constexpr Gkc* getAddedPointers() noexcept { return m_pAddedPointers; }
    [[nodiscard]] constexpr const Gkc* getAddedPointers() const noexcept { return m_pAddedPointers; }

    /// @brief Set pointer to added pointers container
    /// @note When we set map to DeserializeExtendedPointersProcessing 
    ///     it will not owns this container and does not clears its contents on destruction.
    /// @param pAddedPointers Pointer to added free pointers container
    constexpr void setAddedPointers(Gkc* pAddedPointers) noexcept { m_pAddedPointers = pAddedPointers; }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map
    [[nodiscard]] constexpr Dpm* getPointersMap() noexcept { return m_pPointersMap; }
    [[nodiscard]] constexpr const Dpm* getPointersMap() const noexcept { return m_pPointersMap; }

    /// @brief Set map that holding pointers and their relative offsets in binary data.
    /// @note When we set map to DeserializeExtendedPointersProcessing 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Set pointer map to this
    constexpr void setPointersMap(Dpm* pPointersMap) noexcept { m_pPointersMap = pPointersMap; }
    
    /// @brief Allocates memory for type T and costructs default T-object,
    ///     and then places it to container of added free pointers.
    /// @tparam T Type of object to allocate and construct
    /// @return Pointer of costructed object
    template<typename T>
        requires IConstructorAllocator<Allocator<T>>
    [[nodiscard]] T* allocateAndDefaultConstruct()
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
    Gkc* m_pAddedPointers{ nullptr };
    Dpm* m_pPointersMap{ nullptr };
};

template<AnySdContainers _Asdcs, bool>
struct _Epp
{
    using type = SerializeExtendedPointersProcessing<typename _Asdcs::Spm>;
};

template<AnySdContainers _Asdcs>
struct _Epp<_Asdcs, false>
{
    using type = DeserializeExtendedPointersProcessing<typename _Asdcs::Gkc, typename _Asdcs::Dpm>;
};

template<AnySdContainers _Asdcs, bool B>
using Epp = _Epp<_Asdcs, B>::type;

/// @brief Full context of CSP Data Message
/// @tparam _Containers Type that defines SContainers or DContainers
/// @note Use SContainers set for serialization capable context
///     and DContainers set for deserialization capable context
template<AnySdContainers _Containers>
class Data : public Common<traits::BinContainer<_Containers, SContainers<_Containers>>>
{
public:
    using Containers = _Containers;

    using Bin = traits::BinContainer<Containers, SContainers<Containers>>;
    using PM = traits::PmContainer<Containers, SContainers<Containers>>;
    using Gkc = traits::PcContainer<Containers, SContainers<Containers>>;
    using EPP = Epp<Containers, SContainers<Containers>>;

    /// @brief Constructor
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    explicit constexpr Data(
          Bin& binaryData
        , CommonFlags commonFlags = {}
        , DataFlags dataFlags = {}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Bin>(binaryData, traits::getLatestProtocolVersion(), Message::Data, commonFlags)
        , m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { 
        setDataFlags(dataFlags);
    }

    /// @brief Constructor
    /// @param common Reference on Common
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    explicit constexpr Data(
          Common<Bin>& common
        , DataFlags dataFlags = {}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Common<Bin>(std::move(common)), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { 
        setDataFlags(dataFlags);
    }

    /// @brief Constructor
    /// @remark availible only on serialization mode
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param protocolVersion Protocol version that would be used in process
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param interfaceVersion Target interface version
    /// @param pointersMap Pointer to map that implements ISerializationPointersMap interface
    constexpr Data(
          Bin& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = {}
        , DataFlags dataFlags = {}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
        , PM* pPointersMap = nullptr
    ) noexcept
        requires SContainers<Containers>
            : Common<Bin>(binaryData, protocolVersion, Message::Data, commonFlags)
            , m_epp(pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { 
       setDataFlags(dataFlags);
    }

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
          Bin& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = {}
        , DataFlags dataFlags = {}
        , bool auxUsingHeapAllocation = true
        , interface_version_t interfaceVersion = traits::kInterfaceVersionUndefined
        , Gkc* pAddedPointers = nullptr
        , PM* pPointersMap = nullptr
    ) noexcept
        requires (!SContainers<Containers>)
            : Common<Bin>(binaryData, protocolVersion, Message::Data, commonFlags)
            , m_epp(pAddedPointers, pPointersMap), m_interfaceVersion(interfaceVersion), m_auxUsingHeapAllocation(auxUsingHeapAllocation)
    { 
       setDataFlags(dataFlags);
    }

    /// @brief Get data processing flags
    /// @return Data processing flags
    [[nodiscard]] CS_ALWAYS_INLINE constexpr DataFlags getDataFlags() noexcept
    {
        return m_dataFlags;
    }

    /// @brief Set data processing flags
    /// @param dataFlags Data processing flags
    constexpr Data& setDataFlags(DataFlags dataFlags)
    {
        m_dataFlags = dataFlags;
        m_alignmentMayBeNotEqual = m_dataFlags.alignmentMayBeNotEqual();
        m_sizeOfPrimitivesMayBeNotEqual = m_dataFlags.sizeOfIntegersMayBeNotEqual();
        m_allowUnmanagedPointers = m_dataFlags.allowUnmanagedPointers();
        m_checkRecursivePointers = m_dataFlags.checkRecursivePointers();
        m_simplyAssignableTagsOptimizationsAreTurnedOff = m_dataFlags.simplyAssignableTagsOptimizationsAreTurnedOff();
        return *this;
    }

    CS_ALWAYS_INLINE constexpr Data& setDataFlags(uint32_t dataFlags)
    {
        return setDataFlags(static_cast<DataFlags>(dataFlags));
    }

    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool alignmentMayBeNotEqual() const noexcept { return m_alignmentMayBeNotEqual; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool sizeOfIntegersMayBeNotEqual() const noexcept { return m_sizeOfPrimitivesMayBeNotEqual; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool allowUnmanagedPointers() const noexcept { return m_allowUnmanagedPointers; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool checkRecursivePointers() const noexcept { return m_checkRecursivePointers; }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool simplyAssignableTagsOptimizationsAreTurnedOff() const noexcept { return m_simplyAssignableTagsOptimizationsAreTurnedOff; }

    /// @brief Get target interface version
    /// @return Target interface version
    [[nodiscard]] CS_ALWAYS_INLINE constexpr interface_version_t getInterfaceVersion() const noexcept { return m_interfaceVersion; }
    CS_ALWAYS_INLINE constexpr Data& setInterfaceVersion(interface_version_t interfaceVersion) { m_interfaceVersion = interfaceVersion; return *this; }

    /// @brief Is target interface version differs of latest version of top struct that is processed
    /// @return Flag indicating that interface versions are not match
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool isInterfaceVersionsNotMatch() const noexcept { return m_interfaceVersionsNotMatch; }

    /// @brief If target interface version is differs from top struct interface version, true must be set.
    /// @note Should be used before start of data processing.
    /// @param interfaceVersionsNotMatch Flag indicating that interface versions are not match
    CS_ALWAYS_INLINE constexpr Data& setInterfaceVersionsNotMatch(bool interfaceVersionsNotMatch) { m_interfaceVersionsNotMatch = interfaceVersionsNotMatch; return *this; }

    /// @brief Test if allocation of temp data would be used on heap instead of stack
    /// @return Is allocation of temp data would be used on heap instead of stack
    [[nodiscard]] CS_ALWAYS_INLINE constexpr bool isAuxUsingHeapAllocation() const noexcept { return m_auxUsingHeapAllocation; }
    
    /// @brief Set that allocation of temp data should use heap or stack
    /// @param auxUsingHeapAllocation Flag indicating type of temp allocation
    CS_ALWAYS_INLINE constexpr Data& setAuxUsingHeapAllocation(bool auxUsingHeapAllocation) { m_auxUsingHeapAllocation = auxUsingHeapAllocation; return *this; }

    /// @brief Get pointer to holding pointers map
    /// @return Pointer to pointers map
    [[nodiscard]] CS_ALWAYS_INLINE constexpr PM* getPointersMap() noexcept { return m_epp.getPointersMap(); }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr const PM* getPointersMap() const noexcept { return m_epp.getPointersMap(); }

    /// @brief Set holding pointers map.
    ///     You should notice that when we set map to Data 
    ///     it will not owns this map and does not clears its contents on destruction.
    /// @param pPointersMap Set pointer map to this
    CS_ALWAYS_INLINE constexpr Data& setPointersMap(PM* pPointersMap) noexcept { m_epp.setPointersMap(pPointersMap); return *this; }

    /// @brief Get pointer to added free pointers container.
    /// @remark availible only on deserialization mode
    /// @return Pointer to added free pointers container
    [[nodiscard]] CS_ALWAYS_INLINE constexpr Gkc* getAddedPointers() noexcept requires (!SContainers<Containers>) { return m_epp.getAddedPointers(); }
    [[nodiscard]] CS_ALWAYS_INLINE constexpr const Gkc* getAddedPointers() const noexcept requires (!SContainers<Containers>) { return m_epp.getAddedPointers(); }

    /// @brief Set holding pointer to added pointers container
    /// @note When we set map to Data it will not owns it
    ///     and does not clears its contents on destruction.
    /// @remark availible only on deserialization mode
    /// @param pAddedPointers Pointer to added free pointers container
    CS_ALWAYS_INLINE constexpr Data& setAddedPointers(Gkc* pAddedPointers) noexcept requires (!SContainers<Containers>) { m_epp.setAddedPointers(pAddedPointers); return *this; }

    /// @brief Allocates memory for type T and costructs default T-object,
    ///     and then places it to container of added free pointers.
    /// @remark availible only on deserialization mode
    /// @tparam T Type of object to allocate and construct
    /// @return Pointer of costructed object
    template<typename T>
    [[nodiscard]] CS_ALWAYS_INLINE T* allocateAndDefaultConstruct() noexcept requires (!SContainers<Containers>) { return m_epp.template allocateAndDefaultConstruct<T>(); }

    /// @brief Reset all fields to their default values, but leaves processed binary data unchanged.
    /// @note Flag of using heap allocation also not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
    Data& resetToDefaultsExceptDataContents() noexcept override
    {
        Common<Bin>::resetToDefaultsExceptDataContents();
        m_epp.clear();
        m_interfaceVersion = traits::kInterfaceVersionUndefined;
        m_interfaceVersionsNotMatch = false;
        setDataFlags({});
        return *this;
    }

    /// @brief Reset all fields to their default values and clears binary data container
    /// @note Flag of using heap allocation not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
    Data& clear() noexcept override
    {
        Common<Bin>::clear();
        m_epp.clear();
        m_interfaceVersion = traits::kInterfaceVersionUndefined;
        m_interfaceVersionsNotMatch = false;
        setDataFlags({});
        return *this;
    }

private:
    EPP  m_epp;
    interface_version_t m_interfaceVersion{ traits::kInterfaceVersionUndefined };
    DataFlags m_dataFlags;
    bool m_interfaceVersionsNotMatch{ false };
    bool m_auxUsingHeapAllocation{ false };

    // All next bool members are mirroring m_dataFlags value
    // they all are precalculated, when m_dataFlags is set up.
    // We keep both values m_dataFlags and respective booleans
    // for caching proposes (to speed up calculations).
    bool m_alignmentMayBeNotEqual{ false };
    bool m_sizeOfPrimitivesMayBeNotEqual{ false };
    bool m_allowUnmanagedPointers{ false };
    bool m_checkRecursivePointers{ false };
    bool m_simplyAssignableTagsOptimizationsAreTurnedOff{ false };
};

using SData = Data<traits::DefaultSContainers>;
using DData = Data<traits::DefaultDContainers>;

} // namespace common_serialization::csp::context
