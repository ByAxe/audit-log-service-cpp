#include "audit/api/append/routes.hpp"
#include "audit/api/query/routes.hpp"
#include "audit/api/common/json.hpp"
#include "audit/persistence/postgres.hpp"
#include "audit/persistence/migrate.hpp"
#include <cstdlib>
#include <iostream>

namespace {
std::string env(const char* name, const char* fallback) {
  const auto* value = std::getenv(name);
  return value ? value : fallback;
}
}
int main() {
  try {
    const auto conninfo = env("AUDIT_LOG_DB_URL", "host=localhost port=5432 dbname=audit_log user=audit password=audit");
    audit::persistence::migrate(conninfo, env("AUDIT_LOG_MIGRATIONS", "db/migration"));
    audit::persistence::PostgresAppendRepository append_repository(conninfo);
    audit::persistence::PostgresReadRepository read_repository(conninfo);
    audit::application::append::AppendService append_service(append_repository);
    audit::application::query::QueryService query_service(read_repository);
    httplib::Server server;
    audit::api::append::register_routes(server, append_service);
    audit::api::query::register_routes(server, query_service);
    server.set_exception_handler([](const auto&, auto& response, std::exception_ptr exception) {
      std::string message = "Internal Server Error";
      response.status = 500;
      try { if (exception) std::rethrow_exception(exception); }
      catch (const std::invalid_argument& error) { response.status = 400; message = error.what(); }
      catch (const nlohmann::json::exception&) { response.status = 400; message = "Malformed JSON request"; }
      catch (const std::exception& error) { std::cerr << "request failed: " << error.what() << '\n'; }
      response.set_content(audit::api::Json{{"message", message}}.dump(), "application/json");
    });
    const auto host = env("AUDIT_LOG_HOST", "127.0.0.1");
    const auto port_text = env("AUDIT_LOG_PORT", "8080");
    std::size_t consumed = 0;
    const int port = std::stoi(port_text, &consumed);
    if (consumed != port_text.size() || port < 1 || port > 65535) throw std::invalid_argument("invalid AUDIT_LOG_PORT");
    std::cout << "Audit log service listening on " << host << ':' << port << std::endl;
    if (!server.listen(host, port)) throw std::runtime_error("HTTP listener failed");
  } catch (const std::exception& error) {
    std::cerr << "startup failed: " << error.what() << '\n';
    return 1;
  }
}
