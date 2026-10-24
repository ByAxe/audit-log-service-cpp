#pragma once

#include "audit/domain/audit_event.hpp"

namespace audit::domain {

class AppendRepository {
public:
    virtual ~AppendRepository() = default;
    virtual std::int64_t next_id() = 0;
    virtual AuditEvent append(const AuditEvent& event) = 0;
};

}
