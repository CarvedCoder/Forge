#include <Forge/serialiser/abi_serialiser.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

namespace {

// Writes a toml::table to disk, then reparses it from disk (not from memory)
// so we're testing the real round trip a user would hit: write forge.toml,
// re-open Forge later, read it back.
toml::table write_and_reparse(const toml::table& tbl, const fs::path& path) {
    fs::create_directories(path.parent_path());

    std::ofstream out(path);
    if (!out.is_open()) {
        std::println(std::cerr, "failed to open {} for writing", path.string());
        std::exit(1);
    }
    out << tbl;
    out.close();

    try {
        return toml::parse_file(path.string());
    } catch (const toml::parse_error& err) {
        std::println(std::cerr, "reparse of {} failed: {}", path.string(), err.description());
        std::exit(1);
    }
}

forge::ForgeToml make_sample_manifest() {
    forge::ForgeToml manifest;
    manifest.project = {.project_name = "dummy-project",
                        .project_standard = "c++23",
                        .project_build_type = "Debug"};
    manifest.abi = {
        .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};
    manifest.metadata = {.forge_version = "0.1.0", .cmake_version = "3.28"};

    forge::Package pkg;
    pkg.package_name = "fmt";
    pkg.version = "10.2.1";
    pkg.source = {
        .url = "https://github.com/fmtlib/fmt.git", .sha256 = "xyz1234", .commit_hash = "abc1234"};
    pkg.status = forge::PackageStatus::Unresolved;
    manifest.packages.push_back(pkg);

    return manifest;
}

forge::ForgeLock make_sample_lock() {
    forge::ForgeLock lock;
    lock.metadata = {.forge_version = "0.1.0", .cmake_version = "3.28"};
    lock.abi = {.compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};

    forge::ResolvedPackage resolved;
    resolved.name = "fmt";
    resolved.source = {
        .url = "https://github.com/fmtlib/fmt.git", .sha256 = "xyz1234", .commit_hash = "abc1234"};
    lock.resolved_packages.push_back(resolved);

    return lock;
}

bool test_manifest_round_trip() {
    std::println("--- ForgeToml round trip ---");

    forge::ForgeToml original = make_sample_manifest();
    toml::table serialised = forge::serialiser::serialise(original);
    toml::table reparsed = write_and_reparse(serialised, "../.forge/forge.toml");

    auto result = forge::serialiser::deserialise_forge_toml(reparsed);
    if (!result) {
        std::println(std::cerr, "deserialise_manifest failed: {}", result.error());
        return false;
    }

    const forge::ForgeToml& r = *result;
    bool ok = true;

    ok &= (r.project.project_name == original.project.project_name);
    ok &= (r.project.project_standard == original.project.project_standard);
    ok &= (r.abi.compiler == original.abi.compiler);
    ok &= (r.abi.cxx_standard == original.abi.cxx_standard);
    ok &= (r.metadata.forge_version == original.metadata.forge_version);
    ok &= (r.packages.size() == original.packages.size());

    if (!r.packages.empty()) {
        ok &= (r.packages[0].package_name == original.packages[0].package_name);
        ok &= (r.packages[0].source.sha256 == original.packages[0].source.sha256);
        ok &= (r.packages[0].status == original.packages[0].status);
    }

    std::println("project_name: {}", r.project.project_name);
    std::println("compiler: {}", r.abi.compiler);
    std::println("packages: {}", r.packages.size());
    if (!r.packages.empty()) {
        std::println("  [0].package_name: {}", r.packages[0].package_name);
        std::println("  [0].source.sha256: {}", r.packages[0].source.sha256);
    }
    std::println("{}", ok ? "PASS" : "FAIL — field mismatch after round trip");
    return ok;
}

bool test_lock_round_trip() {
    std::println("\n--- ForgeLock round trip ---");

    forge::ForgeLock original = make_sample_lock();
    toml::table serialised = forge::serialiser::serialise(original);
    toml::table reparsed = write_and_reparse(serialised, "../.forge/forge.lock");

    auto result = forge::serialiser::deserialise_forge_lock(reparsed);
    if (!result) {
        std::println(std::cerr, "deserialise_lock failed: {}", result.error());
        return false;
    }

    const forge::ForgeLock& r = *result;
    bool ok = true;

    ok &= (r.metadata.forge_version == original.metadata.forge_version);
    ok &= (r.abi.compiler == original.abi.compiler);
    ok &= (r.resolved_packages.size() == original.resolved_packages.size());

    if (!r.resolved_packages.empty()) {
        ok &= (r.resolved_packages[0].name == original.resolved_packages[0].name);
        ok &= (r.resolved_packages[0].source.commit_hash ==
               original.resolved_packages[0].source.commit_hash);
    }

    std::println("resolved_packages: {}", r.resolved_packages.size());
    if (!r.resolved_packages.empty()) {
        std::println("  [0].name: {}", r.resolved_packages[0].name);
        std::println("  [0].source.commit_hash: {}", r.resolved_packages[0].source.commit_hash);
    }
    std::println("{}", ok ? "PASS" : "FAIL — field mismatch after round trip");
    return ok;
}

// Deliberately break the manifest (drop a required field) and confirm
// deserialise actually reports the error instead of silently defaulting.
bool test_missing_field_is_caught() {
    std::println("\n--- Missing field detection ---");

    toml::table broken{
        {"project-info",
         toml::table{
             {"project-name", "dummy-project"}, {"project-standard", "c++23"}
             // project-build-type deliberately omitted
         }},
        {"abi",
         forge::serialiser::serialise(forge::Abi{
             .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"})},
        {"metadata", forge::serialiser::serialise(
                         forge::Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        {"packages", toml::array{}},
    };

    auto result = forge::serialiser::deserialise_forge_toml(broken);
    if (result) {
        std::println(std::cerr, "FAIL — expected MissingField error but deserialise succeeded");
        return false;
    }

    std::println("Got expected error: {}", result.error());
    bool ok = (result.error().field_name == "project-build-type" &&
               result.error().kind == forge::serialiser::ErrorKind::MissingField);
    std::println("{}", ok ? "PASS" : "FAIL — wrong error field/kind reported");
    return ok;
}

} // namespace

int main() {
    bool all_ok = true;
    all_ok &= test_manifest_round_trip();
    all_ok &= test_lock_round_trip();
    all_ok &= test_missing_field_is_caught();

    std::println("\n=== {} ===", all_ok ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return all_ok ? 0 : 1;
}
