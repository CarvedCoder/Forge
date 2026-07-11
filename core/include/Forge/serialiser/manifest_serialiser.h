#pragma once
#include "Forge/schema/error.h"
#include <expected>
#ifndef MANIFEST_SERIALISER_H
#define MANIFEST_SERIALISER_H

#include <Forge/schema/manifest.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const ForgeToml& forge_toml);
toml::table serialise(const ForgeLock& forge_lock);
[[nodiscard]] std::expected<ForgeToml, Error> deserialise_forge_toml(const toml::table& toml_tbl);
[[nodiscard]] std::expected<ForgeLock, Error> deserialise_forge_lock(const toml::table& toml_tbl);

} // namespace forge::serialiser

#endif // !MANIFEST_SERIALISER_H
