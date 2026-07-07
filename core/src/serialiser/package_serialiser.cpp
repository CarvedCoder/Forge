#include <Forge/schema/package.h>
#include <Forge/serialiser/package_serialiser.h>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const GitSource &git_source) {
  return toml::table{{"url", git_source.url},
                     {"sha256", git_source.sha256},
                     {"commit-hash", git_source.commit_hash}};
}

toml::table serialise(const Package &package) {
  return toml::table{{"package-name", package.package_name},
                     {"version", package.version},
                     {"git-source", serialise(package.source)},
                     {"package-status", package.status}};
}

toml::table serialise(const ResolvedPackage &resolved_package) {
  return toml::table{{"name", resolved_package.name},
                     {"source", serialise(resolved_package.source)}};
}

} // namespace forge::serialiser
