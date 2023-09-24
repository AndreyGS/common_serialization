#include <gtest/gtest.h>
#include "Containers/special_types.h"
#include "Containers/Vector.h"
#include "Allocators/RawKeeperAllocator.h"

using namespace special_types;
using namespace common_serialization;

namespace
{

template<typename T>
using ConstructorVectorAllocatorHelper = GenericAllocatorHelper<T, RawKeeperAllocator<T>>;

using size_type = typename Vector<int, ConstructorVectorAllocatorHelper<int>>::size_type;

constexpr size_t kArraySize = 3;
int g_array[kArraySize] = { 0 };

TEST(RawKeeperAllocatorTest, VectorWorkTest)
{
    Vector<int, ConstructorVectorAllocatorHelper<int>> vec;

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.max_size(), 0);
    EXPECT_EQ(vec.data(), nullptr);

    vec.getAllocatorHelper().getAllocator().setStorage(g_array, kArraySize);
    EXPECT_EQ(vec.max_size(), kArraySize);

    vec.pushBack(0);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(reinterpret_cast<void*>(vec.data()), reinterpret_cast<void*>(g_array));

    vec.clear();

    // try to add more than we can hold
    for (int i = 0; i < kArraySize + 1; ++i)
        vec.pushBack(i);
    
    EXPECT_EQ(vec.size(), kArraySize);
    EXPECT_EQ(vec.capacity(), kArraySize);

    for (int i = 0; i < kArraySize; ++i)
        EXPECT_EQ(vec[i], i);
}

} // namespace anonymous
