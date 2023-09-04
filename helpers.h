#pragma once

namespace common_serialization
{

namespace helpers
{

template<typename T>
constexpr inline bool areRegionsOverlap(const T* objs1, const T* objs2, size_t n)
{
    return objs1 > objs2 && objs1 < objs2 + n || objs1 < objs2 && objs1 + n > objs2;
}

} // namespace helpers

} // namespace common_serialization