/**
 * @file cslib/include/common_serialization/CSP/MessagingServiceStructs.h
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

#include "common_serialization/CSP/ISerializable.h"

// Service structs not support version conversion and must always be the same
// Set is completely depend on protocol version

namespace common_serialization::csp::messaging
{

// Special type - placeholder for Input-Output operations that has no ISerializable Output struct
// Interfaces which are using that struct as an Ouput will always receive the Message::kStatus as result
template<typename T = Dummy>
struct ISerializableDummy : public ISerializable<ISerializableDummy<Dummy>>
{
public:
    using empty_type_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x60b0dd3e, 0x7d2e, 0x42d9, 0xb00d, 0x90f7ecd19d25);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

template<typename T = Dummy>
struct SupportedProtocolVersions : public csp::ISerializable<GetCrtpMainType<SupportedProtocolVersions<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SupportedProtocolVersions<T>, T>;

    static constexpr Uuid kId = helpers::getUuid(0xc665f5c8, 0xa6c2, 0x4f75, 0xb84d, 0x41cff3c47578);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    Vector<protocol_version_t> list;
};

template<typename T = Dummy>
struct GetInterfaceProperties : public csp::ISerializable<GetCrtpMainType<GetInterfaceProperties<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfaceProperties<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x08c68657, 0x4fa7, 0x4419, 0x8c13, 0x66aec2b06cb0);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    Uuid id;
};

template<typename T = Dummy>
struct OurGetInterfaceProperties : public csp::ISerializable<GetCrtpMainType<OurGetInterfaceProperties<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<OurGetInterfaceProperties<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x22bd67db, 0x65a0, 0x42f4, 0xb28b, 0x63c6181aebe1);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    traits::InterfaceProperties properties;
};

template<typename T = Dummy>
struct GetInterfacesList : public csp::ISerializable<GetCrtpMainType<GetInterfacesList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfacesList<T>, T>;
    using empty_type_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0xd5b61816, 0xcc92, 0x4e18, 0xb287, 0xfcc4ed1e9f2a);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

template<typename T = Dummy>
struct InterfacesList : public csp::ISerializable<GetCrtpMainType<InterfacesList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfacesList<T>, T>;

    static constexpr Uuid kId = helpers::getUuid(0xb5346ff9, 0xe557, 0x40af, 0x8fb9, 0x5563fa97700e);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    bool operator==(const InterfacesList& rhs) const noexcept
    {
        return list == rhs.list;
    }

    Vector<traits::InterfaceProperties> list;
};

} // namespace common_serialization::csp::messaging
