#include "audit/application/append/append_service.hpp"
#include <stdexcept>
#include <utility>

namespace audit::application::append {

AppendService::AppendService(domain::AppendRepository& repository,
                             std::function<domain::Instant()> clock)
    : repository_(repository), clock_(std::move(clock)) {
    if (!clock_) throw std::invalid_argument("clock must not be empty");
}

domain::AuditEvent AppendService::append(const AppendCommand& command) {
    const auto id = repository_.next_id();
    const auto recorded_at = clock_();
    const domain::AuditEvent event(id, recorded_at, command.actor, command.action,
                                   command.resource_type, command.resource_id, command.outcome,
                                   command.reason, command.metadata);
    return repository_.append(event);
}

}
