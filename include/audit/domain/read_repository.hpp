#pragma once

#include "audit/domain/event_page.hpp"
#include "audit/domain/search.hpp"

namespace audit::domain {

class ReadRepository {
public:
    virtual ~ReadRepository() = default;
    virtual EventPage find(const Search& search) = 0;
};

}
