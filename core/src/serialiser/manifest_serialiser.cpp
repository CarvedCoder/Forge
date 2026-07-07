#include <Forge/schema/manifest.h>
#include <Forge/schema/package.h>
#include <Forge/serialiser/abi_serialiser.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <Forge/serialiser/package_serialiser.h>
#include <Forge/serialiser/project_serialiser.h>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const ForgeLock &forge_lock) {
  toml::array resolved_packages;
  for (const ResolvedPackage &resolved_package : forge_lock.resolved_packages) {
    resolved_packages.emplace_back(serialise(resolved_package));
  }

  return toml::table{{"metadata", serialise(forge_lock.metadata)},
                     {"abi", serialise(forge_lock.abi)},
                     {"resolved-packages", resolved_packages}

  };
}

toml::table serialise(const ForgeToml &forge_toml) {
  toml::array packages;
  for (const Package &package : forge_toml.packages) {
    packages.emplace_back(serialise(package));
  }

  return toml::table{{"project-info", serialise(forge_toml.project)},
                     {"abi", serialise(forge_toml.abi)},
                     {"metadata", serialise(forge_toml.metadata)},
                     {"packages", packages}};
}

} // namespace forge::serialiser
