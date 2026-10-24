#pragma once

#include "audit/domain/append_repository.hpp"

#include <string>
#include <utility>

namespace audit::persistence {

class PostgresAppendRepository final : public domain::AppendRepository {
 public:
  explicit PostgresAppendRepository(std::string conninfo) : conninfo_(std::move(conninfo)) {}
  std::int64_t next_id() override;
  domain::AuditEvent append(const domain::AuditEvent& event) override;

 private:
  std::string conninfo_;
};

}  // namespace audit::persistence
