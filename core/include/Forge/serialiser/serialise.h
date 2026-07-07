#ifndef FORGE_TOML_H
#define FORGE_TOML_H

#include "Forge/schema/lock.h"
#include <Forge/abi.h>
#include <Forge/lock.h>
#include <Forge/metadata.h>
#include <Forge/package.h>
#include <Forge/project.h>
#include <Forge/toml.h>
#include <toml++/toml.hpp>

toml::table serialise(const Abi &abi);
toml::table serialise(const Metadata &metadata);
toml::table serialise(const Package &package);
toml::table serialise(const PackageStatus &package_status);
toml::table serialise(const ResolvedPackage &resolved_package);
toml::table serialise(const GitSource &git_source);
toml::table serialise(const ProjectInfo &project_info);
toml::table serialise(const ForgeToml &forge_toml);
toml::table serialise(const ForgeLock &forge_lock);

#endif // !FORGE_TOML_H
