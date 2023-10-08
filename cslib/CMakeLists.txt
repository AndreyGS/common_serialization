project(cslib
    DESCRIPTION "Header-only serialization C++ library."
    HOMEPAGE_URL "https://github.com/AndreyGS/common_serialization"
)

include(GNUInstallDirs)

add_library(cslib INTERFACE)

target_include_directories(cslib INTERFACE 
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(cslib INTERFACE cxx_std_20)
target_compile_definitions(cslib INTERFACE _HAS_CXX20)
