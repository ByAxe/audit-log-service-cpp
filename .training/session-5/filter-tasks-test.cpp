#include "filter-tasks.hpp"
#include "tasks.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void expect(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::vector<std::string> ids(const std::vector<Task>& tasks) {
    std::vector<std::string> result;
    for (const auto& task : tasks) {
        result.push_back(task.id);
    }
    return result;
}

void nonemptyOwner() {
    const auto tasks = loadTasks();
    expect(ids(filterTasks(tasks, {.owner = "anna"})) == std::vector<std::string>{"T4", "T1", "T5"},
           "anna selection");
    expect(ids(filterTasks(tasks, {.owner = "boris"})) == std::vector<std::string>{"T2", "T3"},
           "boris selection");
}

void absentOwner() {
    const auto tasks = loadTasks();
    const std::vector<std::string> expected{"T4", "T2", "T1", "T5", "T3"};
    expect(ids(filterTasks(tasks, {})) == expected, "absent owner");
    expect(ids(filterTasks(tasks)) == expected, "default filters");
}

void emptyOwner() {
    expect(ids(filterTasks(loadTasks(), {.owner = ""})) ==
               std::vector<std::string>{"T4", "T2", "T1", "T5", "T3"},
           "empty owner");
}

void unknownOwner() {
    expect(filterTasks(loadTasks(), {.owner = "nobody"}).empty(), "unknown owner");
}

void exactOwner() {
    const auto tasks = loadTasks();
    expect(filterTasks(tasks, {.owner = "Anna"}).empty(), "owner case must match");
    expect(filterTasks(tasks, {.owner = " anna "}).empty(), "owner spaces must match");
}

void originalOrder() {
    const auto tasks = loadTasks();
    const auto selected = filterTasks(tasks, {.owner = "anna"});
    std::vector<std::ptrdiff_t> positions;
    for (const auto& task : selected) {
        const auto found = std::find(tasks.begin(), tasks.end(), task);
        expect(found != tasks.end(), "selected record must come from input");
        positions.push_back(found - tasks.begin());
    }
    expect(std::is_sorted(positions.begin(), positions.end()), "input order must be preserved");
    expect(ids(selected) == std::vector<std::string>{"T4", "T1", "T5"}, "ordered owner records");
}

void originalData() {
    const auto tasks = loadTasks();
    const auto untouched = loadTasks();
    const auto selected = filterTasks(tasks, {.owner = "anna"});
    expect(tasks == untouched, "input data must be preserved");
    expect(selected == std::vector<Task>{{"T4", "anna", "open"},
                                         {"T1", "anna", "closed"},
                                         {"T5", "anna", "open"}},
           "return complete unchanged records");
}

void emptyInput() {
    expect(filterTasks({}, {.owner = "anna"}).empty(), "empty input with owner");
    expect(filterTasks({}).empty(), "empty input with default filters");
}

int main() {
    int failures = 0;
    const auto run = [&failures](const char* name, void (*test)()) {
        try {
            test();
            std::cout << "PASS " << name << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "FAIL " << name << ": " << error.what() << '\n';
        }
    };
    run("nonempty owner selects that owner", nonemptyOwner);
    run("absent owner does not restrict selection", absentOwner);
    run("empty owner does not restrict selection", emptyOwner);
    run("unknown owner has no matches", unknownOwner);
    run("owner uses exact case and whitespace", exactOwner);
    run("original order is preserved", originalOrder);
    run("input and record contents are preserved", originalData);
    run("empty input gives empty output", emptyInput);
    std::cout << 8 - failures << "/8 owner baseline tests passed\n";
    return failures == 0 ? 0 : 1;
}
