#pragma once
#include "Forge/schema/error.h"
#include <expected>
#ifndef PACKAGE_SERIALISER_H
#define PACKAGE_SERIALISER_H

#include <Forge/schema/package.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const Package& package);
toml::table serialise(const ResolvedPackage& resolved_package);
toml::table serialise(const GitSource& git_source);
[[nodiscard]] std::expected<Package, Error> deserialise_package(const toml::table& toml_tbl);
[[nodiscard]] std::expected<ResolvedPackage, Error>
deserialise_resolved_package(const toml::table& toml_tbl);
[[nodiscard]] std::expected<GitSource, Error> deserialise_git_source(const toml::table& toml_tbl);

constexpr std::string_view to_string(PackageStatus status);

constexpr std::expected<PackageStatus, Error> package_status_from_string(std::string_view s,
                                                                         toml::source_region loc);

} // namespace forge::serialiser

#endif // !PACKAGE_SERIALISER_H
