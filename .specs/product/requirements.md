# Product Requirements

## Purpose

Audit Log Service records compliance-sensitive audit events for downstream business systems.

## Invariant

Audit events are append-only. The system can insert events, but it must never update or delete them.

The invariant is enforced twice:

- Application structure exposes append and query paths, but no mutation path.
- PostgreSQL rejects `UPDATE` and `DELETE` on `audit_events`.

## API

### POST /api/v1/audit-events

Creates an audit event.

Required request fields:

- `actor`
- `action`
- `outcome`

Optional request fields:

- `resourceType`
- `resourceId`
- `reason`
- `metadata`

The server generates:

- `id`
- `recordedAt`

### GET /api/v1/audit-events

Reads audit events without mutating state.

Filters:

- `actor`
- `from`
- `to`
- `action`
- `outcome`
- `page`
- `size`

## Vocabulary

Actions:

- `LOGIN`
- `LOGOUT`
- `EXPORT_DATA`
- `DELETE_RECORD`
- `ACCESS_PII`
- `GRANT_ROLE`
- `REVOKE_ROLE`

Outcomes:

- `ALLOWED`
- `DENIED`
