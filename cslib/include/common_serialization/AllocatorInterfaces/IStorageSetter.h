/**
 * @file cslib/include/common_serialization/AllocatorInterfaces/IStorageSetter.h
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

#include <common_serialization/Common/common.h>

namespace common_serialization
{

/// @brief Interface of Allocator
/// @tparam _Setter Most derived class (instance type)
template<typename _Setter>
class IStorageSetter
{
public:
    /// @brief Set storage
    /// @tparam _P Ponter type of storage
    /// @param p Pointer to storage
    /// @param size Size of storage in type units
    /// @returns True if successful, false otherwise
    template<typename _P>
        requires (std::is_pointer_v<_P> || std::is_same_v<_P, decltype(nullptr)>)
    CS_ALWAYS_INLINE constexpr bool setStorage(_P p, size_t size) noexcept
    {
        return static_cast<_Setter*>(this)->setStorageImpl(p, size);
    }
};

template<typename _Setter>
concept IStorageSetterImpl = std::is_base_of_v<IStorageSetter<normalize_t<_Setter>>, normalize_t<_Setter>>;

} // namespace common_serialization
