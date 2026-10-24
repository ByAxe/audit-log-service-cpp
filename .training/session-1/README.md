# Session 1 — repository orientation

Start from `session-1/start`. `heat-session-1-baseline` is an alias for the same starting point. This is the C++ counterpart of the Java cohort's original audit-log baseline.

```sh
git clone --single-branch --branch session-1/start https://github.com/ByAxe/audit-log-service-cpp.git
cd audit-log-service-cpp
git rev-parse HEAD
git status --short
```

Record the starting revision. Read the root [README](../../README.md), [AGENTS.md](../../AGENTS.md) and the [specification index](../../.specs/README.md). Identify the domain, HTTP boundary, append path, query path, persistence and database guards before choosing files for a task.

Use the build and check commands from the root README. State which commands actually ran and what they prove. If a prerequisite is unavailable, record that limit rather than treating another person's output as your own result.

The instructor's demonstration investigates where a `from <= to` validation rule would belong and which product/error decisions it needs. The participant exercise investigates optional exact `resourceType` and `resourceId` query filters. Both are research tasks: do not implement them in this starting branch.

Follow the request from HTTP through application, domain and PostgreSQL adapter to the nearest tests. Record evidence for current behavior, invariants, unknowns, the smallest possible change surface and checks that would distinguish correct from incorrect behavior. Resolve questions such as independent filters versus a required pair, blank values, indexes and combination with existing filters before proposing code.

[Homework 1](homework.md) transfers the same method to a small real task. Repository orientation does not authorize implementing later-session work or changing the append-only invariant.
