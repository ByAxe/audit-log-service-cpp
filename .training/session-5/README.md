# Session 5 · task filtering · team practice

A standalone exercise with one working copy and one result per team.

**Owner of the teaching task:** Aleksei Litvinau.

## Working together

One participant uses their usual agent and shares the screen. The others discuss
the plan and check the result. Agree a short plan within the scope below:

1. Read this task and the nearby [AGENTS.md](AGENTS.md).
2. Agree the plan within your team.
3. Make the change.
4. Run the checks and record the result in [task.md](task.md).

The complete task is supplied upfront. Your team can continue ordinary work
without intermediate signals or permissions from the facilitator. Separate agent
roles, multiple chats, computer-to-computer handoff, and a formal commit/export
procedure are unnecessary.

## Requirements

Use a C++20 compiler. There are no third-party dependencies, downloads, servers,
or databases. CMake 3.16 or newer is optional.

Homework 3/4 and previous local branches are not prerequisites. This exercise is
independent of the Audit Log application. Its `owner` and `status` fields belong
to this task list, not the application's Query API. The application, its tests,
`.specs/`, and the materials for sessions 1–4 are outside this exercise.

## Start

Clone a new team working copy of `session-5/start`. Push access and SSH setup are
not required.

```bash
git clone --single-branch --branch session-5/start \
  https://github.com/ByAxe/audit-log-service-cpp.git ~/audit-cpp-session-5
cd ~/audit-cpp-session-5
git rev-parse HEAD
cd .training/session-5
c++ -std=c++20 filter-tasks.cpp filter-tasks-test.cpp -o /tmp/audit-cpp-session-5-test
/tmp/audit-cpp-session-5-test
```

Record your actual starting SHA in `task.md`. The two compile/run commands above
are the exercise check. Recompile after changing source files.

Alternatively, from the exercise folder:

```bash
cmake -S . -B /tmp/audit-cpp-session-5-build
cmake --build /tmp/audit-cpp-session-5-build
ctest --test-dir /tmp/audit-cpp-session-5-build --output-on-failure
```

## Files

All files below are in `.training/session-5/`:

| File | Purpose |
| --- | --- |
| `README.md` | Contract, expected results, and check commands |
| `AGENTS.md` | Agent scope for this exercise |
| `tasks.hpp` | Fixed teaching data and `loadTasks()` |
| `filter-tasks.hpp` | Task/filter types and the function declaration |
| `filter-tasks.cpp` | The selection function |
| `filter-tasks-test.cpp` | Starter checks of preserved behavior |
| `CMakeLists.txt` | Optional standalone build |
| `task.md` | Your team's task card |

## Input

The fixed fixture in `tasks.hpp` has this order:

| id | owner | status |
| --- | --- | --- |
| T4 | anna | open |
| T2 | boris | open |
| T1 | anna | closed |
| T5 | anna | open |
| T3 | boris | closed |

The C++ adaptation stores this tiny fixture directly in `tasks.hpp`, replacing
the original exercise's `tasks.json` and JavaScript loader. `loadTasks()` returns
a fresh list with the same records and order. Compiling the fixture avoids a
JSON parser dependency and makes the executable independent of its working
directory. The selection function only uses the list passed by its caller.

## Interface

```cpp
std::vector<Task> filterTasks(const std::vector<Task>& tasks,
                              const Filters& filters = {});
```

`Task` contains string fields `id`, `owner`, and `status`. `Filters` contains
optional strings `owner` and `status`. An absent filter is `std::nullopt` or an
omitted aggregate field. The result contains complete records, rather than only
their IDs. The ID lists below describe the observable membership and order.

Inputs are valid task records and optional string filters. This is not an input
validation exercise; do not invent policies for other input types.

## Starting behavior

The owner filter already works:

- An absent or empty `owner` leaves the selection unrestricted.
- A nonempty `owner` uses exact equality.
- Input order, the input list, and record contents are preserved.

The data and filter type contain `status`, but the starter implementation does
not read or use it.

## Change to implement

Add the optional `status` filter to the existing `owner` filter:

- Nonempty filters combine with **AND**.
- An absent or empty filter leaves selection unrestricted.
- String equality includes case and whitespace.
- An unknown nonempty value has no matches.
- Preserve the original order, input list, and record contents.

The empty-status decision is already part of the contract: an empty string
does not restrict selection. Do not add sorting, trimming, case conversion,
a UI, caching, or a general filter language.

## Expected results

These expectations come from the input and agreed contract. They describe the
completed task, not claims that the starter already implements status.

| Filters (C++ aggregate fields) | Expected IDs in order |
| --- | --- |
| `{}` | `[T4, T2, T1, T5, T3]` |
| `{.owner = "anna"}` | `[T4, T1, T5]` |
| `{.owner = "boris"}` | `[T2, T3]` |
| `{.owner = ""}` | `[T4, T2, T1, T5, T3]` |
| `{.status = "open"}` | `[T4, T2, T5]` |
| `{.status = "closed"}` | `[T1, T3]` |
| `{.owner = "anna", .status = "open"}` | `[T4, T5]` |
| `{.owner = "boris", .status = "closed"}` | `[T3]` |
| `{.owner = "anna", .status = ""}` | `[T4, T1, T5]` |
| `{.owner = "", .status = "open"}` | `[T4, T2, T5]` |
| `{.status = ""}` | `[T4, T2, T1, T5, T3]` |
| `{.owner = "nobody"}` | `[]` |
| `{.status = "missing"}` | `[]` |
| `{.owner = "Anna"}` | `[]` |
| `{.owner = " anna "}` | `[]` |
| `{.status = " open "}` | `[]` |

## What green starter tests prove

The starter contains eight owner-baseline tests and no status acceptance tests.
Passing them proves the preserved owner behavior only. Add the relevant status
checks yourself, using expected IDs from the fixture and contract rather than
the function's current output. Check complete records and order, not only a
result count. Keep every existing check.

## Allowed changes

Change only:

- `filter-tasks.cpp`
- `filter-tasks-test.cpp`
- `task.md`

The fixture, headers, README, AGENTS, build configuration, application, migrations,
CI, root instructions, `.specs/`, and sessions 1–4 are outside the allowed scope.
Do not investigate or repair unrelated files. Do not disable checks or report
PASS without executing them.

## Record the result

A commit is optional. A changed working copy, actual check output, and completed
task card are sufficient. Describe an uncommitted working copy honestly. If you
change the code, compile and run again; earlier output becomes historical.

If your team makes a commit, compare against your actual recorded starting SHA:

```bash
git diff <YOUR_START_SHA> -- .training/session-5
```

Stop and ask the task owner if you need a new behavior decision or a file outside
the allowed three. If the check cannot run, record the specific command and
blocker in `task.md`.
