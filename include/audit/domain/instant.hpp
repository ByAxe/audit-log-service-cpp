#pragma once

#include <compare>
#include <cstdint>
#include <string>
#include <string_view>

namespace audit::domain {

struct Instant {
    std::int64_t seconds;
    std::int32_t nanos;

    Instant(std::int64_t seconds = 0, std::int32_t nanos = 0);
    auto operator<=>(const Instant&) const = default;
};

Instant parse_instant(std::string_view value);
std::string format_instant(const Instant& value);
Instant now_instant();

}
