# Pagination window shift — handover

This is the prepared investigation inherited from the public Java session-3 source snapshot. It describes the mechanism and accepted input-boundary change; it is not a record of a C++ run performed by the reader. See [source provenance](../../docs/source-provenance.md).

## Symptom and boundary

Adjacent requests in a traversal can return the same identifier after an event is appended between them. No stored audit event is removed. The append-only database guards still reject UPDATE and DELETE; all previously read events remain retrievable.

The read adapter addresses each page by OFFSET/LIMIT over a fresh result set. An event added ahead of the current offset moves that window; the same numeric offset then points at a row the previous page already returned. The source's seeded rows have distinct timestamps, so equal-time ordering is not the cause of that trace.

## Distinguishing evidence

The characterization checks compare a no-append control with a GET-page-0 → POST → GET-page-1 sequence. The control expects a constant total and disjoint page contents; the latter expects total +1 and the observed intersection. Expectations use actual identifiers and count deltas instead of only fixed seed literals. A later read establishes continued retrievability.

The absence of a tie-breaker was a separate adjacent risk: equal timestamps had no defined relative order. It did not explain the distinct-timestamp incident.

## Accepted homework-2 result on this branch

- Keep `page` and `size`.
- Accept optional input `snapshotId`; when supplied, apply `id <= S` to both content and count.
- Use `recordedAt DESC, id DESC` to give equal timestamps a total order.
- Preserve the previous unbounded behavior when S is absent.
- Preserve filters, validation, append-only guards and the separate read path.

Corresponding C++ checks live in `tests/integration.py`, including `PaginationWindowShiftTests` for the control, moving-window characterization, explicit-boundary traversal and equal-timestamp order. Run `tests/run-postgres.sh build/audit-log-service` after the root build/check commands and inspect its actual results; no Java test count or historical PASS certifies this C++ checkout.

## Handover to session 3

An explicit boundary makes a bounded traversal possible, but the response does not tell a client which S it is reading at. A first request without S remains unbounded. The next task defines how the response reports an effective upper boundary and how the first request applies it. Global versus filtered boundary and empty-result behavior require explicit owner decisions.

An identifier ceiling is not an MVCC snapshot. The educational scenario appends after the first GET has completed; arbitrary overlapping transaction commits are a separate limitation.
