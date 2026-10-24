# Verification boundaries for the prepared starter

The C++ branch ports the prepared Java session-4 source state. The public Java implementation originated at `028d9bbb0d07ca4194486f0a8a96ec06ebb77c14`; the Java session entry is `917fd1b8943aaa77db7c57eb58235b2b89039671`. These identify inherited source behavior only. They do not identify a C++ binary or certify a C++ run.

Check your own exact checkout:

```sh
git rev-parse HEAD
git status --short
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
tests/run-postgres.sh build/audit-log-service
```

The root README states runtime requirements and database isolation. Record the actual test names, executed counts and results from your run. A historical Java report, build success or zero-test run is not an execution receipt for the C++ implementation.

| Claim | C++ evidence to inspect | Limit |
| --- | --- | --- |
| Missing S resolves once and is passed to the search; empty journal resolves to 0 | Query-service checks in `tests/unit.cpp` | A recording repository proves service parameters, not SQL |
| Supplied S and explicit 0 are used unchanged without recomputation | Unit boundary checks and `SnapshotBoundaryContractTests` in `tests/integration.py` | Unit stubs alone do not establish database behavior |
| Content and count share S; append after first GET stays outside later pages | PostgreSQL/HTTP snapshot integration scenarios | Educational append occurs after the GET completes |
| S is global before user filters; no matches still report it | Snapshot boundary integration scenarios | Current starter choice inherited from prepared source, not a claim about cohort decisions |
| `recordedAt DESC,id DESC` and explicit input S remain supported | `PaginationWindowShiftTests` in `tests/integration.py` | The combined actor/tied-time/reused-S regression is the session task |
| Query remains separate from append and domain dependencies remain bounded | Architecture CTest and code inspection | Static boundaries complement runtime trigger checks |

There is no result prefilled here for your run: it remains UNKNOWN until you execute the checks and inspect output. Do not attribute the source Java cohort's test counts, acceptance or environment to the C++ port.

The ID ceiling is not a database snapshot: a transaction can reserve a lower id and commit after a reader has fixed a higher S. That previously invisible row can enter later pages. No full concurrent-transaction isolation guarantee follows from the sequential course scenarios. Offset-query performance, overlapping commits and production load require their own evidence.
