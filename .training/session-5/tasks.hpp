#pragma once

#include "filter-tasks.hpp"

#include <array>

inline const std::array<Task, 5> teachingTasks{{
    {"T4", "anna", "open"},
    {"T2", "boris", "open"},
    {"T1", "anna", "closed"},
    {"T5", "anna", "open"},
    {"T3", "boris", "closed"},
}};

inline std::vector<Task> loadTasks() {
    return {teachingTasks.begin(), teachingTasks.end()};
}
