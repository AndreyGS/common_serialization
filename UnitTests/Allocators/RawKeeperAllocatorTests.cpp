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

using size_type = typename Vector<int, RawKeeperAllocator<int>>::size_type;

constexpr size_t kArraySizeInBytes = 307;
int g_array[kArraySizeInBytes] = { 0 };

TEST(RawKeeperAllocatorTest, VectorWorkTest)
{
    Vector<int, ConstructorVectorAllocatorHelper<int>> vec;

    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.max_size(), 0);
    EXPECT_EQ(vec.data(), nullptr);

    vec.getAllocatorHelper().getAllocator().setStorage(g_array, kArraySizeInBytes);
    EXPECT_EQ(vec.max_size(), kArraySizeInBytes);

    vec.push_back(0);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(reinterpret_cast<void*>(vec.data()), reinterpret_cast<void*>(g_array));

    vec.clear();

    // try to add more than we can hold
    for (int i = 0; i < kArraySizeInBytes + 1; ++i)
        vec.push_back(i);
    
    EXPECT_EQ(vec.size(), kArraySizeInBytes);
    EXPECT_EQ(vec.capacity(), kArraySizeInBytes);

    for (int i = 0; i < kArraySizeInBytes; ++i)
        EXPECT_EQ(vec[i], i);
}

#include <vector>

TEST(RawKeeperAllocatorTest, VectorWorkTest2)
{
    std::vector<std::string, ConstructorNoexceptAllocator<std::string>> str;
    str.push_back("123");
}


/*
constexpr auto getContainer()
{
    Vector<int, ConstructorVectorAllocatorHelper<int>> vec;
    constexpr uint8_t localArray[35] = { 0 };
    vec.getAllocatorHelper().getAllocator().setCStorage(localArray, 35);
    for (int i = 0; i < 35; ++i)
        vec.push_back(i);

    return vec;
}

constexpr auto g_vec = getContainer();

TEST(RawKeeperAllocatorTest, VectorWorkTest2)
{
    for (int i = 0; i < 35; ++i)
        EXPECT_EQ(g_vec[i], i);
}
*/

} // namespace anonymous
