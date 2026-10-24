#pragma once

#include "audit/application/query/query.hpp"
#include "audit/domain/read_repository.hpp"

namespace audit::application::query {

class QueryService {
public:
    explicit QueryService(domain::ReadRepository& repository);
    domain::EventPage find(const Query& query);

private:
    domain::ReadRepository& repository_;
};

}
