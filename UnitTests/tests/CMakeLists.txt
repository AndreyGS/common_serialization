project(common_serialization_tests)

add_subdirectory(for_tests_helpers)
add_subdirectory(serializable_structs)

add_executable(containers_tests "tests/containers/VectorTests.cpp" "tests/containers/WalkerTests.cpp" "tests/containers/UniquePtrTests.cpp")
target_compile_definitions(containers_tests PRIVATE USER_MODE)
target_link_libraries(containers_tests PRIVATE cslib for_tests_helpers PRIVATE gtest_main)
target_include_directories(containers_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(containers_tests PRIVATE "tests/containers/pch.h")

add_executable(allocators_tests "tests/allocators/RawKeeperAllocatorTests.cpp" "tests/allocators/RawNoexceptAllocatorTests.cpp" "tests/allocators/ConstructorNoexceptAllocatorTests.cpp")
target_compile_definitions(allocators_tests PRIVATE USER_MODE)
target_link_libraries(allocators_tests PRIVATE cslib for_tests_helpers PRIVATE gtest_main)
target_include_directories(allocators_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(allocators_tests PRIVATE "tests/allocators/pch.h")

add_executable(csp_tests
    "tests/csp/processing/AlignmentMayBeNotEqualTests.cpp"
    "tests/csp/processing/ISerializableAllowUnmanagedPointersTests.cpp"
    "tests/csp/processing/BasicModeTests.cpp"
    "tests/csp/processing/CheckRecursivePointersTests.cpp"
    "tests/csp/processing/InterfaceVersionsNotMatchTests.cpp"
    "tests/csp/processing/MultiModesTests.cpp"
    "tests/csp/processing/SimplyAssignableTagsOptimizationsAreTurnedOff.cpp"
    "tests/csp/processing/SizeOfPrimitivesMayBeNotEqualTests.cpp"
    "tests/csp/processing/StdIncludedTests.cpp"
    "tests/csp/processing/AnotherEndiannessTests.cpp" 
    "tests/csp/processing/AnotherBitnessTests.cpp"
    "tests/csp/messaging/MessagingTests.cpp"
    "tests/csp/messaging/ClientTests.cpp"
    "tests/csp/messaging/ServerTests.cpp"
    "tests/csp/processing/ProfillingTests.cpp" 
    )

target_compile_definitions(csp_tests PRIVATE USER_MODE)
target_link_libraries(csp_tests PRIVATE cslib for_tests_helpers serializable_structs gtest_main gmock_main)
target_include_directories(csp_tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_precompile_headers(csp_tests PRIVATE "tests/csp/pch.h")
