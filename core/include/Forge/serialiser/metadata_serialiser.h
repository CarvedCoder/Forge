#ifndef METADATA_SERIALISER_H
#define METADATA_SERIALISER_H

#include <Forge/schema/error.h>
#include <Forge/schema/metadata.h>
#include <expected>
#include <toml++/toml.hpp>
namespace forge::serialiser {

toml::table serialise(const Metadata& metadata);
[[nodiscard]] std::expected<Metadata, Error> deserialise_metadata(const toml::table& toml_tbl);

} // namespace forge::serialiser

#endif // !METADATA_SERIALISER_H
