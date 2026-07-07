#pragma once
#ifndef PROJECT_SERIALISER_H
#define PROJECT_SERIALISER_H

#include <Forge/schema/project.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {

toml::table serialise(const ProjectInfo &project_info);
ProjectInfo deserialise_project_info(const toml::table &project_info_table);

} // namespace forge::serialiser

#endif // !PROJECT_SERIALISER_H
