# Audit Log Service — C++

C++ training version of the [Java Audit Log Service](https://github.com/ByAxe/audit-log-service) for the Wargaming Blitz cohort. It records append-only audit events in PostgreSQL and exposes the same two HTTP endpoints used by the Java exercises.

`main` is the starting implementation for sessions 1 and 2. Later session branches contain the preceding exercise's handover; changes from them must not be brought into an earlier starting point. See [session branches and source provenance](docs/source-provenance.md).

## Build and check

You need a C++20 compiler, CMake 3.24 or newer, Python 3 and PostgreSQL client development files (`libpq`). The integration runner also needs PostgreSQL server binaries available through `pg_config` (or the `PG_CONFIG` setting); it starts its own temporary database. CMake obtains the pinned header libraries for HTTP and JSON during configuration.

```sh
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
tests/run-postgres.sh build/audit-log-service
```

CTest runs the unit and architecture checks. The separate integration runner starts the actual HTTP binary against PostgreSQL and removes its temporary database afterward. You can supply `AUDIT_LOG_TEST_DB_URL` for an empty throwaway database instead; the runner rejects existing user tables and leaves that database's test data in place.

A successful build alone does not establish HTTP or database behavior. Run the commands yourself and record their actual output; the repository's Java references describe historical Java runs.

## Run

Docker Compose builds the application and starts its dedicated PostgreSQL database:

```sh
docker compose up --build
```

The HTTP service listens on port `8080`:

```sh
curl 'http://localhost:8080/api/v1/audit-events?page=0&size=3'
```

Start with a new database for this C++ version. Its migration history is separate from Java Flyway history; pointing it at a Java-managed database requires an explicit adoption procedure. The bundled database settings and seed events are for local exercises.

For a native process, `build/audit-log-service` reads these environment settings:

| Variable | Meaning | Default |
| --- | --- | --- |
| `AUDIT_LOG_DB_URL` | PostgreSQL `libpq` connection string | Local `audit_log` database, user/password `audit` |
| `AUDIT_LOG_MIGRATIONS` | Migration directory | `db/migration` |
| `AUDIT_LOG_HOST` | HTTP bind address | `127.0.0.1` |
| `AUDIT_LOG_PORT` | HTTP port | `8080` |

## Service contract

- `POST /api/v1/audit-events` appends an event. The server supplies `id` and `recordedAt`; `actor`, `action` and `outcome` are required.
- `GET /api/v1/audit-events` reads events with optional filters and page-based pagination. Its current contract is in [`.specs/api/query-api.md`](.specs/api/query-api.md).
- There are no event update or delete endpoints. PostgreSQL triggers reject updates and deletes even outside the HTTP API.

The domain, append use case, query use case, HTTP API and PostgreSQL adapters stay separate. Public headers live under `include/audit/`, with implementation under `src/`; database migrations live in `db/migration/`. The C++ stack uses `cpp-httplib`, `nlohmann/json`, `libpq` and CMake; see [porting notes](docs/cpp-adaptation.md) for the limits of equivalence with Spring/JPA.

This is the course implementation, including its current limitations and unfinished exercises. It has no production authentication or authorization layer. Use a local training environment.

## Sessions

| Session | Starting branch | Entry material |
| --- | --- | --- |
| 1 | `session-1/start` | [Repository orientation](.training/session-1/README.md) |
| 2 | `session-2/start` (alias: `session-2/pagination-investigation`) | [Investigation entry](.training/session-2/README.md) |
| 3 | `session-3/start` | Materials on that branch |
| 4 | `session-4/start` | Materials on that branch |
| 5 | `session-5/start` | Materials on that branch |

Start from the branch named for your session. The root [AGENTS.md](AGENTS.md) and the session's own instructions define the boundaries for work with an agent.
