#include <Forge/toml.h>

#include <filesystem>
namespace fs = std::filesystem;

void generate_toml(fs::path &path) { fs::create_directory(path); }

int main() {
  fs::path path = "../.forge";
  generate_toml(path);
}
