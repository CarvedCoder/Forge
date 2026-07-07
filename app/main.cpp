#include <Forge/schema/manifest.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <filesystem>
#include <fstream>
#include <toml++/toml.hpp>
namespace fs = std::filesystem;
int main() {
  forge::ForgeToml forge_toml;
  forge_toml.project.project_name = "dummy-project";
  forge_toml.project.project_standard = "c++23";
  forge_toml.project.project_build_type = "Debug";

  forge_toml.abi.compiler = "gcc";
  forge_toml.abi.stdlib = "libstdc++";
  forge_toml.abi.cxx_standard = "23";
  forge_toml.abi.c_standard = "17";

  forge_toml.metadata.forge_version = "0.1.0";
  forge_toml.metadata.cmake_version = "3.28";

  forge::Package pkg;
  pkg.package_name = "fmt";
  pkg.version = "10.2.1";
  pkg.source.url = "https://github.com/fmtlib/fmt.git";
  pkg.source.sha256 = "xyz1234";
  pkg.source.commit_hash = "abc1234";
  pkg.status = forge::PackageStatus::Unresolved;
  forge_toml.packages.push_back(pkg);

  toml::table serialised = forge::serialiser::serialise(forge_toml);
  fs::path parent_path{fs::current_path().parent_path()};

  fs::create_directory(".forge/");

  std::ofstream out("../.forge/forge.toml");
  out << serialised << "\n";
}
