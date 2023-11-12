/**
 * @file ProcessingDataVersionConverters.h
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

#include "ProcessingDataProcessor.h"

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<typename... NextTo>
struct ToVersionConverter;

template<>
class ToVersionConverter<>
{
public:
    ToVersionConverter(uint32_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    template<typename From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convert(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        return Status::kErrorInternal;
    }

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

private:
    uint32_t m_targetVersion{ traits::kInterfaceVersionMax };
};


template<typename To, typename... NextTo>
class ToVersionConverter<To, NextTo...> : public ToVersionConverter<NextTo...>
{

public:
    using base_class = ToVersionConverter<NextTo...>;

    ToVersionConverter(uint32_t targetVersion)
        : ToVersionConverter<NextTo...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<typename From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convert(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        if (ctx.isAuxUsingHeapAllocation())
        {
            GenericPointerKeeper pointerKeeper;
            if (!pointerKeeper.allocateAndConstruct<To, GenericAllocatorHelper<To, ConstructorNoexceptAllocator<To>>>(1))
                return Status::kErrorNoMemory;

            RUN(pointerKeeper.get<To>()->init(from));

            if (pVersionsHierarchy[0] > getTargetVersion())
                RUN(base_class::convert(*pointerKeeper.get<To>(), ctx))
            else
                RUN(DataProcessor::serializeDataLegacy(*pointerKeeper.get<To>(), ctx))
        }
        else
        {
            To to;
            RUN(to.init(from));

            if (pVersionsHierarchy[0] > getTargetVersion())
                RUN(base_class::convert(to, ctx))
            else
                RUN(DataProcessor::serializeDataLegacy(to, ctx))
        }

        return Status::kNoError;
    }

    static constexpr const uint32_t* pVersionsHierarchy = To::kVersionsHierarchy;
};

template<typename... NextFrom>
struct FromVersionConverter;

template<>
class FromVersionConverter<>
{
public:
    FromVersionConverter(uint32_t targetVersion)
        : m_targetVersion(targetVersion)
    { }

    template<typename To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convert(context::DData<D, PM>& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    template<typename From, typename To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersion(const From& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

    static constexpr const uint32_t pVersionsHierarchy[1] = { traits::kInterfaceVersionMax };

protected:
    using base_class_from = Dummy; // placeholder

private:
    uint32_t m_targetVersion{ traits::kInterfaceVersionMax };
};


template<typename From, typename... NextFrom>
class FromVersionConverter<From, NextFrom...> : public FromVersionConverter<NextFrom...>
{
public:
    FromVersionConverter(uint32_t targetVersion)
        : FromVersionConverter<NextFrom...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<typename To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convert(context::DData<D, PM>& ctx, To& to) noexcept
    {
        // Skip versions that are older than serialized one
        if (base_class::pVersionsHierarchy[0] <= getTargetVersion())
            RUN(base_class::convert(ctx, to))
        else
        {
            if (ctx.isAuxUsingHeapAllocation())
            {
                GenericPointerKeeper pointerKeeper;
                if (!pointerKeeper.allocateAndConstruct<From, GenericAllocatorHelper<From, ConstructorNoexceptAllocator<From>>>(1))
                    return Status::kErrorNoMemory;

                RUN(DataProcessor::deserializeDataLegacy(ctx, *pointerKeeper.get<From>()));
                RUN(convertToUpperVersion(*pointerKeeper.get<From>(), ctx, to));
            }
            else
            {
                From from;
                RUN(DataProcessor::deserializeDataLegacy(ctx, from));
                RUN(convertToUpperVersion(from, ctx, to));
            }
        }

        return Status::kNoError;
    }

    static constexpr const uint32_t* pVersionsHierarchy = From::kVersionsHierarchy;

protected:
    using base_class = FromVersionConverter<NextFrom...>;
    using base_class_from = From;

    template<typename To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersion(const From& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        if (base_class::pVersionsHierarchy[0] != traits::kInterfaceVersionMax)
        {
            using base_from = typename base_class::base_class_from;

            if (ctx.isAuxUsingHeapAllocation())
            {
                GenericPointerKeeper pointerKeeper;
                if (!pointerKeeper.allocateAndConstruct<base_from, GenericAllocatorHelper<base_from, ConstructorNoexceptAllocator<base_from>>>(1))
                    return Status::kErrorNoMemory;

                if constexpr (InitableBySpecialClass<To, base_from>)
                    RUN(pointerKeeper.get<base_from>()->init(from))
                else
                    return Status::kErrorNoSuchHandler;

                RUN(base_class::convertToUpperVersion(*pointerKeeper.get<base_from>(), ctx, to));
            }
            else
            {
                base_from bFrom;

                if constexpr (InitableBySpecialClass<To, base_from>)
                    RUN(bFrom.init(from))
                else
                    return Status::kErrorNoSuchHandler;

                RUN(base_class::convertToUpperVersion(bFrom, ctx, to));
            }
        }
        else if constexpr (InitableBySpecialClass<To, From>)
            RUN(to.init(from))
        else
            return Status::kErrorNoSuchHandler;

        return Status::kNoError;
    }
};

} // namespace processing

} // namespace csp

} // namespace common_serialization
