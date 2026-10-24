#pragma once

#include <optional>
#include <string>
#include <vector>

struct Task {
    std::string id;
    std::string owner;
    std::string status;

    bool operator==(const Task&) const = default;
};

struct Filters {
    std::optional<std::string> owner;
    std::optional<std::string> status;
};

std::vector<Task> filterTasks(const std::vector<Task>& tasks, const Filters& filters = {});
