#include "audit/domain/action.hpp"
#include "audit/domain/outcome.hpp"
#include <array>
#include <stdexcept>

namespace audit::domain {
namespace {
constexpr std::array<std::string_view, 7> actions = {
    "LOGIN", "LOGOUT", "EXPORT_DATA", "DELETE_RECORD", "ACCESS_PII", "GRANT_ROLE", "REVOKE_ROLE"};
constexpr std::array<std::string_view, 2> outcomes = {"ALLOWED", "DENIED"};
}

Action parse_action(std::string_view value) {
    for (std::size_t index = 0; index < actions.size(); ++index) {
        if (actions[index] == value) return static_cast<Action>(index);
    }
    throw std::invalid_argument("Invalid action");
}

std::string_view to_string(Action action) {
    const auto index = static_cast<std::size_t>(action);
    if (index >= actions.size()) throw std::invalid_argument("Invalid action");
    return actions[index];
}

Outcome parse_outcome(std::string_view value) {
    for (std::size_t index = 0; index < outcomes.size(); ++index) {
        if (outcomes[index] == value) return static_cast<Outcome>(index);
    }
    throw std::invalid_argument("Invalid outcome");
}

std::string_view to_string(Outcome outcome) {
    const auto index = static_cast<std::size_t>(outcome);
    if (index >= outcomes.size()) throw std::invalid_argument("Invalid outcome");
    return outcomes[index];
}

}
