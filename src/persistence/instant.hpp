#pragma once

#include "audit/domain/instant.hpp"

#include <stdexcept>
#include <string>

namespace audit::persistence::detail {

inline std::string iso_year(std::int64_t year) {
  auto digits = std::to_string(year < 0 ? -year : year);
  if (digits.size() < 4) digits.insert(0, 4 - digits.size(), '0');
  return (year < 0 ? "-" : year > 9999 ? "+" : "") + digits;
}

inline std::string to_pg_instant(const domain::Instant& value) {
  auto text = domain::format_instant(value);
  const auto year_end = text.find('-', text.front() == '-' ? 1 : 0);
  const auto year = std::stoll(text.substr(0, year_end));
  if (year > 0) {
    if (text.front() == '+') text.erase(0, 1);
    return text;
  }
  // PostgreSQL counts BC years from 1; ISO-8601 includes year zero.
  auto year_text = iso_year(1 - year);
  if (year_text.front() == '+') year_text.erase(0, 1);
  text.replace(0, year_end, year_text);
  text[text.find('T')] = ' ';
  text.pop_back();
  return text + "+00 BC";
}

inline domain::Instant from_pg_instant(std::string text) {
  const bool before_christ = text.ends_with(" BC");
  if (!before_christ && !text.ends_with(" AD")) {
    throw std::invalid_argument("PostgreSQL timestamp is missing its era");
  }
  text.resize(text.size() - 3);
  const auto year_end = text.find('-');
  const auto year = std::stoll(text.substr(0, year_end));
  text.replace(0, year_end, iso_year(before_christ ? 1 - year : year));
  return domain::parse_instant(text);
}

}  // namespace audit::persistence::detail
