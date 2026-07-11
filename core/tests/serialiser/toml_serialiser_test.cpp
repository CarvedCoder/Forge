#include <Forge/serialiser/abi_serialiser.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <Forge/serialiser/package_serialiser.h>
#include <Forge/serialiser/project_serialiser.h>
#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

ForgeToml make_sample_manifest() {
    ForgeToml manifest;
    manifest.project = {.project_name = "dummy-project",
                        .project_standard = "c++23",
                        .project_build_type = "Debug"};
    manifest.abi = {
        .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};
    manifest.metadata = {.forge_version = "0.1.0", .cmake_version = "3.28"};

    Package pkg;
    pkg.package_name = "fmt";
    pkg.version = "10.2.1";
    pkg.source = {
        .url = "https://github.com/fmtlib/fmt.git", .sha256 = "xyz1234", .commit_hash = "abc1234"};
    pkg.status = PackageStatus::Unresolved;
    manifest.packages.push_back(pkg);

    return manifest;
}

TEST(ManifestSerialiser, RoundTripsSuccessfully) {
    ForgeToml original = make_sample_manifest();
    toml::table serialised = serialise(original);

    auto result = deserialise_forge_toml(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    const ForgeToml& r = *result;
    EXPECT_EQ(r.project.project_name, original.project.project_name);
    EXPECT_EQ(r.abi.compiler, original.abi.compiler);
    EXPECT_EQ(r.metadata.forge_version, original.metadata.forge_version);
    ASSERT_EQ(r.packages.size(), original.packages.size());
    EXPECT_EQ(r.packages[0].package_name, original.packages[0].package_name);
    EXPECT_EQ(r.packages[0].source.sha256, original.packages[0].source.sha256);
}

TEST(ManifestSerialiser, RoundTripsWithEmptyPackagesArray) {
    ForgeToml manifest = make_sample_manifest();
    manifest.packages.clear();

    toml::table serialised = serialise(manifest);
    auto result = deserialise_forge_toml(serialised);

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->packages.empty());
}

TEST(ManifestSerialiser, RoundTripsWithMultiplePackages) {
    ForgeToml manifest = make_sample_manifest();

    Package second;
    second.package_name = "spdlog";
    second.version = "1.14.1";
    second.source = {.url = "https://github.com/gabime/spdlog.git",
                     .sha256 = "def5678",
                     .commit_hash = "ghi5678"};
    second.status = PackageStatus::Resolved;
    manifest.packages.push_back(second);

    toml::table serialised = serialise(manifest);
    auto result = deserialise_forge_toml(serialised);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->packages.size(), 2);
    EXPECT_EQ(result->packages[0].package_name, "fmt");
    EXPECT_EQ(result->packages[1].package_name, "spdlog");
    EXPECT_EQ(result->packages[1].status, PackageStatus::Resolved);
}

TEST(ManifestSerialiser, MissingProjectInfoIsCaught) {
    toml::table broken{
        {"abi",
         serialise(Abi{
             .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"})},
        {"metadata", serialise(Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        {"packages", toml::array{}},
        // "project-info" deliberately omitted
    };

    auto result = deserialise_forge_toml(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "project-info");
}

TEST(ManifestSerialiser, MissingPackagesArrayIsCaught) {
    toml::table broken{
        {"project-info", serialise(ProjectInfo{.project_name = "dummy-project",
                                               .project_standard = "c++23",
                                               .project_build_type = "Debug"})},
        {"abi",
         serialise(Abi{
             .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"})},
        {"metadata", serialise(Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        // "packages" deliberately omitted
    };

    auto result = deserialise_forge_toml(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "packages");
}

TEST(ManifestSerialiser, OneBadPackageInArrayFailsWholeDeserialise) {
    // Short-circuit check: an array with a good package followed by a
    // broken one should surface the broken package's error, not silently
    // drop it or crash partway through deserialise_array.
    toml::table good_pkg =
        serialise(Package{.package_name = "fmt",
                          .version = "10.2.1",
                          .source = {.url = "u", .sha256 = "s", .commit_hash = "c"},
                          .status = PackageStatus::Unresolved});

    toml::table bad_pkg{
        {"package-name", "broken-pkg"},
        {"version", "1.0.0"},
        {"package-status", "unresolved"},
        // "git-source" deliberately omitted
    };

    toml::table broken{
        {"project-info",
         serialise(ProjectInfo{
             .project_name = "p", .project_standard = "c++23", .project_build_type = "Debug"})},
        {"abi",
         serialise(Abi{
             .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"})},
        {"metadata", serialise(Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        {"packages", toml::array{good_pkg, bad_pkg}},
    };

    auto result = deserialise_forge_toml(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "git-source");
}

} // namespace
} // namespace forge::serialiser
