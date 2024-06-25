project(common_serialization_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/9fce5480448488e17a50bcbf88d2f3bdb637ad6c.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(containers_tests "Tests/Containers/VectorTests.cpp" "Tests/Containers/WalkerTests.cpp" "Tests/Containers/UniquePtrTests.cpp")
target_compile_definitions(containers_tests PRIVATE USER_MODE)
target_link_libraries(containers_tests PRIVATE cslib ForTestsHelpers PRIVATE gtest_main)
target_include_directories(containers_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(containers_tests PRIVATE "Tests/Containers/pch.h")

add_executable(allocators_tests "Tests/Allocators/RawKeeperAllocatorTests.cpp" "Tests/Allocators/RawNoexceptAllocatorTests.cpp" "Tests/Allocators/ConstructorNoexceptAllocatorTests.cpp")
target_compile_definitions(allocators_tests PRIVATE USER_MODE)
target_link_libraries(allocators_tests PRIVATE cslib ForTestsHelpers PRIVATE gtest_main)
target_include_directories(allocators_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(allocators_tests PRIVATE "Tests/Allocators/pch.h")

add_executable(csp_tests
    "Tests/csp/processing/ISerializableAlignmentMayBeNotEqualTests.cpp"
    "Tests/csp/processing/ISerializableAllowUnmanagedPointersTests.cpp"
    "Tests/csp/processing/ISerializableBasicModeTests.cpp"
    "Tests/csp/processing/ISerializableCheckRecursivePointersTests.cpp"
    "Tests/csp/processing/ISerializableInterfaceVersionsNotMatchTests.cpp"
    "Tests/csp/processing/ISerializableMultiModesTests.cpp"
    "Tests/csp/processing/ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff.cpp"
    "Tests/csp/processing/ISerializableSizeOfPrimitivesMayBeNotEqualTests.cpp"
    "Tests/csp/processing/StdIncludedTests.cpp"
    "Tests/csp/processing/ISerializableAnotherEndianness.cpp" 
    "Tests/csp/processing/ISerializableAnotherBitness.cpp"
    "Tests/csp/messaging/MessagingTests.cpp"
 "Tests/csp/messaging/ClientTests.cpp")

target_compile_definitions(csp_tests PRIVATE USER_MODE)
add_subdirectory(ForTestsHelpers)
add_subdirectory(SerializableStructs)
target_link_libraries(csp_tests PRIVATE cslib ForTestsHelpers SerializableStructs gtest_main gmock_main)
target_include_directories(csp_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(csp_tests PRIVATE "Tests/csp/pch.h")
