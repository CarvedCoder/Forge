#pragma once
#ifndef PACKAGE_SERIALISER_H
#define PACKAGE_SERIALISER_H

#include <Forge/schema/package.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const Package &package);
toml::table serialise(const ResolvedPackage &resolved_package);
toml::table serialise(const GitSource &git_source);
Package deserialise_package(const toml::table &package_table);
Package deserialise_resolved_package(const toml::table &resolved_package_table);
Package deserialise_git_source(const toml::table &git_source_table);

} // namespace forge::serialiser

#endif // !PACKAGE_SERIALISER_H
