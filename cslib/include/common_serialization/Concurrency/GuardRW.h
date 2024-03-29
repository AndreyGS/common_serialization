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

/// @brief Simple RAII guard for RW mutex
/// @tparam SM Mutex which implement ISharedMutex interface
/// @tparam write Flag that indicates that guard required for write
template<ISharedMutex SM, bool write>
class GuardRW
{
public:
    /// @brief Init constructor
    /// @param sharedMutex Managed mutex
    GuardRW(SM& sharedMutex)
        : m_sharedMutex(sharedMutex)
    {
        if constexpr (write)
            m_sharedMutex.lock();
        else
            m_sharedMutex.lock_shared();
    }

    ~GuardRW()
    {
        if constexpr (write)
            m_sharedMutex.unlock();
        else
            m_sharedMutex.unlock_shared();
    }

private:
    SM& m_sharedMutex;
};

/// @brief Shared read GuardRW<>
/// @tparam SM Mutex which implement ISharedMutex interface
template<ISharedMutex SM>
using RGuard = GuardRW<SM, false>;

/// @brief Exclusive Write GuardRW<>
/// @tparam SM Mutex which implement ISharedMutex interface
template<ISharedMutex SM>
using WGuard = GuardRW<SM, true>;

} // namespace common_serialization
