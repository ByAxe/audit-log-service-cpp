#pragma once

#include "audit/persistence/database_error.hpp"

#include <libpq-fe.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace audit::persistence::detail {

using Result = std::unique_ptr<PGresult, decltype(&PQclear)>;
using Parameters = std::vector<std::optional<std::string>>;

// shortcut: one connection per repository call; add pooling when concurrent load requires it.
class Connection {
 public:
  explicit Connection(const std::string& conninfo) : connection_(PQconnectdb(conninfo.c_str()), PQfinish) {
    if (!connection_ || PQstatus(connection_.get()) != CONNECTION_OK) {
      throw DatabaseError(connection_ ? PQerrorMessage(connection_.get()) : "Cannot allocate PostgreSQL connection", "08001");
    }
  }

  Result exec(const std::string& sql) {
    return check(PQexec(connection_.get(), sql.c_str()));
  }

  Result exec(const std::string& sql, const Parameters& parameters) {
    std::vector<const char*> values;
    values.reserve(parameters.size());
    for (const auto& parameter : parameters) {
      if (parameter && parameter->find('\0') != std::string::npos) {
        throw DatabaseError("PostgreSQL text cannot contain a zero byte", "22021");
      }
      values.push_back(parameter ? parameter->c_str() : nullptr);
    }
    return check(PQexecParams(connection_.get(), sql.c_str(), static_cast<int>(values.size()),
                              nullptr, values.data(), nullptr, nullptr, 0));
  }

 private:
  Result check(PGresult* raw) {
    Result result(raw, PQclear);
    if (!result) throw DatabaseError(PQerrorMessage(connection_.get()), "");
    const auto status = PQresultStatus(result.get());
    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
      const char* sqlstate = PQresultErrorField(result.get(), PG_DIAG_SQLSTATE);
      throw DatabaseError(PQresultErrorMessage(result.get()), sqlstate ? sqlstate : "");
    }
    return result;
  }

  std::unique_ptr<PGconn, decltype(&PQfinish)> connection_;
};

class Transaction {
 public:
  explicit Transaction(Connection& connection, bool read_only = false) : connection_(connection) {
    connection_.exec(read_only ? "BEGIN READ ONLY" : "BEGIN");
  }
  ~Transaction() {
    if (!committed_) {
      try { connection_.exec("ROLLBACK"); } catch (...) {}
    }
  }
  Transaction(const Transaction&) = delete;
  Transaction& operator=(const Transaction&) = delete;
  void commit() {
    connection_.exec("COMMIT");
    committed_ = true;
  }

 private:
  Connection& connection_;
  bool committed_ = false;
};

inline std::optional<std::string> nullable_text(const Result& result, int row, int column) {
  return PQgetisnull(result.get(), row, column) ? std::nullopt
                                             : std::optional<std::string>(PQgetvalue(result.get(), row, column));
}

}  // namespace audit::persistence::detail
