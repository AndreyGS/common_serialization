#include <gtest/gtest.h>
#include "Containers/special_types.h"
#include "Containers/Vector.h"
#include "Allocators/RawKeeperAllocator.h"

using namespace special_types;
using namespace common_serialization;

namespace
{

template<typename T>
using ConstructorVectorAllocatorHelper = GenericAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawKeeperAllocator>>;

using size_type = typename Vector<int, ConstructorVectorAllocatorHelper<int>>::size_type;

constexpr size_t kArraySizeInBytes = 307;
uint8_t g_array[kArraySizeInBytes] = { 0 };

TEST(RawKeeperAllocatorTest, VectorWorkTest)
{
    Vector<int, ConstructorVectorAllocatorHelper<int>> vec;

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.max_size(), 0);
    EXPECT_EQ(vec.data(), nullptr);

    constexpr size_type kExpectedMaxSize = kArraySizeInBytes / sizeof(int);

    vec.getAllocatorHelper().getAllocator().setStorage(g_array, 307);
    EXPECT_EQ(vec.max_size(), kExpectedMaxSize);

    vec.push_back(0);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(reinterpret_cast<void*>(vec.data()), reinterpret_cast<void*>(g_array));

    vec.clear();

    // try to add more than we can hold
    for (size_type i = 0; i < kExpectedMaxSize + 1; ++i)
        vec.push_back(i);
    
    EXPECT_EQ(vec.size(), kExpectedMaxSize);
    EXPECT_EQ(vec.capacity(), kExpectedMaxSize);

    for (size_type i = 0; i < kExpectedMaxSize; ++i)
        EXPECT_EQ(vec[i], i);
}



} // namespace anonymous
