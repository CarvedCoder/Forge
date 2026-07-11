#include "Forge/serialiser/get_field.h"
#include <Forge/schema/abi.h>
#include <Forge/serialiser/abi_serialiser.h>
#include <expected>
#include <toml++/toml.hpp>

namespace forge::serialiser {
toml::table serialise(const Abi& abi) {
    return toml::table{{"compiler", abi.compiler},
                       {"stdlib", abi.stdlib},
                       {"cxx-standard", abi.cxx_standard},
                       {"c-standard", abi.c_standard}};
}
[[nodiscard]] std::expected<Abi, Error> deserialise_abi(const toml::table& toml_tbl) {
    auto compiler = get_field<std::string>(toml_tbl, "compiler");
    if (!compiler)
        return std::unexpected(compiler.error());
    auto stdlib = get_field<std::string>(toml_tbl, "stdlib");
    if (!stdlib)
        return std::unexpected(stdlib.error());
    auto cxx_standard = get_field<std::string>(toml_tbl, "cxx-standard");
    if (!cxx_standard)
        return std::unexpected(cxx_standard.error());
    auto c_standard = get_field<std::string>(toml_tbl, "c-standard");
    if (!c_standard)
        return std::unexpected(c_standard.error());

    return Abi{.compiler = *compiler,
               .stdlib = *stdlib,
               .cxx_standard = *cxx_standard,
               .c_standard = *c_standard};
}

} // namespace forge::serialiser
