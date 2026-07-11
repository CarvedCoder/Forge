#include <Forge/serialiser/abi_serialiser.h>
#include <Forge/serialiser/manifest_serialiser.h>
#include <Forge/serialiser/metadata_serialiser.h>
#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

ForgeLock make_sample_lock() {
    ForgeLock lock;
    lock.metadata = {.forge_version = "0.1.0", .cmake_version = "3.28"};
    lock.abi = {.compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};

    ResolvedPackage resolved;
    resolved.name = "fmt";
    resolved.source = {
        .url = "https://github.com/fmtlib/fmt.git", .sha256 = "xyz1234", .commit_hash = "abc1234"};
    lock.resolved_packages.push_back(resolved);

    return lock;
}

TEST(LockSerialiser, RoundTripsSuccessfully) {
    ForgeLock original = make_sample_lock();
    toml::table serialised = serialise(original);

    auto result = deserialise_forge_lock(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    const ForgeLock& r = *result;
    EXPECT_EQ(r.metadata.forge_version, original.metadata.forge_version);
    EXPECT_EQ(r.abi.compiler, original.abi.compiler);
    ASSERT_EQ(r.resolved_packages.size(), original.resolved_packages.size());
    EXPECT_EQ(r.resolved_packages[0].name, original.resolved_packages[0].name);
    EXPECT_EQ(r.resolved_packages[0].source.commit_hash,
              original.resolved_packages[0].source.commit_hash);
}

TEST(LockSerialiser, RoundTripsWithMultipleResolvedPackages) {
    ForgeLock lock = make_sample_lock();

    ResolvedPackage second;
    second.name = "spdlog";
    second.source = {.url = "u2", .sha256 = "s2", .commit_hash = "c2"};
    lock.resolved_packages.push_back(second);

    toml::table serialised = serialise(lock);
    auto result = deserialise_forge_lock(serialised);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->resolved_packages.size(), 2);
    EXPECT_EQ(result->resolved_packages[1].name, "spdlog");
}

TEST(LockSerialiser, MissingResolvedPackagesArrayIsCaught) {
    toml::table broken{
        {"metadata", serialise(Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        {"abi",
         serialise(Abi{
             .compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"})},
        // "resolved-packages" deliberately omitted
    };

    auto result = deserialise_forge_lock(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "resolved-packages");
}

TEST(LockSerialiser, MissingAbiIsCaught) {
    toml::table broken{
        {"metadata", serialise(Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"})},
        {"resolved-packages", toml::array{}},
        // "abi" deliberately omitted
    };

    auto result = deserialise_forge_lock(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "abi");
}

} // namespace
} // namespace forge::serialiser
