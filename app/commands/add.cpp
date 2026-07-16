#include "add.h"
#include "../cmake/cmake_deps.h"

#include "../registry/package_index.h"
#include <Forge/serialiser/manifest_serialiser.h>

#include <array>
#include <cstdio>
#include <fstream>
#include <memory>
#include <optional>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace forge::commands {
namespace {

struct GitRef {
    std::string tag;
    std::string commit_hash;
};

std::string run_command(const std::string& cmd) {
    std::array<char, 4096> buffer{};
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        std::println(stderr, "failed to run: {}", cmd);
        std::exit(1);
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::optional<std::array<int, 3>> parse_stable_semver(const std::string& tag) {
    static const std::regex pattern(R"(^v?(\d+)\.(\d+)\.(\d+)$)");
    std::smatch match;
    if (!std::regex_match(tag, match, pattern))
        return std::nullopt;
    return std::array<int, 3>{std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3])};
}

std::vector<GitRef> fetch_tags(const std::string& url) {
    std::string output = run_command("git ls-remote --tags " + url + " 2>&1");

    std::vector<GitRef> refs;
    std::istringstream stream(output);
    std::string line;
    static const std::regex line_pattern(R"(^([0-9a-f]{40})\s+refs/tags/([^\s\^]+)$)");

    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_match(line, match, line_pattern)) {
            refs.push_back(GitRef{.tag = match[2], .commit_hash = match[1]});
        }
    }
    return refs;
}

std::optional<GitRef> find_latest_stable(const std::vector<GitRef>& refs) {
    const GitRef* best = nullptr;
    std::array<int, 3> best_version{};

    for (const auto& ref : refs) {
        auto version = parse_stable_semver(ref.tag);
        if (!version)
            continue;
        if (!best || *version > best_version) {
            best = &ref;
            best_version = *version;
        }
    }
    if (!best)
        return std::nullopt;
    return *best;
}

std::optional<GitRef> find_exact_version(const std::vector<GitRef>& refs,
                                         const std::string& version) {
    for (const auto& ref : refs) {
        if (ref.tag == version || ref.tag == ("v" + version))
            return ref;
    }
    return std::nullopt;
}

} // namespace

void register_add(CLI::App& app) {
    auto* add = app.add_subcommand("add", "Add a known package to the project");

    static std::string name;
    static std::string version; // empty => resolve latest stable

    add->add_option("name", name, "Package name, e.g. 'fmt'")->required();
    add->add_option("--version", version, "Specific version to pin (default: latest stable)");

    add->callback([]() {
        auto url = lookup_package_url(name);
        if (!url) {
            std::println(stderr,
                         "'{}' isn't in Forge's known package list yet. "
                         "Open an issue/PR to have it added.",
                         name);
            std::exit(1);
        }

        std::println("Resolving tags for {}...", name);
        std::vector<GitRef> tags = fetch_tags(*url);
        if (tags.empty()) {
            std::println(stderr, "no tags found for {} ({})", name, *url);
            std::exit(1);
        }

        std::optional<GitRef> resolved =
            version.empty() ? find_latest_stable(tags) : find_exact_version(tags, version);

        if (!resolved) {
            if (version.empty()) {
                std::println(stderr, "no stable tags found for {}", name);
            } else {
                std::println(stderr, "version '{}' not found for {}", version, name);
            }
            std::exit(1);
        }

        toml::table existing;
        try {
            existing = toml::parse_file("forge.toml");
        } catch (const toml::parse_error& err) {
            std::println(stderr, "no valid forge.toml found: {}", err.description());
            std::exit(1);
        }

        auto manifest = serialiser::deserialise_forge_toml(existing);
        if (!manifest) {
            std::println(stderr, "forge.toml is invalid: {}", manifest.error());
            std::exit(1);
        }

        Package pkg;
        pkg.package_name = name;
        pkg.version = resolved->tag;
        pkg.source = {.url = *url,
                      .sha256 = "", // TODO: filled in once the resolver fetches content
                      .commit_hash = resolved->commit_hash};
        pkg.status = PackageStatus::Unresolved;
        manifest->packages.push_back(pkg);

        std::ofstream out("forge.toml");
        out << serialiser::serialise(*manifest);

        write_cmake_deps(*manifest); // regenerate .forge/deps.cmake from scratch

        std::println("Added '{}' @ {} ({}) to forge.toml", name, resolved->tag,
                     resolved->commit_hash.substr(0, 8));
    });
}

} // namespace forge::commands
