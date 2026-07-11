#pragma once
#include "Forge/schema/error.h"
#include <expected>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#ifndef GET_FIELD_H
#define GET_FIELD_H

namespace forge::serialiser {
template <typename T>
[[nodiscard]] std::expected<T, Error> get_field(const toml::table& toml_tbl, std::string_view key) {
    auto node = toml_tbl[key];
    auto key_node = node.value<T>();
    if (!key_node) {
        return std::unexpected<Error>(
            Error{.kind = ErrorKind::MissingField,
                  .field_name = std::string{key},
                  .toml_source = node.node() ? node.node()->source() : toml_tbl.source()});
    }
    return *key_node;
}

} // namespace forge::serialiser

#endif // !FIELD_H
