#pragma once
#ifndef ABI_SERIALISER_H
#define ABI_SERIALISER_H

#include <Forge/schema/abi.h>
#include <Forge/schema/error.h>
#include <expected>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const Abi& abi);
[[nodiscard]] std::expected<Abi, Error> deserialise_abi(const toml::table& toml_tbl);

} // namespace forge::serialiser

#endif // !ABI_SERIALISER_H
