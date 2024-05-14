/**
 * @file cslib/include/common_serialization/Concurrency/GuardRW.h
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

#include "common_serialization/Concurrency/Concepts.h"

namespace common_serialization
{

/// @brief Simple RAII guard for shared or unique mutex
/// @tparam _T Mutex which implement ISharedMutex interface
template<typename _T, bool exclusive = true>
class Guard
{
public:
    /// @brief Init ctor
    /// @param mutex Mutex
    /// @param deferred Should be locked later
    explicit Guard(_T& mutex, bool deferred = false)
        : m_mutex(mutex), locked(false)
    {
        if (!deferred)
            lock();
    }

    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    Guard& operator=(const Guard&) = delete;
    Guard& operator=(Guard&&) = delete;

    void lock()
    {
        if (locked)
            return;

        if constexpr (exclusive)
            m_mutex.lock();
        else
            m_mutex.lock_shared();

        locked = true;
    }

    void unlock()
    {
        if (!locked)
            return;

        if constexpr (exclusive)
            m_mutex.unlock();
        else
            m_mutex.unlock_shared();

        locked = false;
    }

    ~Guard()
    {
        unlock();
    }

private:
    _T& m_mutex;
    bool locked{ false };
};

template<typename _T>
using RGuard = Guard<_T, false>;

template<typename _T>
using WGuard = Guard<_T, true>;

} // namespace common_serialization
