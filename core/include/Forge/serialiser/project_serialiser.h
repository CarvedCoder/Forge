#pragma once
#include "Forge/schema/error.h"
#include <expected>
#ifndef PROJECT_SERIALISER_H
#define PROJECT_SERIALISER_H

#include <Forge/schema/project.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const ProjectInfo& project_info);
[[nodiscard]] std::expected<ProjectInfo, Error>
deserialise_project_info(const toml::table& toml_tbl);

} // namespace forge::serialiser

#endif // !PROJECT_SERIALISER_H
