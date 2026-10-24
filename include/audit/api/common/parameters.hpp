#pragma once

#include "audit/domain/action.hpp"
#include "audit/domain/outcome.hpp"
#include "audit/domain/instant.hpp"
#include "audit/domain/text.hpp"
#include <charconv>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace audit::api {

// Spring and Jackson trim Java String.trim characters only at the HTTP boundary.
inline std::string_view trim_parameter(std::string_view value) {
  while (!value.empty() && static_cast<unsigned char>(value.front()) <= 0x20) value.remove_prefix(1);
  while (!value.empty() && static_cast<unsigned char>(value.back()) <= 0x20) value.remove_suffix(1);
  return value;
}

inline domain::Action parameter_action(std::string_view value) {
  return domain::parse_action(trim_parameter(value));
}

inline domain::Outcome parameter_outcome(std::string_view value) {
  return domain::parse_outcome(trim_parameter(value));
}

template<class T>
T parameter_integer(std::string_view value) {
  // Spring NumberUtils removes Java whitespace and also accepts signed hexadecimal input.
  std::string number;
  for (std::size_t index = 0; index < value.size();) {
    const auto first = static_cast<unsigned char>(value[index]);
    const std::size_t length = first < 0x80 ? 1 : first < 0xE0 ? 2 : first < 0xF0 ? 3 : 4;
    const auto codepoint = value.substr(index, length);
    if (!domain::is_blank(codepoint)) number.append(codepoint);
    index += codepoint.size();
  }
  const bool negative = number.starts_with('-');
  const bool positive_sign = number.starts_with('+');
  std::string_view digits(number);
  if (digits.starts_with('-') || digits.starts_with('+')) digits.remove_prefix(1);
  int base = 10;
  if (digits.starts_with("0x") || digits.starts_with("0X")) { base = 16; digits.remove_prefix(2); }
  else if (digits.starts_with('#')) { base = 16; digits.remove_prefix(1); }
  if (positive_sign && base == 16) throw std::invalid_argument("invalid integer");
  if (digits.starts_with('-') || digits.starts_with('+')) throw std::invalid_argument("invalid integer");
  std::string normalized = negative ? "-" : "";
  normalized.append(digits);
  T result;
  const auto [end, error] = std::from_chars(normalized.data(), normalized.data() + normalized.size(), result, base);
  if (error != std::errc{} || end != normalized.data() + normalized.size()) {
    throw std::invalid_argument("invalid integer");
  }
  return result;
}

inline domain::Instant parameter_instant(std::string_view value) {
  auto decimal = value;
  if (decimal.starts_with('+')) decimal.remove_prefix(1);
  std::int64_t millis;
  const auto [end, error] = std::from_chars(decimal.data(), decimal.data() + decimal.size(), millis);
  if (error == std::errc{} && end == decimal.data() + decimal.size() &&
      !(value.starts_with('+') && decimal.starts_with('-'))) {
    auto seconds = millis / 1000;
    auto remainder = millis % 1000;
    if (remainder < 0) { --seconds; remainder += 1000; }
    return {seconds, static_cast<std::int32_t>(remainder * 1000000)};
  }
  if (!value.empty() && ((value.front() >= 'A' && value.front() <= 'Z') ||
                        (value.front() >= 'a' && value.front() <= 'z'))) {
    const bool weekday = value.find(',') != std::string_view::npos;
    std::tm date{};
    std::istringstream input{std::string(value)};
    input.imbue(std::locale::classic());
    int expected_weekday = -1;
    if (weekday) {
      input >> std::get_time(&date, "%a, ");
      expected_weekday = date.tm_wday;
    }
    input >> std::get_time(&date, "%d %b %Y %H:%M");
    if (input.peek() == ':') { input.get(); input >> std::get_time(&date, "%S"); }
    std::string zone, trailing;
    if (input.fail() || !(input >> zone) || (input >> trailing)) throw std::invalid_argument("invalid HTTP date");
    std::string offset;
    for (auto& character : zone) {
      if (character >= 'A' && character <= 'Z') character += 'a' - 'A';
    }
    if (zone == "gmt") offset = "Z";
    else if (zone.size() == 5 && (zone[0] == '+' || zone[0] == '-')) offset = zone.substr(0, 3) + ':' + zone.substr(3);
    else throw std::invalid_argument("invalid HTTP date zone");
    std::ostringstream iso;
    iso << std::put_time(&date, "%Y-%m-%dT%H:%M:%S");
    const auto local = domain::parse_instant(iso.str() + "Z");
    auto days = local.seconds / 86400;
    if (local.seconds < 0 && local.seconds % 86400) --days;
    if (weekday && ((days + 4) % 7 + 7) % 7 != expected_weekday) throw std::invalid_argument("invalid HTTP date weekday");
    return domain::parse_instant(iso.str() + offset);
  }
  return domain::parse_instant(value);
}

}
