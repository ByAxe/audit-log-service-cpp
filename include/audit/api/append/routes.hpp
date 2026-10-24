#pragma once
#include "audit/application/append/append_service.hpp"
#include <httplib.h>
namespace audit::api::append {
void register_routes(httplib::Server&, application::append::AppendService&);
}
