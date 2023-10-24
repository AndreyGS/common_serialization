add_library(types4tests STATIC "SpecialTypesFilling.cpp" "pch.h")

target_compile_definitions(types4tests PRIVATE USER_MODE)
target_link_libraries(types4tests PRIVATE cslib)
target_precompile_headers(types4tests PRIVATE pch.h)
