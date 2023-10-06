project(common_serialization_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/9fce5480448488e17a50bcbf88d2f3bdb637ad6c.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(containers_tests "Containers/VectorTests.cpp" "Containers/WalkerTests.cpp")
target_link_libraries(containers_tests PUBLIC common_options PRIVATE gtest_main)
target_include_directories(containers_tests PRIVATE "${PROJECT_SOURCE_DIR}")

add_executable(allocators_tests "Allocators/RawKeeperAllocatorTests.cpp")
target_link_libraries(allocators_tests PUBLIC common_options PRIVATE gtest_main)
target_include_directories(allocators_tests PRIVATE "${PROJECT_SOURCE_DIR}")

add_executable(serialization_tests "Serialization/SerializationNoFlagsTests.cpp")
target_link_libraries(serialization_tests PUBLIC common_options PRIVATE gtest_main)
target_include_directories(serialization_tests PRIVATE "${PROJECT_SOURCE_DIR}")
