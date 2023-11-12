/**
 * @file GenericPointerKeeper.h
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

#include "../Serialization/SerializationConcepts.h"

namespace common_serialization
{

// This class is something like unique_ptr, but not template.
// Its interface was intentionally made different from std analogue.
// At the moment role of this class only in keeping random pointers in container
// and its interface is not allowing to get pointer or destructor once it (pointer) created
class GenericPointerKeeper
{
public:
    using DestroyAndDeallocateFunc = void(*)(void*, size_t);

    GenericPointerKeeper() noexcept { }
    GenericPointerKeeper(const GenericPointerKeeper& rhs) = delete; // for now
    GenericPointerKeeper(GenericPointerKeeper&& rhs) noexcept
        : m_p(rhs.m_p), m_destroyAndDeallocate(rhs.m_destroyAndDeallocate)
    { 
        rhs.m_p = nullptr, rhs.m_destroyAndDeallocate = nullptr;
    }
    GenericPointerKeeper& operator=(const GenericPointerKeeper& rhs) = delete; // for now
    GenericPointerKeeper& operator=(GenericPointerKeeper&& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        if (m_p)
            destroyAndDeallocate();

        m_p = rhs.m_p, m_destroyAndDeallocate = rhs.m_destroyAndDeallocate;
        rhs.m_p = nullptr, rhs.m_destroyAndDeallocate = nullptr;
    }
    ~GenericPointerKeeper()
    {
        destroyAndDeallocate();
    }

    template<typename T, typename AllocatorHelper, typename... Args>
    T* allocateAndConstruct(size_t n, Args&&... args)
    {
        if (m_p)
            destroyAndDeallocate();

        AllocatorHelper allocatorHelper{};

        if (n > allocatorHelper.getAllocator().max_size())
            return nullptr;

        T* p = allocatorHelper.allocateAndConstruct(n, nullptr, std::forward<Args>(args)...);
        if (p)
        {
            m_p = p;
            m_size = n;
            m_destroyAndDeallocate = reinterpret_cast<DestroyAndDeallocateFunc>(&destroyAndDeallocateHelper<T, AllocatorHelper>);
        }

        return p;
    }

    void destroyAndDeallocate() noexcept
    {
        if (m_p)
        {
            m_destroyAndDeallocate(m_p, m_size);
            m_p = nullptr;
            m_size = 0;
        }
    }

    template<typename T>
    T* get()
    {
        return static_cast<T*>(m_p);
    }

private:
    template<typename T, typename AllocatorHelper>
    static void destroyAndDeallocateHelper(T* p, size_t n)
    {
        AllocatorHelper().destroyAndDeallocate(p, n);
    }

    void* m_p{ nullptr };
    size_t m_size{ 0 };
    DestroyAndDeallocateFunc m_destroyAndDeallocate{ nullptr };
};

} // namespace common_serialization
