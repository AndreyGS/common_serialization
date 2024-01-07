/**
 * @file cslib/include/common_serialization/Containers/Concepts.h
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

#include "common_serialization/Status.h"

namespace common_serialization
{

class GenericPointerKeeper;

/// @brief Interface of container that holds
///     GenericPointerKeeper objects
template<typename S>
concept IGenericPointersKeeperContainer
    =  requires(S e)
         {
             typename S::value_type;
             typename S::constructor_allocator;

             { e.clear() };
             { e.begin() };
             { e.end() };
             { e.erase(0, 1) };
             { e.data() } -> std::same_as<typename S::value_type*>;
             { e.size() } -> std::same_as<typename S::size_type>;
             { e.capacity() } -> std::same_as<typename S::size_type>;

             { e.reserve(1) } -> std::same_as<Status>;
             { e.pushBack(*(new GenericPointerKeeper)) } -> std::same_as<Status>;
         } 
    && std::is_same_v<typename S::value_type, GenericPointerKeeper> && std::is_same_v<typename S::constructor_allocator, std::true_type>;

} // namespace common_serialization
