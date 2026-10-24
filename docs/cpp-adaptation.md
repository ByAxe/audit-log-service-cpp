# C++ adaptation

The course keeps the HTTP routes, event vocabulary, PostgreSQL schema, seed data, append-only database guards and each branch's exercise state. The implementation tools change:

| Java source | C++ version |
| --- | --- |
| Java 21 records, enums and repository interfaces | C++20 value types, enum classes and repository ports |
| Spring MVC and validation annotations | `cpp-httplib` handlers, DTO parsing and explicit boundary validation |
| Jackson | `nlohmann/json` |
| Spring Data JPA / Hibernate | Parameterized PostgreSQL queries through `libpq` |
| Spring transaction annotations | Explicit PostgreSQL transactions; read-only query transactions |
| Flyway | Native migration runner with its own migration history |
| Gradle / JUnit / MockMvc / Testcontainers | CMake, CTest and a PostgreSQL-backed HTTP integration check |

`libpq` is the PostgreSQL client library, not an ORM replacement. SQL and transaction handling are visible in the adapters; no claim is made that Hibernate internals or its exact query scheduling have a one-to-one C++ equivalent.

The adapters open native connections for their operations rather than using Java's connection pool. Appending uses an explicit INSERT rather than Hibernate's assigned-identifier merge path. The read adapter retains Spring Data's partial-page count shortcut: a short first page, or a nonempty short later page, derives the total from offset and content size; full pages and empty later pages run COUNT under the same predicates. PostgreSQL timestamp storage retains its microsecond precision, and sequence allocation can still leave gaps.

The port preserves meaningful HTTP validation outcomes and the explicit invalid-enum message contract. Error bodies produced implicitly by Spring/Jackson for malformed requests are framework-specific: their field set, text and ordering are not a byte-for-byte C++ contract. JSON object key order, header order and timestamp formatting that represents the same instant are likewise not application-level differences.

Checks must exercise the C++ implementation. Old Java test reports only describe the Java source revision and must not be reused as proof that this port compiled, ran, or passed its database checks.

## Database startup

Use a dedicated new PostgreSQL database. The SQL event schema and seed are carried over, but the C++ migration ledger is independent of Java's Flyway ledger. Existing Java databases require deliberate adoption and verification before use; automatic cross-runtime database migration is outside this repository's startup path.

The sample database credentials are local training defaults. The service retains the source application's lack of an authentication/authorization layer; publishing this repository does not make the HTTP service a production deployment.
