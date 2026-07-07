add_library(forge_core STATIC
    src/misc/timestamp.cpp
    src/serialiser/project_serialiser.cpp
    src/serialiser/abi_serialiser.cpp
    src/serialiser/manifest_serialiser.cpp
    src/serialiser/metadata_serialiser.cpp
    src/serialiser/package_serialiser.cpp

)

target_include_directories(forge_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(forge_core PUBLIC
    tomlplusplus::tomlplusplus
)

target_compile_features(forge_core PUBLIC cxx_std_23)
