#include <Forge/serialiser/project_serialiser.h>

#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

ProjectInfo make_sample_project() {
    return ProjectInfo{.project_name = "dummy-project",
                       .project_standard = "c++23",
                       .project_build_type = "Debug"};
}

TEST(ProjectSerialiser, RoundTripsSuccessfully) {
    ProjectInfo original = make_sample_project();
    toml::table serialised = serialise(original);

    auto result = deserialise_project_info(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->project_name, original.project_name);
    EXPECT_EQ(result->project_standard, original.project_standard);
    EXPECT_EQ(result->project_build_type, original.project_build_type);
}

TEST(ProjectSerialiser, EmptyTableReturnsMissingFieldForFirstKey) {
    toml::table empty;

    auto result = deserialise_project_info(empty);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().kind, ErrorKind::MissingField);
}

TEST(ProjectSerialiser, MissingBuildTypeReturnsCorrectFieldName) {
    toml::table broken{
        {"project-name", "dummy-project"},
        {"project-standard", "c++23"},
        // "project-build-type" deliberately omitted
    };

    auto result = deserialise_project_info(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "project-build-type");
}

} // namespace
} // namespace forge::serialiser
