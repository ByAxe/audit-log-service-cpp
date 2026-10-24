#pragma once

#include <filesystem>
#include <string>

namespace audit::persistence {

void migrate(const std::string& conninfo, const std::filesystem::path& directory);

}  // namespace audit::persistence
