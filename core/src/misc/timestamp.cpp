#include <Forge/misc/timestamp.h>
#include <chrono>
#include <string>

namespace forge::timestamp {

std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();

    auto zoned = std::chrono::zoned_time{std::chrono::current_zone(), now};

    return std::format("{:%FT%T%Ez}", zoned);
}
} // namespace forge::timestamp
