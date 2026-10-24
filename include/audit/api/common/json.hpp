#pragma once
#include "audit/domain/audit_event.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>

namespace audit::api {
using Json = nlohmann::json;
Json event_json(const domain::AuditEvent& event);
std::optional<std::string> optional_text(const Json& object, const char* field);
std::optional<domain::Action> optional_action(const Json& object, const char* field);
std::optional<domain::Outcome> optional_outcome(const Json& object, const char* field);
bool json_content_type(std::string_view value);
}
