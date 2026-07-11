#include <Forge/serialiser/get_field.h>
#include <Forge/serialiser/project_serialiser.h>

namespace forge::serialiser {
toml::table serialise(const ProjectInfo& project_info) {
    return toml::table{{"project-name", project_info.project_name},
                       {"project-standard", project_info.project_standard},
                       {"project-build-type", project_info.project_build_type}};
}

[[nodiscard]] std::expected<ProjectInfo, Error>
deserialise_project_info(const toml::table& toml_tbl) {
    auto project_name = get_field<std::string>(toml_tbl, "project-name");
    if (!project_name)
        return std::unexpected(project_name.error());

    auto project_standard = get_field<std::string>(toml_tbl, "project-standard");
    if (!project_standard)
        return std::unexpected(project_standard.error());

    auto project_build_type = get_field<std::string>(toml_tbl, "project-build-type");
    if (!project_build_type)
        return std::unexpected(project_build_type.error());

    return ProjectInfo{.project_name = *project_name,
                       .project_standard = *project_standard,
                       .project_build_type = *project_build_type};
}

} // namespace forge::serialiser
