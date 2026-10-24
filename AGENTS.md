# Audit Log Service — C++

## Scope and source of truth

This repository is a C++ training port of the Java Audit Log Service. Preserve the behavior and exercise state of the branch you are working on. Do not pull later-session solutions into an earlier starting branch or improve neighboring behavior without an explicit task.

Read the relevant `.specs/` contract and the session's `.training/` material before editing. A session can intentionally leave a document or acceptance check unfinished; its entry material names the authoritative contract in that case.

## Invariant

**Audit events are append-only. Never add application UPDATE or DELETE behavior for audit events. Never weaken or remove the database triggers that reject UPDATE and DELETE. Query code must never touch the append/write path.**

## Layering

- `include/audit/domain/` contains framework-free types and ports; `src/` contains implementation.
- `include/audit/application/append/` and `include/audit/application/query/` contain separate use cases.
- `include/audit/api/append/` and `include/audit/api/query/` contain the HTTP boundary.
- `include/audit/persistence/` declares PostgreSQL adapters, implemented in `src/persistence/append/` and `src/persistence/query/`.
- `db/migration/` contains schema, append-only guards and seed migrations.
- Domain types and repository ports have no HTTP-server or PostgreSQL dependencies.
- HTTP API code uses request/response DTOs, validates input at the boundary, and depends on application use cases.
- Application use cases depend on domain ports.
- PostgreSQL persistence implements those ports.
- Append and query paths remain separate; query operations are read-only.
- Use enum types for `action` and `outcome`; do not silently accept new enum values.

## Checks

```sh
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
tests/run-postgres.sh build/audit-log-service
```

Choose checks for the changed behavior. Database and HTTP claims need the PostgreSQL integration check; a compile alone is not enough. Documentation-only edits need inspection of the affected references, not an application rebuild.

Do not delete, skip or weaken a failing test to make the change pass. If a test is wrong, correct it separately and explain why. Record checks you actually ran, including failures and unavailable prerequisites; do not present historical output as your own run.

Preserve unrelated changes. No commit, push, dependency upgrade, new abstraction or production deployment is implied by a local exercise. Never put credentials or private cohort correspondence into this public repository.
