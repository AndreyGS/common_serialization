cmake_minimum_required(VERSION 3.16)

project(common_serialization)

add_subdirectory(UnitTests)

add_library(common_options INTERFACE)
target_include_directories(common_options INTERFACE "${PROJECT_SOURCE_DIR}/Include" "${PROJECT_SOURCE_DIR}")
target_compile_features(common_options INTERFACE cxx_std_20)
target_compile_definitions(common_options INTERFACE USER_MODE _HAS_CXX20)

#add_library(common_serialization "Serialization/Src/SerializationFlags.cpp" )

#target_link_libraries(common_serialization PUBLIC common_options)
target_precompile_headers(common_options INTERFACE pch.h)
