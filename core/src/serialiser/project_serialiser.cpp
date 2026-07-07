#include <Forge/schema/project.h>
#include <Forge/serialiser/project_serialiser.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const ProjectInfo &project_info) {
  return toml::table{{"project-name", project_info.project_name},
                     {"project-standard", project_info.project_standard},
                     {"project-build-type", project_info.project_build_type}};
}

} // namespace forge::serialiser
