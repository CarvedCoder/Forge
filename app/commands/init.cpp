#include "init.h"

#include <Forge/serialiser/manifest_serialiser.h>

#include <filesystem>
#include <fstream>
#include <print>

namespace forge::commands {

void register_init(CLI::App& app) {
    auto* init = app.add_subcommand("init", "Create a new project");

    static std::string project_name = std::filesystem::current_path().filename().string();
    init->add_option("-n,--name", project_name, "Project name")->default_val(project_name);

    init->callback([]() {
        if (std::filesystem::exists("forge.toml")) {
            std::println(stderr, "forge.toml already exists in this directory");
            std::exit(1);
        }

        ForgeToml manifest;
        manifest.project = {.project_name = project_name,
                            .project_standard = "c++23",
                            .project_build_type = "Debug"};
        manifest.abi = {
            .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};
        manifest.metadata = {.forge_version = "0.1.0", .cmake_version = "3.28"};

        toml::table serialised = serialiser::serialise(manifest);
        std::filesystem::create_directory("src");
        std::ofstream main("src/main.cpp");
        std::filesystem::create_directory(".forge");
        std::ofstream toml(".forge/forge.toml");
        toml << serialised;

        std::println("Created forge.toml for '{}'", project_name);
    });
}

} // namespace forge::commands
