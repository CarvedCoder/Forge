#include <Forge/misc/timestamp.h>
#include <Forge/schema/metadata.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const Metadata &metadata) {
  return toml::table{{"forge-version", metadata.forge_version},
                     {"generated-at", timestamp::current_timestamp()},
                     {"cmake-version", metadata.cmake_version}};
}
} // namespace forge::serialiser
