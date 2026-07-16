add_executable(forge
    app.cpp
    commands/add.cpp
    commands/init.cpp
    cmake/cmake_deps.cpp
)

target_link_libraries(forge PRIVATE
    forge_core
    CLI11::CLI11

)

target_compile_features(forge PRIVATE cxx_std_23)

install(TARGETS forge
    RUNTIME DESTINATION bin
)
