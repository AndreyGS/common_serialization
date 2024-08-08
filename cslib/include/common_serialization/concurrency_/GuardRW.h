/**
 * @file cslib/include/common_serialization/concurrency_/GuardRW.h
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

#include <common_serialization/concurrency_/Concepts.h>

namespace common_serialization
{
    
/// @brief Simple RAII guard for shared or unique lockable type
/// @tparam _T Lockable
template<IExclusiveLockableImpl _T, bool exclusive = true>
class Guard
{
public:
    // For some reason I've got compiler internal error 
    // when using requires clause with (exclusive || ISharedLockableImpl<_T>)
    static_assert(exclusive || ISharedLockableImpl<_T>);

    /// @brief Init ctor
    /// @param lockable Lockable
    /// @param deferred Should be locked later
    explicit Guard(_T& lockable, bool deferred = false)
        : m_lockable(lockable), m_locked(false)
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
        if (m_locked)
            return;

        if constexpr (exclusive)
            m_lockable.lock();
        else
            m_lockable.lock_shared();

        m_locked = true;
    }

    void unlock()
    {
        if (!m_locked)
            return;

        if constexpr (exclusive)
            m_lockable.unlock();
        else
            m_lockable.unlock_shared();

        m_locked = false;
    }

    ~Guard()
    {
        unlock();
    }

private:
    _T& m_lockable;
    bool m_locked{ false };
};

template<ISharedLockableImpl _T>
using RGuard = Guard<_T, false>;

template<IExclusiveLockableImpl _T>
using WGuard = Guard<_T, true>;

} // namespace common_serialization
