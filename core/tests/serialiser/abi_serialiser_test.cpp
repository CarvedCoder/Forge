#include <Forge/serialiser/abi_serialiser.h>

#include <gtest/gtest.h>

namespace forge::serialiser {
namespace {

Abi make_sample_abi() {
    return Abi{.compiler = "gcc", .stdlib = "libstdc++", .cxx_standard = "23", .c_standard = "17"};
}

TEST(AbiSerialiser, SerialisesToExpectedKeys) {
    toml::table tbl = serialise(make_sample_abi());

    EXPECT_TRUE(tbl.contains("compiler"));
    EXPECT_TRUE(tbl.contains("stdlib"));
    EXPECT_TRUE(tbl.contains("cxx-standard"));
    EXPECT_TRUE(tbl.contains("c-standard"));

    EXPECT_EQ(tbl["compiler"].value<std::string>(), "gcc");
    EXPECT_EQ(tbl["cxx-standard"].value<std::string>(), "23");
}

TEST(AbiSerialiser, RoundTripsSuccessfully) {
    Abi original = make_sample_abi();
    toml::table serialised = serialise(original);

    auto result = deserialise_abi(serialised);
    ASSERT_TRUE(result.has_value()) << "unexpected error: " << result.error().field_name;

    EXPECT_EQ(result->compiler, original.compiler);
    EXPECT_EQ(result->stdlib, original.stdlib);
    EXPECT_EQ(result->cxx_standard, original.cxx_standard);
    EXPECT_EQ(result->c_standard, original.c_standard);
}

TEST(AbiSerialiser, MissingCompilerReturnsCorrectFieldName) {
    toml::table broken{
        {"stdlib", "libstdc++"},
        {"cxx-standard", "23"},
        {"c-standard", "17"},
        // "compiler" deliberately omitted
    };

    auto result = deserialise_abi(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().kind, ErrorKind::MissingField);
    EXPECT_EQ(result.error().field_name, "compiler");
}

TEST(AbiSerialiser, MissingStdlibReturnsCorrectFieldName) {
    toml::table broken{
        {"compiler", "gcc"},
        {"cxx-standard", "23"},
        {"c-standard", "17"},
        // "stdlib" deliberately omitted
    };

    auto result = deserialise_abi(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "stdlib");
}

TEST(AbiSerialiser, WrongTypeIsRejectedNotSilentlyCoerced) {
    // cxx-standard given as an integer instead of a string.
    toml::table broken{
        {"compiler", "gcc"},
        {"stdlib", "libstdc++"},
        {"cxx-standard", 23}, // wrong type
        {"c-standard", "17"},
    };

    auto result = deserialise_abi(broken);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().field_name, "cxx-standard");
}

} // namespace
} // namespace forge::serialiser
