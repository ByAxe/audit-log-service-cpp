#include "audit/domain/instant.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace audit::domain {
namespace {
constexpr std::int64_t minimum_second = -31557014167219200LL;
constexpr std::int64_t maximum_second = 31556889864403199LL;

std::int64_t days_from_civil(std::int64_t year, unsigned month, unsigned day) {
    year -= month <= 2;
    const auto era = (year >= 0 ? year : year - 399) / 400;
    const auto year_of_era = static_cast<unsigned>(year - era * 400);
    const auto day_of_year = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1;
    return era * 146097 + year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year - 719468;
}

bool leap_year(std::int64_t year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

[[noreturn]] void invalid_instant() {
    throw std::invalid_argument("Invalid ISO-8601 instant");
}

int digits(std::string_view value, std::size_t& position, std::size_t count) {
    if (position + count > value.size()) invalid_instant();
    int result = 0;
    for (std::size_t index = 0; index < count; ++index) {
        const char digit = value[position++];
        if (digit < '0' || digit > '9') invalid_instant();
        result = result * 10 + digit - '0';
    }
    return result;
}

void separator(std::string_view value, std::size_t& position, char expected) {
    if (position >= value.size() || value[position++] != expected) invalid_instant();
}
}

Instant::Instant(std::int64_t seconds, std::int32_t nanos) : seconds(seconds), nanos(nanos) {
    if (seconds < minimum_second || seconds > maximum_second || nanos < 0 || nanos >= 1000000000) {
        throw std::invalid_argument("Instant is out of range");
    }
}

Instant parse_instant(std::string_view value) {
    std::size_t position = 0;
    const bool signed_year = !value.empty() && (value.front() == '+' || value.front() == '-');
    const bool negative_year = !value.empty() && value.front() == '-';
    if (signed_year) ++position;
    const auto year_start = position;
    std::int64_t year = 0;
    while (position < value.size() && value[position] >= '0' && value[position] <= '9') {
        year = year * 10 + value[position++] - '0';
        if (year > 1000000000) invalid_instant();
    }
    const auto year_length = position - year_start;
    if (year_length < 4 || (!signed_year && year_length != 4) ||
        (signed_year && !negative_year && year_length <= 4) || (negative_year && year == 0)) invalid_instant();
    if (negative_year) year = -year;
    separator(value, position, '-');
    const int month = digits(value, position, 2);
    separator(value, position, '-');
    const int day = digits(value, position, 2);
    if (position >= value.size() || (value[position] != 'T' && value[position] != 't')) invalid_instant();
    ++position;
    const int hour = digits(value, position, 2);
    separator(value, position, ':');
    const int minute = digits(value, position, 2);
    separator(value, position, ':');
    int second = digits(value, position, 2);
    std::int32_t nanos = 0;
    if (position < value.size() && value[position] == '.') {
        ++position;
        const auto start = position;
        while (position < value.size() && value[position] >= '0' && value[position] <= '9') {
            if (position - start == 9) invalid_instant();
            nanos = nanos * 10 + value[position++] - '0';
        }
        if (position == start) invalid_instant();
        for (auto length = position - start; length < 9; ++length) nanos *= 10;
    }
    int offset = 0;
    if (position < value.size() && (value[position] == 'Z' || value[position] == 'z')) {
        ++position;
    } else {
        if (position >= value.size() || (value[position] != '+' && value[position] != '-')) invalid_instant();
        const int sign = value[position++] == '-' ? -1 : 1;
        const int offset_hour = digits(value, position, 2);
        separator(value, position, ':');
        const int offset_minute = digits(value, position, 2);
        int offset_second = 0;
        if (position < value.size() && value[position] == ':') {
            ++position;
            offset_second = digits(value, position, 2);
        }
        if (offset_hour > 18 || offset_minute > 59 || offset_second > 59 ||
            (offset_hour == 18 && (offset_minute != 0 || offset_second != 0))) invalid_instant();
        offset = sign * (offset_hour * 3600 + offset_minute * 60 + offset_second);
    }
    constexpr int month_lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (position != value.size() || month < 1 || month > 12 || day < 1 ||
        day > month_lengths[month - 1] + (month == 2 && leap_year(year)) || minute > 59 ||
        hour > 24 || second > 60) invalid_instant();
    if (second == 60) {
        if (hour != 23 || minute != 59) invalid_instant();
        second = 59;
    }
    if (hour == 24 && (minute != 0 || second != 0 || nanos != 0)) invalid_instant();
    return Instant(days_from_civil(year, month, day) * 86400 + hour * 3600 + minute * 60 + second - offset, nanos);
}

std::string format_instant(const Instant& value) {
    const Instant checked(value.seconds, value.nanos);
    auto days = checked.seconds / 86400;
    auto seconds_of_day = checked.seconds % 86400;
    if (seconds_of_day < 0) {
        --days;
        seconds_of_day += 86400;
    }
    days += 719468;
    const auto era = (days >= 0 ? days : days - 146096) / 146097;
    const auto day_of_era = static_cast<unsigned>(days - era * 146097);
    const auto year_of_era = (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146096) / 365;
    auto year = static_cast<std::int64_t>(year_of_era) + era * 400;
    const auto day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    const auto month_part = (5 * day_of_year + 2) / 153;
    const auto day = day_of_year - (153 * month_part + 2) / 5 + 1;
    const auto month = month_part < 10 ? month_part + 3 : month_part - 9;
    year += month <= 2;
    std::ostringstream out;
    out << std::setfill('0');
    if (year < 0) out << '-' << std::setw(4) << -year;
    else if (year > 9999) out << '+' << year;
    else out << std::setw(4) << year;
    out << '-' << std::setw(2) << month << '-' << std::setw(2) << day << 'T'
        << std::setw(2) << seconds_of_day / 3600 << ':' << std::setw(2) << seconds_of_day / 60 % 60
        << ':' << std::setw(2) << seconds_of_day % 60;
    if (checked.nanos != 0) {
        if (checked.nanos % 1000000 == 0) out << '.' << std::setw(3) << checked.nanos / 1000000;
        else if (checked.nanos % 1000 == 0) out << '.' << std::setw(6) << checked.nanos / 1000;
        else out << '.' << std::setw(9) << checked.nanos;
    }
    out << 'Z';
    return out.str();
}

Instant now_instant() {
    const auto duration = std::chrono::system_clock::now().time_since_epoch();
    const auto seconds = std::chrono::floor<std::chrono::seconds>(duration);
    return Instant(seconds.count(), static_cast<std::int32_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration - seconds).count()));
}

}
