/**
 * @file cslib/include/common_serialization/Containers/UniquePtr.h
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

#include "common_serialization/helpers.h"
#include "common_serialization/Allocators/AllocatorHelpers/GenericAllocatorHelper.h"

namespace common_serialization
{

/// @brief Something like std::unique_ptr
/// @note It's class half-finished article, should be upgraded later
/// @tparam T Type that internal pointer points to
/// @tparam D Custom deleter
template<typename T, typename D = Dummy>
class UniquePtr
{
public:
    UniquePtr(T* p) noexcept : m_p(p) {}

    ~UniquePtr() noexcept
    {
        if constexpr (std::is_same_v<D, Dummy>)
            delete m_p;
        else
            D(m_p);
    }

    /// @brief Get stored pointer
    /// @return Stored pointer
    [[nodiscard]] constexpr T* get() const noexcept { return m_p; }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    constexpr T* release() noexcept
    {
        T* p = m_p;
        m_p = nullptr;
        return p;
    }

    [[nodiscard]] constexpr T* operator->() const noexcept
    { 
        return m_p; 
    }

    [[nodiscard]] constexpr T& operator*() const noexcept
    { 
        return *m_p; 
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_p);
    }

private:
    T* m_p{ nullptr };
};

} // namespace common_serialization
