#include <Forge/misc/timestamp.h>
#include <Forge/serialiser/get_field.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <expected>
#include <toml++/toml.hpp>
namespace forge::serialiser {
toml::table serialise(const Metadata& metadata) {
    return toml::table{{"forge-version", metadata.forge_version},
                       {"generated-at", timestamp::current_timestamp()},
                       {"cmake-version", metadata.cmake_version}};
}

[[nodiscard]] std::expected<Metadata, Error> deserialise_metadata(const toml::table& toml_tbl) {
    auto forge_version = get_field<std::string>(toml_tbl, "forge-version");
    if (!forge_version)
        return std::unexpected(forge_version.error());
    auto cmake_version = get_field<std::string>(toml_tbl, "cmake-version");
    if (!cmake_version)
        return std::unexpected(cmake_version.error());

    auto generated_at = get_field<std::string>(toml_tbl, "generated-at");
    if (!generated_at)
        return std::unexpected(generated_at.error());

    return Metadata{.forge_version = *forge_version,
                    .generated_at = *generated_at,
                    .cmake_version = *cmake_version};
}
} // namespace forge::serialiser
