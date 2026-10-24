#include "audit/api/append/routes.hpp"
#include "audit/api/common/json.hpp"
#include "audit/domain/action.hpp"
#include "audit/domain/outcome.hpp"
#include "audit/domain/text.hpp"
#include <map>
#include <stdexcept>
#include <vector>

namespace audit::api::append {
void register_routes(httplib::Server& server, application::append::AppendService& service) {
  server.Post("/api/v1/audit-events", [&service](const auto& request, auto& response) {
    if (!json_content_type(request.get_header_value("Content-Type"))) {
      response.status = 415;
      response.set_content(Json{{"message", "Content-Type must be application/json"}}.dump(), "application/json");
      return;
    }
    const auto body = Json::parse(request.body);
    if (!body.is_object()) throw std::invalid_argument("request body must be an object");
    auto actor = optional_text(body, "actor");
    auto action = optional_action(body, "action");
    auto outcome = optional_outcome(body, "outcome");
    std::vector<std::string> errors;
    if (!actor || domain::is_blank(*actor)) errors.emplace_back("actor must not be blank");
    if (!action) errors.emplace_back("action must not be null");
    if (!outcome) errors.emplace_back("outcome must not be null");
    if (!errors.empty()) {
      std::string message;
      for (const auto& error : errors) { if (!message.empty()) message += "; "; message += error; }
      throw std::invalid_argument(message);
    }
    std::map<std::string, std::string> metadata;
    if (body.contains("metadata") && !body.at("metadata").is_null()) {
      if (!body.at("metadata").is_object()) throw std::invalid_argument("metadata must be an object");
      for (const auto& [key, value] : body.at("metadata").items()) {
        auto text = optional_text(body.at("metadata"), key.c_str());
        if (!text) throw std::runtime_error("null metadata value");
        metadata.emplace(key, *text);
      }
    }
    const auto event = service.append({*actor, *action,
        optional_text(body, "resourceType"), optional_text(body, "resourceId"),
        *outcome, optional_text(body, "reason"), std::move(metadata)});
    response.status = 201;
    response.set_header("Location", "http://" + request.get_header_value("Host") +
        "/api/v1/audit-events/" + std::to_string(event.id));
    response.set_content(event_json(event).dump(), "application/json");
  });
}
}
