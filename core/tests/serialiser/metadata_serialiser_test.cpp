#include <Forge/serialiser/metadata_serialiser.h>

#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

Metadata make_sample_metadata() {
    return Metadata{.forge_version = "0.1.0", .cmake_version = "3.28"};
}

TEST(MetadataSerialiser, RoundTripsSuccessfully) {
    Metadata original = make_sample_metadata();
    toml::table serialised = serialise(original);

    auto result = deserialise_metadata(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->forge_version, original.forge_version);
    EXPECT_EQ(result->cmake_version, original.cmake_version);
}

TEST(MetadataSerialiser, MissingForgeVersionIsCaught) {
    toml::table broken{
        {"cmake-version", "3.28"},
        // "forge-version" deliberately omitted
    };

    auto result = deserialise_metadata(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().kind, ErrorKind::MissingField);
}

} // namespace
} // namespace forge::serialiser
