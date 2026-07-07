#ifndef METADATA_SERIALISER_H
#define METADATA_SERIALISER_H

#include <Forge/schema/metadata.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const Metadata &metadata);
Metadata deserialise_metadata(const toml::table &metadata_table);

} // namespace forge::serialiser

#endif // !METADATA_SERIALISER_H
