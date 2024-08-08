project(common_serialization_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/9fce5480448488e17a50bcbf88d2f3bdb637ad6c.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(for_tests_helpers)
add_subdirectory(serializable_structs)

add_executable(containers_tests "tests_/containers_/VectorTests.cpp" "tests_/containers_/WalkerTests.cpp" "tests_/containers_/UniquePtrTests.cpp")
target_compile_definitions(containers_tests PRIVATE USER_MODE)
target_link_libraries(containers_tests PRIVATE cslib for_tests_helpers PRIVATE gtest_main)
target_include_directories(containers_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(containers_tests PRIVATE "tests_/containers_/pch.h")

add_executable(allocators_tests "tests_/allocators_/RawKeeperAllocatorTests.cpp" "tests_/allocators_/RawNoexceptAllocatorTests.cpp" "tests_/allocators_/ConstructorNoexceptAllocatorTests.cpp")
target_compile_definitions(allocators_tests PRIVATE USER_MODE)
target_link_libraries(allocators_tests PRIVATE cslib for_tests_helpers PRIVATE gtest_main)
target_include_directories(allocators_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(allocators_tests PRIVATE "tests_/allocators_/pch.h")

add_executable(csp_tests
    "tests_/csp/processing/ISerializableAlignmentMayBeNotEqualTests.cpp"
    "tests_/csp/processing/ISerializableAllowUnmanagedPointersTests.cpp"
    "tests_/csp/processing/ISerializableBasicModeTests.cpp"
    "tests_/csp/processing/ISerializableCheckRecursivePointersTests.cpp"
    "tests_/csp/processing/ISerializableInterfaceVersionsNotMatchTests.cpp"
    "tests_/csp/processing/ISerializableMultiModesTests.cpp"
    "tests_/csp/processing/ISerializableSimplyAssignableTagsOptimizationsAreTurnedOff.cpp"
    "tests_/csp/processing/ISerializableSizeOfPrimitivesMayBeNotEqualTests.cpp"
    "tests_/csp/processing/StdIncludedTests.cpp"
    "tests_/csp/processing/ISerializableAnotherEndianness.cpp" 
    "tests_/csp/processing/ISerializableAnotherBitness.cpp"
    "tests_/csp/messaging/MessagingTests.cpp"
    "tests_/csp/messaging/ClientTests.cpp"
    "tests_/csp/messaging/ServerTests.cpp"
    "tests_/csp/processing/ProfillingTests.cpp" 
    )

target_compile_definitions(csp_tests PRIVATE USER_MODE)
target_link_libraries(csp_tests PRIVATE cslib for_tests_helpers serializable_structs gtest_main gmock_main)
target_include_directories(csp_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(csp_tests PRIVATE "tests_/csp/pch.h")
