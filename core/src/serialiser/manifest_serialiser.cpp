#include <Forge/schema/manifest.h>
#include <Forge/schema/package.h>
#include <Forge/serialiser/abi_serialiser.h>
#include <Forge/serialiser/deserialise_array.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <Forge/serialiser/package_serialiser.h>
#include <Forge/serialiser/project_serialiser.h>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const ForgeLock& forge_lock) {
    toml::array resolved_packages;
    for (const ResolvedPackage& resolved_package : forge_lock.resolved_packages) {
        resolved_packages.emplace_back(serialise(resolved_package));
    }

    return toml::table{{"metadata", serialise(forge_lock.metadata)},
                       {"abi", serialise(forge_lock.abi)},
                       {"resolved-packages", resolved_packages}

    };
}

toml::table serialise(const ForgeToml& forge_toml) {
    toml::array packages;
    for (const Package& package : forge_toml.packages) {
        packages.emplace_back(serialise(package));
    }

    return toml::table{{"project-info", serialise(forge_toml.project)},
                       {"abi", serialise(forge_toml.abi)},
                       {"metadata", serialise(forge_toml.metadata)},
                       {"packages", packages}};
}

[[nodiscard]] std::expected<ForgeToml, Error> deserialise_forge_toml(const toml::table& tbl) {
    auto* project_tbl = tbl["project-info"].as_table();
    if (!project_tbl) {
        return std::unexpected(Error{ErrorKind::MissingField, "project-info", tbl.source()});
    }
    auto project = deserialise_project_info(*project_tbl);
    if (!project)
        return std::unexpected(project.error());

    auto* abi_tbl = tbl["abi"].as_table();
    if (!abi_tbl) {
        return std::unexpected(Error{ErrorKind::MissingField, "abi", tbl.source()});
    }
    auto abi = deserialise_abi(*abi_tbl);
    if (!abi)
        return std::unexpected(abi.error());

    auto* metadata_tbl = tbl["metadata"].as_table();
    if (!metadata_tbl) {
        return std::unexpected(Error{ErrorKind::MissingField, "metadata", tbl.source()});
    }
    auto metadata = deserialise_metadata(*metadata_tbl);
    if (!metadata)
        return std::unexpected(metadata.error());

    auto* packages_arr = tbl["packages"].as_array();
    if (!packages_arr) {
        return std::unexpected(Error{ErrorKind::MissingField, "packages", tbl.source()});
    }
    auto packages = deserialise_array<Package>(*packages_arr, &deserialise_package);
    if (!packages)
        return std::unexpected(packages.error());

    return ForgeToml{
        .project = *project, .packages = *std::move(packages), .abi = *abi, .metadata = *metadata};
}

[[nodiscard]] std::expected<ForgeLock, Error> deserialise_forge_lock(const toml::table& tbl) {
    auto* metadata_tbl = tbl["metadata"].as_table();
    if (!metadata_tbl) {
        return std::unexpected(Error{ErrorKind::MissingField, "metadata", tbl.source()});
    }
    auto metadata = deserialise_metadata(*metadata_tbl);
    if (!metadata)
        return std::unexpected(metadata.error());

    auto* abi_tbl = tbl["abi"].as_table();
    if (!abi_tbl) {
        return std::unexpected(Error{ErrorKind::MissingField, "abi", tbl.source()});
    }
    auto abi = deserialise_abi(*abi_tbl);
    if (!abi)
        return std::unexpected(abi.error());

    auto* resolved_arr = tbl["resolved-packages"].as_array();
    if (!resolved_arr) {
        return std::unexpected(Error{ErrorKind::MissingField, "resolved-packages", tbl.source()});
    }
    auto resolved_packages =
        deserialise_array<ResolvedPackage>(*resolved_arr, &deserialise_resolved_package);
    if (!resolved_packages)
        return std::unexpected(resolved_packages.error());

    return ForgeLock{
        .metadata = *metadata, .abi = *abi, .resolved_packages = *std::move(resolved_packages)};
}
} // namespace forge::serialiser
