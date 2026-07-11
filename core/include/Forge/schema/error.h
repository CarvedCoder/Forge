#pragma once

#include <format>
#include <sstream>
#ifndef ERROR_H
#define ERROR_H
#include <source_location>
#include <string>
#include <toml++/toml.hpp>
namespace forge::serialiser {
enum class ErrorKind { MissingField, TypeMismatch };

constexpr std::string_view to_string(ErrorKind kind) {
    switch (kind) {
    case ErrorKind::MissingField:
        return "MissingField";
    case ErrorKind::TypeMismatch:
        return "TypeMismatch";
    }
    return "Unknown";
}

struct Error {
    ErrorKind kind;
    std::string field_name;
    toml::source_region toml_source;
    std::source_location raised_at = std::source_location::current();
};

} // namespace forge::serialiser

template <> struct std::formatter<forge::serialiser::Error> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const forge::serialiser::Error& err, format_context& ctx) const {
        ostringstream loc;
        loc << err.toml_source;

        return format_to(ctx.out(), "{} in field '{}' ({})", to_string(err.kind), err.field_name,
                         loc.str());
    }
};

#endif // !ERROR_H
