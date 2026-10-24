#include "audit/persistence/migrate.hpp"

#include "libpq.hpp"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <map>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace audit::persistence {
namespace {

struct Migration {
  std::string script;
  std::string sql;
  std::string checksum;
};

std::string checksum(const std::string& sql) {
  std::uint64_t hash = 14695981039346656037ULL;
  for (const unsigned char byte : sql) hash = (hash ^ byte) * 1099511628211ULL;
  std::ostringstream value;
  value << std::hex << std::setfill('0') << std::setw(16) << hash;
  return value.str();
}

std::map<int, Migration> load_migrations(const std::filesystem::path& directory) {
  std::map<int, Migration> migrations;
  const std::regex naming(R"(^V([0-9]+)__[A-Za-z0-9_]+\.sql$)");
  for (const auto& entry : std::filesystem::directory_iterator(directory)) {
    if (!entry.is_regular_file() || entry.path().extension() != ".sql") continue;
    const auto script = entry.path().filename().string();
    std::smatch match;
    if (!std::regex_match(script, match, naming)) {
      throw std::runtime_error("Invalid versioned migration name: " + script);
    }
    const auto parsed_version = std::stoll(match[1]);
    if (parsed_version < 1 || parsed_version > 2147483647LL) {
      throw std::runtime_error("Migration version is outside the supported range: " + script);
    }
    std::ifstream file(entry.path(), std::ios::binary);
    if (!file) throw std::runtime_error("Cannot read migration: " + script);
    std::ostringstream contents;
    contents << file.rdbuf();
    if (file.bad()) throw std::runtime_error("Cannot read migration: " + script);
    auto sql = contents.str();
    const auto digest = checksum(sql);
    if (!migrations.emplace(static_cast<int>(parsed_version), Migration{script, std::move(sql), digest}).second) {
      throw std::runtime_error("Duplicate migration version: " + script);
    }
  }
  if (migrations.empty()) throw std::runtime_error("No versioned SQL migrations found");
  return migrations;
}

}  // namespace

void migrate(const std::string& conninfo, const std::filesystem::path& directory) {
  const auto migrations = load_migrations(directory);
  detail::Connection connection(conninfo);
  detail::Transaction transaction(connection);
  connection.exec("SELECT pg_advisory_xact_lock(4707471356761853767)");
  const auto existing = connection.exec(
      "SELECT to_regclass('audit_schema_history') IS NOT NULL, to_regclass('audit_events') IS NOT NULL");
  if (std::string(PQgetvalue(existing.get(), 0, 0)) == "f" &&
      std::string(PQgetvalue(existing.get(), 0, 1)) == "t") {
    throw std::runtime_error("Existing audit_events has no C++ migration history; use a new database");
  }
  connection.exec(
      "CREATE TABLE IF NOT EXISTS audit_schema_history ("
      "version integer PRIMARY KEY, script text NOT NULL, checksum text NOT NULL, "
      "installed_on timestamptz NOT NULL DEFAULT clock_timestamp())");
  const auto history = connection.exec("SELECT version, script, checksum FROM audit_schema_history ORDER BY version");
  int installed_version = 0;
  for (int row = 0; row < PQntuples(history.get()); ++row) {
    const auto version = std::stoi(PQgetvalue(history.get(), row, 0));
    const auto migration = migrations.find(version);
    if (migration == migrations.end() || migration->second.script != PQgetvalue(history.get(), row, 1) ||
        migration->second.checksum != PQgetvalue(history.get(), row, 2)) {
      throw std::runtime_error("Applied migration is missing or changed: V" + std::to_string(version));
    }
    installed_version = version;
  }
  for (const auto& [version, migration] : migrations) {
    if (version <= installed_version) {
      bool applied = false;
      for (int row = 0; row < PQntuples(history.get()); ++row) {
        if (version == std::stoi(PQgetvalue(history.get(), row, 0))) applied = true;
      }
      if (!applied) throw std::runtime_error("Out-of-order migration: " + migration.script);
      continue;
    }
    connection.exec(migration.sql);
    connection.exec("INSERT INTO audit_schema_history (version, script, checksum) VALUES ($1::integer, $2, $3)",
                    {std::to_string(version), migration.script, migration.checksum});
  }
  transaction.commit();
}

}  // namespace audit::persistence
