#include "audit/persistence/postgres_append_repository.hpp"

#include "../instant.hpp"
#include "../libpq.hpp"

#include <nlohmann/json.hpp>

namespace audit::persistence {

std::int64_t PostgresAppendRepository::next_id() {
  detail::Connection connection(conninfo_);
  const auto result = connection.exec("SELECT nextval('audit_events_id_seq')");
  return std::stoll(PQgetvalue(result.get(), 0, 0));
}

domain::AuditEvent PostgresAppendRepository::append(const domain::AuditEvent& event) {
  detail::Connection connection(conninfo_);
  connection.exec(
      "INSERT INTO audit_events "
      "(id, recorded_at, actor, action, resource_type, resource_id, outcome, reason, metadata) "
      "VALUES ($1::bigint, $2::timestamptz, $3, $4, $5, $6, $7, $8, $9::jsonb)",
      {std::to_string(event.id), detail::to_pg_instant(event.recorded_at), event.actor,
       std::string(domain::to_string(event.action)), event.resource_type, event.resource_id,
       std::string(domain::to_string(event.outcome)), event.reason, nlohmann::json(event.metadata).dump()});
  return event;
}

}  // namespace audit::persistence
