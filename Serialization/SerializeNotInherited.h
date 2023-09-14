/**
 * @file SerializeNotInherited.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

#include "Serializable.h"

namespace common_serialization
{

// common function for  types
template<typename T>
    requires std::is_arithmetic_v<T>
int serialize(T value, RawData& output)
{
    if (!output.reserve_from_current_offset(sizeof(T)))
        return ERROR_NO_MEMORY;


}


template<>
int serialize(const Vector<T> RawData& output)
{
    size_type size = output.size();

    if (output.reserve_from_current_offset(sizeof(size)))
    {
        *static_cast<size_type*>(static_cast<void*>(output.data() + output.tell())) = size;
        output.seek(output.tell() + sizeof(size));

        if constexpr (SerializableEmptyType<T>)
        {
            return 0;
        }
        else if constexpr (SerializableCompositeType<T>)
            for (auto& value : *this)
            {
                if (value.serialize(output) <= 0)
                    return 1;
            }
        else
        {
            output.write(*static_cast<uint8_t*>(static_cast<void*>(output.data())), size * sizeof(T));
        }
    }
}

} // common_serialization
