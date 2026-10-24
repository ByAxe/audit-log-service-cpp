#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace audit::persistence {

class DatabaseError : public std::runtime_error {
 public:
  DatabaseError(std::string message, std::string sqlstate)
      : std::runtime_error(std::move(message)), sqlstate_(std::move(sqlstate)) {}

  const std::string& sqlstate() const noexcept { return sqlstate_; }

 private:
  std::string sqlstate_;
};

}  // namespace audit::persistence
