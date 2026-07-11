#pragma once

#ifndef DESERIALISE_ARRAY
#define DESERIALISE_ARRAY_H

#include <Forge/schema/error.h>
#include <expected>

namespace forge::serialiser {
template <typename T>
[[nodiscard]] std::expected<std::vector<T>, Error>
deserialise_array(const toml::array& toml_arr,
                  std::expected<T, Error> (*parse_element)(const toml::table& toml_tbl)) {
    std::vector<T> result;
    result.reserve(toml_arr.size());

    for (const auto& node : toml_arr) {
        const toml::table* tbl = node.as_table();
        if (!tbl)
            return std::unexpected(
                Error{ErrorKind::TypeMismatch, "package_array_element", tbl->source()});
        auto item = parse_element(*tbl);
        if (!item)
            return std::unexpected(item.error());
        result.emplace_back(std::move(*item));
    }
    return result;
}
} // namespace forge::serialiser

#endif // !DESERIALISE_ARRAY_H
