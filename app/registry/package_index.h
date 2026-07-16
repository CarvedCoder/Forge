#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace forge {

/* A small, curated set of known packages.
It ships inside the Forge binary itself, so there's no network call and
no external service Forge depends on. It only maps a friendly name to
where its git repo lives; Forge still resolves tags/commits directly
from that repo, nothing here is cached or versioned externally.

Adding a package here just means adding one line — no registry account,
no publishing step. */

inline const std::unordered_map<std::string, std::pair<std::string, std::string>>&
known_packages() {
    static const std::unordered_map<std::string, std::pair<std::string, std::string>> index{
        {"fmt", {"https://github.com/fmtlib/fmt.git", "fmt::fmt"}},
        {"spdlog", {"https://github.com/gabime/spdlog.git", ""}},
        {"nlohmann_json", {"https://github.com/nlohmann/json.git", ""}},
        {"catch2", {"https://github.com/catchorg/Catch2.git", ""}},
        {"googletest", {"https://github.com/google/googletest.git", ""}},
        {"tomlplusplus", {"https://github.com/marzer/tomlplusplus.git", ""}},
        {"cli11", {"https://github.com/CLIUtils/CLI11.git", ""}},
        {"doctest", {"https://github.com/doctest/doctest.git", ""}},
        {"range-v3", {"https://github.com/ericniebler/range-v3.git", ""}},
        {"eigen", {"https://gitlab.com/libeigen/eigen.git", ""}},
    };
    return index;
}

inline std::optional<std::string> lookup_package_url(const std::string& name) {
    const auto& index = known_packages();
    auto it = index.find(name);
    if (it == index.end())
        return std::nullopt;
    return it->second.first;
}

inline std::optional<std::string> cmake_name(const std::string& name) {}

} // namespace forge
