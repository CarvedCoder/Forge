#pragma once
#ifndef MANIFEST_SERIALISER_H
#define MANIFEST_SERIALISER_H

#include <Forge/schema/manifest.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const ForgeToml &forge_toml);
toml::table serialise(const ForgeLock &forge_lock);
ForgeToml deserialise_forge_toml(const toml::table &forge_toml_table);
ForgeLock deserialise_forge_lock(const toml::table &forge_lock_table);

} // namespace forge::serialiser

#endif // !MANIFEST_SERIALISER_H
