/**
 * @file cslib/include/common_serialization/CSP/MessagingCommonServer.h
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

namespace common_serialization::csp::messaging
{

// Special type - placeholder for Input-Output operations that has no ISerializable Output struct
// Interfaces which are using that struct as an Ouput will always receive the Message::kStatus as result
struct ISerializableDummy : public ISerializable<Dummy>
{
public:
    using empty_type_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x60b0dd3e, 0x7d2e, 0x42d9, 0xb00d, 0x90f7ecd19d25);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

template<typename T = Dummy>
struct GetInterfaceVersion : public csp::ISerializable<GetCrtpMainType<GetInterfaceVersion<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfaceVersion<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x166b4718, 0x9918, 0x462b, 0x8416, 0xd4f7b66a288d);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    Uuid id;
};

template<typename T = Dummy>
struct InterfaceVersion : public csp::ISerializable<GetCrtpMainType<InterfaceVersion<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfaceVersion<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x22bd67db, 0x65a0, 0x42f4, 0xb28b, 0x63c6181aebe1);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    csp::interface_version_t version{ 0 };
};

template<typename T = Dummy>
struct GetInterfacesGroupVersion : public csp::ISerializable<GetCrtpMainType<GetInterfacesGroupVersion<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfacesGroupVersion<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x08c68657, 0x4fa7, 0x4419, 0x8c13, 0x66aec2b06cb0);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    Uuid id;
};

template<typename T = Dummy>
struct InterfacesGroupVersion : public csp::ISerializable<GetCrtpMainType<InterfacesGroupVersion<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfacesGroupVersion<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0x3b44e3c2, 0x9e9d, 0x4adb, 0xa068, 0x60a7cad9d93d);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    csp::interface_version_t interfacesGroupVersion{ 0 };
};

template<typename T = Dummy>
struct GetInterfacesGroupList : public csp::ISerializable<GetCrtpMainType<GetInterfacesGroupList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<GetInterfacesGroupList<T>, T>;
    using empty_type_tag = std::true_type;

    static constexpr Uuid kId = helpers::getUuid(0xd5b61816, 0xcc92, 0x4e18, 0xb287, 0xfcc4ed1e9f2a);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

struct InterfacesGroupTraits
{
    Uuid id;
    csp::interface_version_t interfacesGroupVersion{ 0 };
};

template<typename T = Dummy>
struct InterfacesGroupList : public csp::ISerializable<GetCrtpMainType<InterfacesGroupList<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<InterfacesGroupList<T>, T>;

    static constexpr Uuid kId = helpers::getUuid(0xb5346ff9, 0xe557, 0x40af, 0x8fb9, 0x5563fa97700e);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    Vector<InterfacesGroupTraits> list;
};

} // namespace common_serialization::csp::messaging
