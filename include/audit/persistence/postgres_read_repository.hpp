#pragma once

#include "audit/domain/read_repository.hpp"

#include <string>
#include <utility>

namespace audit::persistence {

class PostgresReadRepository final : public domain::ReadRepository {
 public:
  explicit PostgresReadRepository(std::string conninfo) : conninfo_(std::move(conninfo)) {}
  domain::EventPage find(const domain::Search& search) override;
  std::optional<std::int64_t> max_id() override;

 private:
  std::string conninfo_;
};

}  // namespace audit::persistence
