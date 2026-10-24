# Product Design

## Architecture

The service keeps five boundaries:

- Domain: framework-independent audit event types and repository ports.
- Append application: the write use case.
- Query application: the read use case.
- Persistence: PostgreSQL adapters using `libpq`.
- API: HTTP request/response DTOs and separate append/query handlers.

## Append Path

The HTTP append handler accepts a create request and calls the append use case.

The append use case validates required fields, generates the server timestamp and identifier, and writes through the append repository port.

## Query Path

The HTTP query handler accepts read-only filters and calls the query use case.

The query use case normalizes paging and delegates to the read repository port. It does not depend on append services or append persistence adapters.

## Database

The `audit_events` table stores immutable events:

- `id bigint primary key` from the `audit_events_id_seq` sequence
- `recorded_at timestamptz not null`
- `actor text not null`
- `action text not null`
- `resource_type text`
- `resource_id text`
- `outcome text not null`
- `reason text`
- `metadata jsonb`

Indexes exist for `recorded_at`, `actor`, `action`, and `outcome`.

`V2__append_only_guard.sql` adds triggers that raise `audit_events is append-only` before every update or delete. Native migration history belongs to this C++ application; adopting an existing Java-managed database is outside the training startup flow.
