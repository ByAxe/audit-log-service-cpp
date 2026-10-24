#include "audit/api/common/json.hpp"
#include "audit/api/common/parameters.hpp"
#include "audit/domain/instant.hpp"
#include <charconv>
#include <stdexcept>

namespace audit::api {
namespace {
Json nullable(const std::optional<std::string>& value) {
  return value ? Json(*value) : Json(nullptr);
}

template<class Enum, class Parser>
std::optional<Enum> optional_enum(const Json& object, const char* field, Enum last, Parser parser) {
  if (!object.contains(field) || object.at(field).is_null()) return std::nullopt;
  const auto& value = object.at(field);
  const auto maximum = static_cast<std::int64_t>(last);
  if (value.is_number_unsigned()) {
    const auto ordinal = value.get<std::uint64_t>();
    if (ordinal <= static_cast<std::uint64_t>(maximum)) return static_cast<Enum>(ordinal);
  } else if (value.is_number_integer()) {
    const auto ordinal = value.get<std::int64_t>();
    if (ordinal >= 0 && ordinal <= maximum) return static_cast<Enum>(ordinal);
  } else if (value.is_string()) {
    const auto& text = value.get_ref<const std::string&>();
    const auto trimmed = trim_parameter(text);
    if (trimmed.empty()) return std::nullopt;
    if (trimmed.front() >= '0' && trimmed.front() <= '9' &&
        !(trimmed.front() == '0' && trimmed.size() > 1)) {
      std::int32_t ordinal;
      const auto [end, error] = std::from_chars(trimmed.data(), trimmed.data() + trimmed.size(), ordinal);
      if (error == std::errc{} && end == trimmed.data() + trimmed.size() && ordinal <= maximum) {
        return static_cast<Enum>(ordinal);
      }
    }
    return parser(trimmed);
  }
  throw std::invalid_argument(std::string("Invalid ") + field);
}
}
Json event_json(const domain::AuditEvent& event) {
  return {{"id", event.id}, {"recordedAt", domain::format_instant(event.recorded_at)},
          {"actor", event.actor}, {"action", domain::to_string(event.action)},
          {"resourceType", nullable(event.resource_type)}, {"resourceId", nullable(event.resource_id)},
          {"outcome", domain::to_string(event.outcome)}, {"reason", nullable(event.reason)},
          {"metadata", event.metadata}};
}
std::optional<std::string> optional_text(const Json& object, const char* field) {
  if (!object.contains(field) || object.at(field).is_null()) return std::nullopt;
  const auto& value = object.at(field);
  if (value.is_string()) return value.get<std::string>();
  // Jackson's String fields also accept scalar JSON values.
  if (value.is_boolean() || value.is_number()) return value.dump();
  throw std::invalid_argument(std::string(field) + " must be a string");
}

std::optional<domain::Action> optional_action(const Json& object, const char* field) {
  return optional_enum(object, field, domain::Action::REVOKE_ROLE, domain::parse_action);
}

std::optional<domain::Outcome> optional_outcome(const Json& object, const char* field) {
  return optional_enum(object, field, domain::Outcome::DENIED, domain::parse_outcome);
}

bool json_content_type(std::string_view value) {
  value = trim_parameter(value.substr(0, value.find(';')));
  std::string media_type(value);
  for (auto& character : media_type) {
    if (character >= 'A' && character <= 'Z') character += 'a' - 'A';
  }
  if (!media_type.starts_with("application/")) return false;
  const auto subtype = std::string_view(media_type).substr(12);
  for (const unsigned char character : subtype) {
    if (character <= 0x20 || character >= 0x7F ||
        std::string_view("()<>@,;:\\\"/[]?={}").find(character) != std::string_view::npos) return false;
  }
  return subtype == "json" || (subtype.size() > 5 && subtype.ends_with("+json"));
}
}
