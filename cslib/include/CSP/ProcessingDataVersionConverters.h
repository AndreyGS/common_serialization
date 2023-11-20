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

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    template<typename From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convertOnHeap(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        return Status::kErrorInternal;
    }

    template<typename From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convertOnStack(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        return Status::kErrorInternal;
    }

private:
    uint32_t m_targetVersion{ traits::kInterfaceVersionMax };
};

template<serialization_concepts::IsISerializableBased To, typename... NextTo>
class ToVersionConverter<To, NextTo...> : public ToVersionConverter<NextTo...>
{
public:
    ToVersionConverter(uint32_t targetVersion)
        : ToVersionConverter<NextTo...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<serialization_concepts::IsISerializableBased From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convert(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        if (ctx.isAuxUsingHeapAllocation())
            RUN(convertOnHeap(from, ctx))
        else
            RUN(convertOnStack(from, ctx))

        return Status::kNoError;
    }

protected:
    using base_class = ToVersionConverter<NextTo...>;

    template<serialization_concepts::IsISerializableBased From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convertOnHeap(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<To, GenericAllocatorHelper<To, ConstructorNoexceptAllocator<To>>>(1))
            return Status::kErrorNoMemory;

        RUN(pointerKeeper.get<To>()->init(from));

        if (pVersionsHierarchy[0] > getTargetVersion())
            RUN(base_class::convertOnHeap(*pointerKeeper.get<To>(), ctx))
        else
            RUN(DataProcessor::serializeData(*pointerKeeper.get<To>(), ctx))

            return Status::kNoError;
    }

    template<serialization_concepts::IsISerializableBased From, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::ISerializationPointersMap PM>
    Status convertOnStack(const From& from, context::SData<S, PM>& ctx) noexcept
    {
        To to;
        RUN(to.init(from));

        if (pVersionsHierarchy[0] > getTargetVersion())
            RUN(base_class::convertOnStack(to, ctx))
        else
            RUN(DataProcessor::serializeData(to, ctx))

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

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convert(context::DData<D, PM>& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    constexpr uint32_t getTargetVersion() const noexcept { return m_targetVersion; }

protected:
    using from_type = Dummy; // placeholder

    static constexpr const uint32_t pVersionsHierarchy[1] = { traits::kInterfaceVersionMax };

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersionOnHeap(const from_type& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersionOnStack(const from_type& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        return Status::kErrorInternal;
    }

private:
    uint32_t m_targetVersion{ traits::kInterfaceVersionMax };
};


template<serialization_concepts::IsISerializableBased From, typename... NextFrom>
class FromVersionConverter<From, NextFrom...> : public FromVersionConverter<NextFrom...>
{
public:
    FromVersionConverter(uint32_t targetVersion)
        : FromVersionConverter<NextFrom...>(targetVersion)
    { }

    constexpr uint32_t getTargetVersion() const noexcept { return base_class::getTargetVersion(); }

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convert(context::DData<D, PM>& ctx, To& to) noexcept
    {
        // Skip versions that are older than serialized one
        if (base_class::pVersionsHierarchy[0] <= getTargetVersion())
            RUN(base_class::convert(ctx, to))
        else
        {
            if (ctx.isAuxUsingHeapAllocation())
                RUN(convertOnHeap(ctx, to))
            else
                RUN(convertOnStack(ctx, to))
        }

        return Status::kNoError;
    }

protected:
    using base_class = FromVersionConverter<NextFrom...>;
    using from_type = From;

    static constexpr const uint32_t* pVersionsHierarchy = From::kVersionsHierarchy;

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertOnHeap(context::DData<D, PM>& ctx, To& to) noexcept
    {
        GenericPointerKeeper pointerKeeper;
        if (!pointerKeeper.allocateAndConstruct<From, GenericAllocatorHelper<From, ConstructorNoexceptAllocator<From>>>(1))
            return Status::kErrorNoMemory;

        RUN(DataProcessor::deserializeData(ctx, *pointerKeeper.get<From>()));
        RUN(convertToUpperVersionOnHeap(*pointerKeeper.get<From>(), ctx, to));

        return Status::kNoError;
    }

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertOnStack(context::DData<D, PM>& ctx, To& to) noexcept
    {
        From from;
        RUN(DataProcessor::deserializeData(ctx, from));
        RUN(convertToUpperVersionOnStack(from, ctx, to));

        return Status::kNoError;
    }

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersionOnHeap(const From& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        if (base_class::pVersionsHierarchy[0] != traits::kInterfaceVersionMax)
        {
            using base_from = typename base_class::from_type;

            GenericPointerKeeper pointerKeeper;
            if (!pointerKeeper.allocateAndConstruct<base_from, GenericAllocatorHelper<base_from, ConstructorNoexceptAllocator<base_from>>>(1))
                return Status::kErrorNoMemory;

            if constexpr (InitableBySpecialClass<To, base_from>)
                RUN(pointerKeeper.get<base_from>()->init(from))
            else
                return Status::kErrorNoSuchHandler;

            RUN(base_class::convertToUpperVersionOnHeap(*pointerKeeper.get<base_from>(), ctx, to));
        }
        else if constexpr (InitableBySpecialClass<To, From>)
            RUN(to.init(from))
        else
            return Status::kErrorNoSuchHandler;

        return Status::kNoError;
    }

    template<serialization_concepts::IsISerializableBased To, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IDeserializationPointersMap PM>
    Status convertToUpperVersionOnStack(const From& from, context::DData<D, PM>& ctx, To& to) noexcept
    {
        if (base_class::pVersionsHierarchy[0] != traits::kInterfaceVersionMax)
        {
            using base_from = typename base_class::from_type;

            base_from bFrom;

            if constexpr (InitableBySpecialClass<To, base_from>)
                RUN(bFrom.init(from))
            else
                return Status::kErrorNoSuchHandler;

            RUN(base_class::convertToUpperVersionOnStack(bFrom, ctx, to));
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
