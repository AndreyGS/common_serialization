/**
 * @file cslib/include/common_serialization/Containers/GenericPointerKeeper.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

namespace common_serialization
{

// This class is something like unique_ptr, but not template.

/// @brief Container for keeping any pointer type
/// @details This class is something like unique_ptr, but not template
///     and this property allows us to use it in any other containers,
///     like Vector and others. Of course you should know the type
///     of pointer which currently hold GenericPointerKeeper instance
///     if you whish to get it from here. But most important, that
///     this container implements RAII and properly destroys every
///     object that it holds wherever it's needed.
/// @remark Current interface of GenericPointerKeeper is pretty short
///     and meets only those needs that were required when working on CSP.
///     In future it should be extended.
class GenericPointerKeeper
{
public:
    /// @brief Function that destroys and deallocates holding pointer
    using DestroyAndDeallocateFunc = void(*)(void*, size_t);

    GenericPointerKeeper() = default;

    /// @brief Copy ctor
    /// @remark I'm not sure about necessity of this costructor,
    ///     even in future
    GenericPointerKeeper(const GenericPointerKeeper&) = delete;

    GenericPointerKeeper(GenericPointerKeeper&& rhs) noexcept
        : m_p(rhs.m_p), m_size(rhs.m_size), m_destroyAndDeallocate(rhs.m_destroyAndDeallocate)
    { 
        rhs.m_p = nullptr, rhs.m_size = 0, rhs.m_destroyAndDeallocate = nullptr;
    }

    /// @brief Copy assignment operator
    /// @remark I'm not sure about necessity of this operator,
    ///     even in future
    GenericPointerKeeper& operator=(const GenericPointerKeeper&) = delete;

    GenericPointerKeeper& operator=(GenericPointerKeeper&& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        if (m_p)
            destroyAndDeallocate();

        m_p = rhs.m_p;
        m_size = rhs.m_size;
        m_destroyAndDeallocate = rhs.m_destroyAndDeallocate;

        rhs.m_p = nullptr;
        rhs.m_size = 0;
        rhs.m_destroyAndDeallocate = nullptr;
    }

    ~GenericPointerKeeper()
    {
        destroyAndDeallocate();
    }

    /// @brief Allocates storage and constructs an object 
    ///     or array of objects using supplied arguments
    /// @tparam T Type of object(s) to create
    /// @tparam AllocatorHelper Type that implements AllocatorHelper interface
    /// @tparam ...Args Parameters types that go to ctor of every element
    /// @param n 
    /// @param ...args Parameters that go to ctor of every element
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    ///     or if object construction process return error.
    template<typename _T, typename _AllocatorHelper, typename... _Args>
    _T* allocateAndConstruct(size_t n, _Args&&... args)
    {
        if (m_p)
            destroyAndDeallocate();

        _AllocatorHelper allocatorHelper{};

        if (n > allocatorHelper.getAllocator().max_size())
            return nullptr;

        _T* p = allocatorHelper.allocateAndConstruct(n, nullptr, std::forward<_Args>(args)...);
        if (p)
        {
            m_p = p;
            m_size = n;
            m_destroyAndDeallocate = reinterpret_cast<DestroyAndDeallocateFunc>(&destroyAndDeallocateHelper<_T, _AllocatorHelper>);
        }

        return p;
    }

    template<typename _T, typename... _Args>
    _T* allocateAndConstructOne(_Args&&... args)
    {
        return allocateAndConstruct<_T, CGenericAllocatorHelperT<_T>, _Args...>(1, std::forward<_Args>(args)...);
    }

    /// @brief Destroys holding objects 
    ///     and deallocates storage that they used
    void destroyAndDeallocate() noexcept
    {
        if (m_p)
        {
            m_destroyAndDeallocate(m_p, m_size);
            m_p = nullptr;
            m_size = 0;
        }
    }

    /// @brief Get stored pointer
    /// @tparam T Type on which pointer points
    /// @return Stored pointer
    template<typename _T>
    constexpr _T* get()
    {
        return static_cast<_T*>(m_p);
    }

private:
    template<typename _T, typename _AllocatorHelper>
    static void destroyAndDeallocateHelper(_T* p, size_t n)
    {
        _AllocatorHelper().destroyAndDeallocate(p, n);
    }

    void* m_p{ nullptr };
    size_t m_size{ 0 };
    DestroyAndDeallocateFunc m_destroyAndDeallocate{ nullptr };
};

} // namespace common_serialization
