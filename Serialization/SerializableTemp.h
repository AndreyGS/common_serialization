/**
 * @file SerializableDefinition.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

#include "SerializableDeclaration.h"

namespace common_serialization
{


/*
template<typename T>
ptrdiff_t serialize(const void* input, RawData& rawData)
{

}

template<typename T>
class SerTest_Version0Impl : public Serializable<T>
{
    friend SerTestImpl<SerTest>;
public:
    int GetI() const noexcept { return m_i; }
    void SetI(int i) noexcept { m_i = i; }
    std::string& GetS() noexcept { return m_s; }
    const std::string& GetS() const noexcept { return m_s; }
    void SetS(const std::string& s) { m_s = s; }

protected:
    static constexpr uint64_t ancestors[5] = { 0 };
    static constexpr uint64_t members[5] = { 0 };           // should be represented by hash (thisStructTypeName + memberTypeName + fieldName) contain Type and offset
    static constexpr uint64_t nameHash = 0;
    static constexpr uint32_t versionThis = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t versionInterface = 0;         // latest version among all dependable struct

private:
    mutable uint32_t m_convertedFromVersion = versionThis;  // versionInterface of struct from which this instance was converted by deserialization

    int m_i{ 0 };
    int[10] m_s = { 0 };
};

class SerTest_Version0 : public SerTest_Version0Impl<SerTest>
{
public:
    using composite_type = std::true_type;
};

template<typename T>
class SerTestImpl : public Serializable<T>
{
public:
    int GetI() const noexcept { return m_i; }
    void SetI(int i) noexcept { m_i = i; }
    std::string& GetS() noexcept { return m_s; }
    const std::string& GetS() const noexcept { return m_s; }
    void SetS(const std::string& s) { m_s = s; }

    void ConvertToOld(RawData& rawData) noexcept;
    void ConvertToVersion0(RawData& rawData) noexcept;
    void ConvertFromVersion0(const RawData& rawData) noexcept;

protected:
    static constexpr uint64_t ancestors[5] = { 0 };
    static constexpr uint64_t members[5] = { 0 };           // should be represented by hash (thisStructTypeName + memberTypeName + fieldName) contain Type and offset
    static constexpr uint64_t nameHash = 0;
    static constexpr uint32_t versionThis = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t versionInterface = 0;         // latest version among all dependable struct

private:
    mutable uint32_t m_convertedFromVersion = versionThis;  // versionInterface of struct from which this instance was converted by deserialization

    IntWrapper<int> m_i; //int m_i{0};
    Vector<int> m_s;
};


void f(const SerTestImpl& t, RawData& output)
{
    output.push_back_n(reinterpret_cast<uint8_t*>(&t.m_i), sizeof(t.m_i));

}

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
