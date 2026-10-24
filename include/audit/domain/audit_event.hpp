#pragma once

#include "audit/domain/action.hpp"
#include "audit/domain/instant.hpp"
#include "audit/domain/outcome.hpp"
#include "audit/domain/text.hpp"
#include <cstdint>
#include <map>
#include <optional>
#include <string>

namespace audit::domain {

struct AuditEvent {
    std::int64_t id;
    Instant recorded_at;
    std::string actor;
    Action action;
    std::optional<std::string> resource_type;
    std::optional<std::string> resource_id;
    Outcome outcome;
    std::optional<std::string> reason;
    std::map<std::string, std::string> metadata;

    AuditEvent(std::int64_t id, Instant recorded_at, std::string actor, Action action,
               std::optional<std::string> resource_type, std::optional<std::string> resource_id,
               Outcome outcome, std::optional<std::string> reason,
               std::map<std::string, std::string> metadata = {});
    bool operator==(const AuditEvent&) const = default;
};

}
