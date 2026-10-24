# C++ verification

Checked on 2026-10-24. These are C++ results; Java revisions in the provenance table identify the source being adapted.

## Sessions 1 and 2

Application revision: `ee5c31f` (the following documentation/CI commit does not change the application).

Native environment: macOS, AppleClang 21, PostgreSQL 14.24. The repository's CI separately checks Linux with PostgreSQL 16.

| Check | Actual result |
| --- | --- |
| CMake Debug build | Passed |
| CTest unit and architecture targets | 2/2 passed; architecture target contains three checks |
| `tests/run-postgres.sh build/audit-log-service` | 25 HTTP/PostgreSQL tests passed |
| `python3 tests/migrations.py build/audit-log-service` | Passed: restart preserves rows/history/sequence; changed migration and schema without history refused |

The native checks use a temporary database and actual HTTP process. They include direct PostgreSQL UPDATE/DELETE rejection, enum/media/date coercions and extended-year timestamp round trips. PostgreSQL 16 results are available in [GitHub Actions](https://github.com/ByAxe/audit-log-service-cpp/actions).

The original Java full suite and Docker image were not rerun for this report. Selected live comparisons used the existing baseline Java build; they are supplementary evidence rather than a fresh Java source build. Framework implementation differences are recorded in [C++ adaptation](cpp-adaptation.md).

## Session 3

Application revision: `ed582dd`. [Linux / PostgreSQL 16 run](https://github.com/ByAxe/audit-log-service-cpp/actions/runs/40081357266): build passed, CTest 2/2, HTTP/PostgreSQL 29 tests, migration lifecycle passed. The added four pagination scenarios establish the inherited homework-2 boundary and ordering; the response boundary remains the session-3 assignment.
