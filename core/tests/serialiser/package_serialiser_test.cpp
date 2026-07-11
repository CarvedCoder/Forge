#include <Forge/serialiser/package_serialiser.h>

#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

GitSource make_sample_git_source() {
    return GitSource{
        .url = "https://github.com/fmtlib/fmt.git", .sha256 = "xyz1234", .commit_hash = "abc1234"};
}

Package make_sample_package() {
    Package pkg;
    pkg.package_name = "fmt";
    pkg.version = "10.2.1";
    pkg.source = make_sample_git_source();
    pkg.status = PackageStatus::Unresolved;
    return pkg;
}

ResolvedPackage make_sample_resolved_package() {
    ResolvedPackage resolved;
    resolved.name = "fmt";
    resolved.source = make_sample_git_source();
    return resolved;
}

// --- GitSource ---

TEST(GitSourceSerialiser, RoundTripsSuccessfully) {
    GitSource original = make_sample_git_source();
    toml::table serialised = serialise(original);

    auto result = deserialise_git_source(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->url, original.url);
    EXPECT_EQ(result->sha256, original.sha256);
    EXPECT_EQ(result->commit_hash, original.commit_hash);
}

TEST(GitSourceSerialiser, MissingCommitHashUsesHyphenatedKey) {
    // regression check: the key is "commit-hash", not "commit_hash"
    toml::table broken{
        {"url", "https://github.com/fmtlib/fmt.git"},
        {"sha256", "xyz1234"},
        // "commit-hash" deliberately omitted
    };

    auto result = deserialise_git_source(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "commit-hash");
}

// --- Package ---

TEST(PackageSerialiser, RoundTripsSuccessfully) {
    Package original = make_sample_package();
    toml::table serialised = serialise(original);

    auto result = deserialise_package(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->package_name, original.package_name);
    EXPECT_EQ(result->version, original.version);
    EXPECT_EQ(result->source.sha256, original.source.sha256);
    EXPECT_EQ(result->status, original.status);
}

TEST(PackageSerialiser, RoundTripsResolvedStatusToo) {
    Package original = make_sample_package();
    original.status = PackageStatus::Resolved;

    toml::table serialised = serialise(original);
    auto result = deserialise_package(serialised);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->status, PackageStatus::Resolved);
}

TEST(PackageSerialiser, MissingGitSourceIsCaughtNotNullDereferenced) {
    // Regression test for the deserialise_package null-pointer crash:
    // previously *git_source was dereferenced without checking the
    // expected<GitSource, Error> for failure first. This must return an
    // error, not crash, when git-source is absent.
    toml::table broken{
        {"package-name", "fmt"},
        {"version", "10.2.1"},
        {"package-status", "unresolved"},
        // "git-source" deliberately omitted
    };

    auto result = deserialise_package(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "git-source");
}

TEST(PackageSerialiser, IncompleteGitSourceInsidePackagePropagatesError) {
    // git-source table present, but missing a required field inside it —
    // confirms the nested-struct error propagates up rather than being
    // swallowed or dereferenced unchecked.
    toml::table broken{
        {"package-name", "fmt"},
        {"version", "10.2.1"},
        {"package-status", "unresolved"},
        {"git-source",
         toml::table{
             {"url", "https://github.com/fmtlib/fmt.git"},
             {"sha256", "xyz1234"},
             // "commit-hash" deliberately omitted
         }},
    };

    auto result = deserialise_package(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "commit-hash");
}

TEST(PackageSerialiser, MissingPackageStatusIsCaughtNotNullDereferenced) {
    // Regression test for the toml_tbl["package_status"] (underscore) vs
    // "package-status" (hyphen) key mismatch that caused a SEGV via
    // .node()->source() on a null node.
    toml::table broken{
        {"package-name", "fmt"},
        {"version", "10.2.1"},
        {"git-source", serialise(make_sample_git_source())},
        // "package-status" deliberately omitted
    };

    auto result = deserialise_package(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "package-status");
}

TEST(PackageSerialiser, InvalidPackageStatusStringIsRejected) {
    toml::table broken{
        {"package-name", "fmt"},
        {"version", "10.2.1"},
        {"git-source", serialise(make_sample_git_source())},
        {"package-status", "not-a-real-status"},
    };

    auto result = deserialise_package(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().kind, ErrorKind::TypeMismatch);
}

// --- ResolvedPackage ---

TEST(ResolvedPackageSerialiser, RoundTripsSuccessfully) {
    ResolvedPackage original = make_sample_resolved_package();
    toml::table serialised = serialise(original);

    auto result = deserialise_resolved_package(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->name, original.name);
    EXPECT_EQ(result->source.commit_hash, original.source.commit_hash);
}

TEST(ResolvedPackageSerialiser, MissingSourceIsCaught) {
    toml::table broken{
        {"name", "fmt"},
        // "source" deliberately omitted
    };

    auto result = deserialise_resolved_package(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "source");
}

} // namespace
} // namespace forge::serialiser
