#include <Forge/serialiser/get_field.h>
#include <Forge/serialiser/package_serialiser.h>
#include <expected>

namespace forge::serialiser {

toml::table serialise(const GitSource& git_source) {
    return toml::table{{"url", git_source.url},
                       {"sha256", git_source.sha256},
                       {"commit-hash", git_source.commit_hash}};
}

toml::table serialise(const Package& package) {
    return toml::table{{"package-name", package.package_name},
                       {"version", package.version},
                       {"git-source", serialise(package.source)},
                       {"package-status", to_string(package.status)}};
}

toml::table serialise(const ResolvedPackage& resolved_package) {
    return toml::table{{"name", resolved_package.name},
                       {"source", serialise(resolved_package.source)}};
}

constexpr std::string_view to_string(PackageStatus status) {
    switch (status) {
    case PackageStatus::Resolved:
        return "resolved";
    case PackageStatus::Unresolved:
        return "unresolved";
    case PackageStatus::None:
        return "unresolved";
    }
    return "unresolved";
}

constexpr std::expected<PackageStatus, Error> package_status_from_string(std::string_view s,
                                                                         toml::source_region loc) {
    if (s == "resolved")
        return PackageStatus::Resolved;
    if (s == "unresolved")
        return PackageStatus::Unresolved;
    return std::unexpected(Error{ErrorKind::TypeMismatch, "package-status", loc});
}

[[nodiscard]] std::expected<GitSource, Error> deserialise_git_source(const toml::table& toml_tbl) {
    auto url = get_field<std::string>(toml_tbl, "url");
    if (!url)
        return std::unexpected(url.error());
    auto sha256 = get_field<std::string>(toml_tbl, "sha256");
    if (!sha256)
        return std::unexpected(sha256.error());
    auto commit_hash = get_field<std::string>(toml_tbl, "commit-hash");
    if (!commit_hash)
        return std::unexpected(commit_hash.error());

    return GitSource{.url = *url, .sha256 = *sha256, .commit_hash = *commit_hash};
}

[[nodiscard]] std::expected<ResolvedPackage, Error>
deserialise_resolved_package(const toml::table& toml_tbl) {
    auto name = get_field<std::string>(toml_tbl, "name");
    if (!name)
        return std::unexpected(name.error());

    auto source_tbl = toml_tbl["source"].as_table();
    if (!source_tbl)
        return std::unexpected(Error{ErrorKind::MissingField, "source", toml_tbl.source()});
    auto source = deserialise_git_source(*source_tbl);

    return ResolvedPackage{.name = *name, .source = *source};
}

[[nodiscard]] std::expected<Package, Error> deserialise_package(const toml::table& toml_tbl) {
    auto package_name = get_field<std::string>(toml_tbl, "package-name");
    if (!package_name)
        return std::unexpected(package_name.error());
    auto version = get_field<std::string>(toml_tbl, "version");
    if (!version)
        return std::unexpected(version.error());
    auto source_tbl = toml_tbl["git-source"].as_table();
    if (!source_tbl)
        return std::unexpected(Error{ErrorKind::MissingField, "git-source", toml_tbl.source()});
    auto git_source = deserialise_git_source(*source_tbl);
    if (!git_source)
        return std::unexpected(git_source.error());
    auto status_str = get_field<std::string>(toml_tbl, "package-status");
    if (!status_str)
        return std::unexpected(status_str.error());
    auto package_status =
        package_status_from_string(*status_str, toml_tbl["package-status"].node()->source());
    if (!package_status)
        return std::unexpected(package_status.error());
    return Package{.package_name = *package_name,
                   .version = *version,
                   .source = *git_source,
                   .status = *package_status};
}

} // namespace forge::serialiser
