add_executable(forge_core_tests
    serialiser/abi_serialiser_test.cpp
    serialiser/project_serialiser_test.cpp
    serialiser/metadata_serialiser_test.cpp
    serialiser/package_serialiser_test.cpp
    serialiser/toml_serialiser_test.cpp
    serialiser/lock_serialiser_test.cpp
)

target_link_libraries(forge_core_tests PRIVATE
    forge_core
    GTest::gtest_main
)

target_compile_features(forge_core_tests PRIVATE cxx_std_23)

include(GoogleTest)
gtest_discover_tests(forge_core_tests)
