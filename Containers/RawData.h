/**
 * @file RawData.h
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

#include "Walker.h"

namespace common_serialization
{

template<typename AllocatorHelper = StrategicAllocatorHelper<uint8_t, ConstructorNoexceptAllocator<uint8_t>>>
class RawData : public Walker<uint8_t, AllocatorHelper>
{
public:
    using size_type = typename Walker<uint8_t, AllocatorHelper>::size_type;

    template<typename T>
    size_type addArithmeticValue(T value)
        requires std::is_arithmetic_v<T>;

    template<typename T>
    size_type addArithmeticValuesArray(const T* p, size_type arraySize)
        requires std::is_arithmetic_v<std::remove_pointer_t<T>>;
};

template<typename AllocatorHelper>
template<typename T>
RawData<AllocatorHelper>::size_type RawData<AllocatorHelper>::addArithmeticValue(T value)
    requires std::is_arithmetic_v<T>
{
    if (!this->reserve_from_current_offset(sizeof(T)))
        return 0;

    *static_cast<T*>(static_cast<void*>(this->data() + this->tell())) = value;
    this->seek(this->tell() + sizeof(T));

    return sizeof(T);
}


template<typename AllocatorHelper>
template<typename T>
RawData<AllocatorHelper>::size_type RawData<AllocatorHelper>::addArithmeticValuesArray(const T* p, size_type arraySize)
    requires std::is_arithmetic_v<std::remove_pointer_t<T>>
{
    size_type dataSize = arraySize * sizeof(T);

    if (dataSize / arraySize != sizeof(T))
        return 0;
    
    if (!this->reserve_from_current_offset(dataSize))
        return 0;

    memcpy(this->data() + this->tell(), p, dataSize);
    this->seek(this->tell() + dataSize);

    return sizeof(T);
}

} // namespace common_serialization
