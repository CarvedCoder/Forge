add_library(forge_core STATIC
    src/serialiser/serialise.cpp
)

target_include_directories(forge_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(forge_core PUBLIC
    tomlplusplus::tomlplusplus
)

target_compile_features(forge_core PUBLIC cxx_std_23)
