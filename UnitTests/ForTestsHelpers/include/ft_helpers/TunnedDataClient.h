/**
 * @file UnitTests/ForTestsHelpers/include/ft_helpers/TunnedDataClient.h
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

namespace ft_helpers
{

namespace cs = common_serialization;

class TunnedDataClient : public cs::csp::messaging::IDataClient
{
public:
    using FilterFunction = void(*)(cs::BinWalker&);

    TunnedDataClient() {}
    TunnedDataClient(cs::csp::protocol_version_t defaultProtocolVersion, cs::csp::context::DataFlags defaultFlags
        , const cs::Uuid& defaultInterfaceId, cs::csp::interface_version_t targetInterfaceVersion
    )
        : cs::csp::messaging::IDataClient(defaultProtocolVersion, defaultFlags, defaultInterfaceId, targetInterfaceVersion)
    {
    }

    void setEffectiveMaxLoopsCount(size_t effectiveMaxLoopsCount)
    {
        if (effectiveMaxLoopsCount > kMaxLoopsCount)
            assert(false);

        m_effectiveMaxLoopsCount = effectiveMaxLoopsCount;
    }
    
    void setFilterFunction(FilterFunction filterFunction, bool isPreFunction, size_t index)
    {
        if (index >= m_effectiveMaxLoopsCount)
            return;

        isPreFunction ? m_preOperation[index] = filterFunction : m_postOperation[index] = filterFunction;
    }

    size_t getLoopCount() const noexcept
    {
        return m_operationsLoopIndex;
    }

    void resetLoopCount() noexcept
    {
        m_operationsLoopIndex = 0;
    }

private:
    cs::Status handleBinData(cs::BinVector& binInput, cs::BinWalker& binOutput) override
    {
        if (m_operationsLoopIndex == kMaxLoopsCount)
            return cs::Status::kErrorOverflow;

        cs::BinWalker input;
        input.init(std::move(binInput));

        if (m_preOperation[m_operationsLoopIndex])
            m_preOperation[m_operationsLoopIndex](input);
        
        if (cs::Status status = cs::csp::messaging::CommonServer::handleMessage(input, binOutput.getVector()); !statusSuccess(status))
            return status;

        if (m_postOperation[m_operationsLoopIndex])
            m_postOperation[m_operationsLoopIndex](binOutput);

        ++m_operationsLoopIndex;

        return cs::Status::kNoError;
    }

    size_t m_operationsLoopIndex{ 0 };

    static constexpr size_t kMaxLoopsCount = 3;
    size_t m_effectiveMaxLoopsCount = 3;

    FilterFunction m_preOperation[kMaxLoopsCount] = { nullptr };
    FilterFunction m_postOperation[kMaxLoopsCount]= { nullptr };
};

} // namespace ft_helpers

