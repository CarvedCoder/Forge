#ifndef TOML_H
#define TOML_H

#include <Forge/abi.h>
#include <Forge/metadata.h>
#include <Forge/package.h>
#include <Forge/project.h>
#include <vector>

struct ForgeToml {
  ProjectInfo project{};
  std::vector<Package> packages{};
  Abi abi{};
  Metadata metadata{};
};

#endif // !TOML_H
