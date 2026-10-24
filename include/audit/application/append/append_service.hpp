#pragma once

#include "audit/application/append/append_command.hpp"
#include "audit/domain/append_repository.hpp"
#include <functional>

namespace audit::application::append {

class AppendService {
public:
    explicit AppendService(domain::AppendRepository& repository,
                           std::function<domain::Instant()> clock = domain::now_instant);
    domain::AuditEvent append(const AppendCommand& command);

private:
    domain::AppendRepository& repository_;
    std::function<domain::Instant()> clock_;
};

}
