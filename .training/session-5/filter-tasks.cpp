#include "filter-tasks.hpp"

std::vector<Task> filterTasks(const std::vector<Task>& tasks, const Filters& filters) {
    const bool ownerIsSet = filters.owner && !filters.owner->empty();
    std::vector<Task> selected;
    for (const auto& task : tasks) {
        if (!ownerIsSet || task.owner == *filters.owner) {
            selected.push_back(task);
        }
    }
    return selected;
}
