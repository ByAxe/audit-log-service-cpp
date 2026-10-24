#pragma once

#include <string_view>

namespace audit::domain {

enum class Action { LOGIN, LOGOUT, EXPORT_DATA, DELETE_RECORD, ACCESS_PII, GRANT_ROLE, REVOKE_ROLE };

Action parse_action(std::string_view value);
std::string_view to_string(Action action);

}
