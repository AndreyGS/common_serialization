/**
 * @file cslib/include/common_serialization/csp/context/InOutData.h
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

#include "common_serialization/csp/context/Data.h"

namespace common_serialization::csp::context
{

/// @brief Holder of context in processing message of type InOutData
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
class InOutData : public Data<Container, serialize, PM, PC, EPP>
{
public:
    /// @brief Constructor
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param inInterfaceVersion Target input struct interface version
    /// @param outInterfaceVersion Target output struct interface version
    constexpr InOutData(
          Container& container
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Data<Container, serialize, PM, PC, EPP>(container, commonFlags, dataFlags, auxUsingHeapAllocation, inInterfaceVersion)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { 
        this->setMessageType(Message::kInOutData);
    }

    /// @brief Constructor
    /// @param common Reference on Common
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param inInterfaceVersion Target input struct interface version
    /// @param outInterfaceVersion Target output struct interface version
    constexpr InOutData(
          Common<Container>& common
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Data<Container, serialize, PM, PC, EPP>(common, dataFlags, auxUsingHeapAllocation, inInterfaceVersion)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { }

    /// @brief Copy constructor
    /// @param rhs Another instance
    constexpr InOutData(InOutData<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Data<Container, serialize, PM, PC, EPP>(rhs)
        , m_outputInterfaceVersion(rhs.m_outputInterfaceVersion)
    { }

    /// @brief Constructor (availible only on serialization mode)
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param protocolVersion Protocol version that would be used in process
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param inInterfaceVersion Target input struct interface version
    /// @param outInterfaceVersion Target output struct interface version
    /// @param pointersMap Pointer to map that implements ISerializationPointersMap interface
    constexpr InOutData(
          Container& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
        , PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
        : Data<Container, serialize, PM, PC, EPP>(binaryData, protocolVersion, commonFlags, dataFlags
            , auxUsingHeapAllocation, inInterfaceVersion, pPointersMap)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { 
        this->setMessageType(Message::kInOutData);
    }
    
    /// @brief Constructor (availible only on deserialization mode)
    /// @param binaryData Container that hold or would hold binary data from processing
    /// @param protocolVersion Protocol version that would be used in process
    /// @param dataFlags Data flags that are using in processing
    /// @param auxUsingHeapAllocation Should allocation of temp data be used on heap instead of stack
    /// @param inInterfaceVersion Target input struct interface version
    /// @param outInterfaceVersion Target output struct interface version
    /// @param pAddedPointers Pointer to additional free pointers container
    /// @param pointersMap Pointer to map that implements ISerializationPointersMap interface
    constexpr InOutData(
          Container& binaryData
        , protocol_version_t protocolVersion
        , CommonFlags commonFlags = CommonFlags{}
        , DataFlags dataFlags = DataFlags{}
        , bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
        , PC* pPointersContainer = nullptr
        , PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
        : Data<Container, serialize, PM, PC, EPP>(binaryData, protocolVersion, commonFlags, dataFlags
            , auxUsingHeapAllocation, inInterfaceVersion, pPointersContainer, pPointersMap)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { 
        this->setMessageType(Message::kInOutData);
    }

    /// @brief Get target output interface version
    /// @return Target output interface version
    [[nodiscard]] constexpr interface_version_t getOutputInterfaceVersion() const noexcept { return m_outputInterfaceVersion; }

    /// @brief Set target output interface version
    /// @param outInterfaceVersion Target output interface version
    constexpr void setOutputInterfaceVersion(interface_version_t outInterfaceVersion) { m_outputInterfaceVersion = outInterfaceVersion; }

    /// @brief Reset all fields to their default values, but leaves processed binary data unchanged.
    /// @note Fag of using heap allocation also not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
    void resetToDefaultsExceptDataContents() noexcept override
    {
        Data<Container, serialize, PM, PC, EPP>::resetToDefaultsExceptDataContents();
        m_outputInterfaceVersion = traits::kInterfaceVersionUndefined;
    }

    /// @brief Reset all fields to their default values and clears binary data container
    /// @note Flag of using heap allocation not resets to false,
    ///     because it's rather environment tool option instead of struct/operation specific.
    void clear() noexcept override
    {
        Data<Container, serialize, PM, PC, EPP>::clear();
        m_outputInterfaceVersion = traits::kInterfaceVersionUndefined;
    }

private:
    interface_version_t m_outputInterfaceVersion{ traits::kInterfaceVersionUndefined };
};

template<ISerializationCapableContainer S = BinVector, ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SInOutData = InOutData<S, true, PM>;

template<IDeserializationCapableContainer D = BinWalker, IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DInOutData = InOutData<D, false, PM>;

} // namespace common_serialization::csp::context