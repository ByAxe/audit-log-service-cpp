#pragma once

#include <string_view>

namespace audit::domain {

enum class Outcome { ALLOWED, DENIED };

Outcome parse_outcome(std::string_view value);
std::string_view to_string(Outcome outcome);

}
