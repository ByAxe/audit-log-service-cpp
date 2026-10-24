#pragma once
#include "audit/application/query/query_service.hpp"
#include <httplib.h>
namespace audit::api::query {
void register_routes(httplib::Server&, application::query::QueryService&);
}
