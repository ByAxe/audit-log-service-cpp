# Status filter · team task card

Owner of the teaching task: Aleksei Litvinau.

The team fills these fields. Leave results blank until they are observed.

## Task

Add `status` to the existing `owner` filter. Nonempty filters combine with AND;
an absent or empty filter does not restrict selection.

Contract and expected results: [README.md](README.md). Fixed input:
[tasks.hpp](tasks.hpp). Preserve owner behavior, input order, the input list,
and complete records. Do not add sorting, trimming, case conversion, a UI,
caching, or a general filter language.

## Allowed files

- `.training/session-5/filter-tasks.cpp`
- `.training/session-5/filter-tasks-test.cpp`
- `.training/session-5/task.md`

## Short agreed plan

1.
2.
3.

## Code version

- Team starting version (`git rev-parse HEAD` before edits):
- State checked: working copy / commit `<SHA>`:

An uncommitted working copy is not a checked HEAD. After code changes, compile
and rerun the relevant check; earlier output becomes historical.

## Check

From this exercise folder:

```bash
c++ -std=c++20 filter-tasks.cpp filter-tasks-test.cpp -o /tmp/audit-cpp-session-5-test
/tmp/audit-cpp-session-5-test
```

- Actual result or specific blocker:

```text

```

## Result

- **Decision:**
- **Evidence:**
- **Next action:**

## What we will reuse on our next task

