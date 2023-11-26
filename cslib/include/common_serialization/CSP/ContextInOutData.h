/**
 * @file cslib/include/common_serialization/CSP/ContextInOutData.h
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

#include "common_serialization/CSP/ContextData.h"

namespace common_serialization::csp::context
{

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
    constexpr InOutData(Container& container, bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Data<Container, serialize, PM, PC, EPP>(container, auxUsingHeapAllocation, inInterfaceVersion)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { }

    constexpr InOutData(Common<Container>& common, bool auxUsingHeapAllocation = true
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
    ) noexcept
        : Data<Container, serialize, PM, PC, EPP>(common, auxUsingHeapAllocation, inInterfaceVersion)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { }

    constexpr InOutData(InOutData<Container, serialize, PM, PC, EPP>& rhs) noexcept
        : Data<Container, serialize, PM, PC, EPP>(rhs)
        , m_outputInterfaceVersion(rhs.m_outputInterfaceVersion)
    { }

    constexpr InOutData(Container& binaryData, protocol_version_t protocolVersion, DataFlags flags, bool auxUsingHeapAllocation = false
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
        , PM* pPointersMap = nullptr
    ) noexcept
        requires serialize
        : Data<Container, serialize, PM, PC, EPP>(binaryData, flags, auxUsingHeapAllocation, inInterfaceVersion, pPointersMap)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { }

    constexpr InOutData(Container& binaryData, protocol_version_t protocolVersion, DataFlags flags, bool auxUsingHeapAllocation = false
        , interface_version_t inInterfaceVersion = traits::kInterfaceVersionUndefined
        , interface_version_t outInterfaceVersion = traits::kInterfaceVersionUndefined
        , PC* pPointersContainer = nullptr, PM* pPointersMap = nullptr
    ) noexcept
        requires !serialize
        : Data<Container, serialize, PM, PC, EPP>(binaryData, flags, auxUsingHeapAllocation, inInterfaceVersion, pPointersContainer, pPointersMap)
        , m_outputInterfaceVersion(outInterfaceVersion)
    { }

    [[nodiscard]] constexpr interface_version_t getOutputInterfaceVersion() const noexcept { return m_outputInterfaceVersion; }
    constexpr void setOutputInterfaceVersion(interface_version_t outInterfaceVersion) { m_outputInterfaceVersion = outInterfaceVersion; }

private:
    interface_version_t m_outputInterfaceVersion{ traits::kInterfaceVersionUndefined };
};


template<ISerializationCapableContainer S = BinVector, ISerializationPointersMap PM = std::unordered_map<const void*, uint64_t>>
using SInOutData = InOutData<S, true, PM>;

template<IDeserializationCapableContainer D = BinWalker, IDeserializationPointersMap PM = std::unordered_map<uint64_t, void*>>
using DInOutData = InOutData<D, false, PM>;

} // namespace common_serialization::csp::context