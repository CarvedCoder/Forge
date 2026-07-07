add_executable(forge
    main.cpp
)

target_link_libraries(forge PRIVATE
    forge_core
)

target_compile_features(forge PRIVATE cxx_std_23)
