#pragma once

#include "audit/domain/action.hpp"
#include "audit/domain/outcome.hpp"
#include <map>
#include <optional>
#include <string>

namespace audit::application::append {

struct AppendCommand {
    std::string actor;
    domain::Action action;
    std::optional<std::string> resource_type;
    std::optional<std::string> resource_id;
    domain::Outcome outcome;
    std::optional<std::string> reason;
    std::map<std::string, std::string> metadata;
};

}
