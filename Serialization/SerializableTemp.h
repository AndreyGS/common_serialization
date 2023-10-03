/**
 * @file SerializableTemp.h
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

#include <unordered_map> // temporary header
#include <string>
#include <tuple>
#include <stdint.h>
#include <vector>

#include "ISerializable.h"

namespace common_serialization
{

template<typename T = Dummy>
class SerT : public ISerializable<GetCrtpMainType<SerT<T>, T>>
{
private:
    int i = 5;

private:
    friend ISerializable<SerT<T>>;
    template<serializable_concepts::ISerializationCapableContainer S>
    friend Status serializeThis(const SerT<Dummy>& value, S& output);
    template<serializable_concepts::IDeserializationCapableContainer D>
    friend Status deserializeThis(D& input, SerT<>& value);

    using instance_type = std::conditional_t<std::is_same_v<T, Dummy>, SerT<T>, T>;

    static constexpr uint64_t kAncestors[5] = { 0 };
    static constexpr uint64_t kMembers[5] = { 0 };
    static constexpr uint64_t kNameHash = 0;
    static constexpr uint32_t kThisVersion = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kInterfaceVersion = 0;         // latest version among all dependable struct
};

template<typename T = Dummy>
class SerT2 : public ISerializable<GetCrtpMainType<SerT2<T>, T>>
{
private:
    int k = 15;

private:
    friend ISerializable<SerT2<T>>;
    template<serializable_concepts::ISerializationCapableContainer S>
    friend Status serializeThis(const SerT2<Dummy>& value, S& output);
    template<serializable_concepts::IDeserializationCapableContainer D>
    friend Status deserializeThis(D& input, SerT2<>& value);

    using instance_type = std::conditional_t<std::is_same_v<T, Dummy>, SerT2<T>, T>;

    static constexpr uint64_t kAncestors[5] = { 0 };
    static constexpr uint64_t kMembers[5] = { 0 };
    static constexpr uint64_t kNameHash = 0;
    static constexpr uint32_t kThisVersion = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kInterfaceVersion = 0;         // latest version among all dependable struct
};

template<typename T = Dummy>
class SerTInh : public SerT<SerTInh<T>>, public SerT2<SerTInh<T>>
{
public:
    SerT<Dummy>& getSerT() noexcept
    {
        return *static_cast<SerT<Dummy>*>(static_cast<void*>(static_cast<SerT<SerTInh<T>>*>(this)));
    }

    const SerT<Dummy>& getSerT() const noexcept
    {
        return *static_cast<const SerT<Dummy>*>(static_cast<const void*>(static_cast<const SerT<SerTInh<T>>*>(this)));
    }

    SerT2<Dummy>& getSerT2() noexcept
    {
        return *static_cast<SerT2<Dummy>*>(static_cast<void*>(static_cast<SerT2<SerTInh<T>>*>(this)));
    }

    const SerT2<Dummy>& getSerT2() const noexcept
    {
        return *static_cast<const SerT2<Dummy>*>(static_cast<const void*>(static_cast<const SerT2<SerTInh<T>>*>(this)));
    }
    uint32_t arr[5] = { 0 };

    //using simple_assignable = std::true_type;

private:
    int j = 10;
    

private:
    friend ISerializable<SerTInh<T>>;
    template<serializable_concepts::ISerializationCapableContainer S>
    friend Status serializeThis(const SerTInh<Dummy>& value, S& output);
    template<serializable_concepts::IDeserializationCapableContainer D>
    friend Status deserializeThis(D& input, SerTInh<>& value);

    using instance_type = std::conditional_t<std::is_same_v<T, Dummy>, SerTInh<T>, T>;

    static constexpr uint64_t kAncestors[5] = { 0 };
    static constexpr uint64_t kMembers[5] = { 0 };
    static constexpr uint64_t kNameHash = 0;
    static constexpr uint32_t kThisVersion = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kInterfaceVersion = 0;         // latest version among all dependable struct
};

template<serializable_concepts::ISerializationCapableContainer S>
Status serializeThis(const SerT<>& value, S& output)
{
    RUN(serializeThis(value.i, output));

    return Status::kNoError;
}

template<serializable_concepts::ISerializationCapableContainer S>
Status serializeThis(const SerT2<>& value, S& output)
{
    RUN(serializeThis(value.k, output));

    return Status::kNoError;
}

template<serializable_concepts::ISerializationCapableContainer S>
Status serializeThis(const SerTInh<>& value, S& output)
{
    RUN(serializeThis(value.getSerT(), output));
    RUN(serializeThis(value.getSerT2(), output));
    RUN(serializeThis(value.j, output));
    RUN(serializeThis(value.arr, output));

    return Status::kNoError;
}

/*
class SerTest : public SerTestImpl<SerTest>
{
public:
    using composite_type = std::true_type;
};

template<typename T>
void SerTestImpl<T>::ConvertToOld(RawData& output) noexcept
{
    RawData thisData;
    this->serialize(thisData);

    ConvertToVersion0(thisData);
}

template<typename T>
void SerTestImpl<T>::ConvertToVersion0(RawData& output) noexcept
{
    SerTest_Version0Impl<T> oldData;
    oldData.deserialize(thisData);

    oldData.m_s = this->m_s;

    oldData.serialize(output);
}*/

} // namespace common_serialization
