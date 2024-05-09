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

/// @brief Simple RAII guard for shared mutex
/// @tparam _T Mutex which implement ISharedMutex interface
template<ISharedMutex _T>
class RGuard
{
public:
    /// @brief Init ctor
    /// @param mutex Shared mutex
    explicit RGuard(_T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock_shared();
    }

    RGuard(const RGuard&) = delete;
    RGuard(RGuard&&) = delete;
    RGuard& operator=(const RGuard&) = delete;
    RGuard& operator=(RGuard&&) = delete;

    ~RGuard()
    {
        m_mutex.unlock_shared();
    }

private:
    _T& m_mutex;
};

/// @brief Simple RAII guard for exclusive mutex
/// @tparam _T Mutex which implement IExclusiveMutex interface
template<IExclusiveMutex _T>
class WGuard
{
public:
    /// @brief Init ctor
    /// @param mutex Exclusive mutex
    explicit WGuard(_T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    WGuard(const WGuard&) = delete;
    WGuard(WGuard&&) = delete;
    WGuard& operator=(const WGuard&) = delete;
    WGuard& operator=(WGuard&&) = delete;

    ~WGuard()
    {
        m_mutex.unlock();
    }

private:
    _T& m_mutex;
};

} // namespace common_serialization
