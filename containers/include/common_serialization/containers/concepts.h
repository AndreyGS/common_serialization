/**
 * @file common_serialization/containers/concepts.h
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

#include <common_serialization/common/common.h>

namespace common_serialization
{

template<typename T1, typename D1, typename T2, typename D2>
concept SmartPtrArrConvertible =std::is_convertible_v<T2*, T1*>
                            && (std::is_reference_v<D1> && std::is_same_v<D2, D1> || !std::is_reference_v<D1> && std::is_convertible_v<D2, D1>)
                            && (std::is_same_v<T2, T1> || std::has_virtual_destructor_v<T1>);

template<typename T1, typename D1, typename T2, typename D2>
concept SmartPtrConvertible =   std::is_convertible_v<T2*, T1*>
                            && (std::is_reference_v<D1> && std::is_same_v<D2, D1> || !std::is_reference_v<D1> && std::is_convertible_v<D2, D1>)
                            && (std::is_same_v<T2, T1> || std::has_virtual_destructor_v<T1> || HasDestroyingDeleteOp<T1>);

} // namespace common_serialization
