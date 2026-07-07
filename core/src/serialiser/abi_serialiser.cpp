#include <Forge/schema/abi.h>
#include <Forge/serialiser/abi_serialiser.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const Abi &abi) {
  return toml::table{{"compiler", abi.compiler},
                     {"stdlib", abi.stdlib},
                     {"cxx-standard", abi.cxx_standard},
                     {"c-standard", abi.c_standard}};
}
} // namespace forge::serialiser
