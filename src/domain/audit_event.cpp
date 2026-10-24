#include "audit/domain/audit_event.hpp"
#include <stdexcept>
#include <utility>

namespace audit::domain {

bool is_blank(std::string_view value) {
    for (std::size_t index = 0; index < value.size();) {
        const auto first = static_cast<unsigned char>(value[index++]);
        std::uint32_t codepoint = first;
        if (first >= 0x80) {
            const int remaining = first >= 0xC2 && first <= 0xDF ? 1 :
                                  first >= 0xE0 && first <= 0xEF ? 2 :
                                  first >= 0xF0 && first <= 0xF4 ? 3 : -1;
            if (remaining < 0 || index + remaining > value.size()) return false;
            codepoint &= (1U << (6 - remaining)) - 1;
            for (int part = 0; part < remaining; ++part) {
                const auto byte = static_cast<unsigned char>(value[index++]);
                if ((byte & 0xC0) != 0x80) return false;
                codepoint = (codepoint << 6) | (byte & 0x3F);
            }
            if ((remaining == 1 && codepoint < 0x80) ||
                (remaining == 2 && codepoint < 0x800) ||
                (remaining == 3 && codepoint < 0x10000) ||
                codepoint > 0x10FFFF || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) return false;
        }
        // Java Character.isWhitespace deliberately excludes non-breaking spaces.
        if (!((codepoint >= 0x09 && codepoint <= 0x0D) ||
              (codepoint >= 0x1C && codepoint <= 0x20) || codepoint == 0x1680 ||
              (codepoint >= 0x2000 && codepoint <= 0x2006) ||
              (codepoint >= 0x2008 && codepoint <= 0x200A) ||
              codepoint == 0x2028 || codepoint == 0x2029 || codepoint == 0x205F || codepoint == 0x3000)) {
            return false;
        }
    }
    return true;
}

AuditEvent::AuditEvent(std::int64_t id, Instant recorded_at, std::string actor, Action action,
                       std::optional<std::string> resource_type, std::optional<std::string> resource_id,
                       Outcome outcome, std::optional<std::string> reason,
                       std::map<std::string, std::string> metadata)
    : id(id), recorded_at(recorded_at), actor(std::move(actor)), action(action),
      resource_type(std::move(resource_type)), resource_id(std::move(resource_id)), outcome(outcome),
      reason(std::move(reason)), metadata(std::move(metadata)) {
    if (is_blank(this->actor)) throw std::invalid_argument("actor must not be blank");
    to_string(action);
    to_string(outcome);
}

}
