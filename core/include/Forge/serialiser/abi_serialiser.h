#pragma once
#ifndef ABI_SERIALISER_H
#define ABI_SERIALISER_H

#include <Forge/schema/abi.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const Abi &abi);
Abi deserialise_abi(const toml::table &abi_toml);

} // namespace forge::serialiser

#endif // !ABI_SERIALISER_H
