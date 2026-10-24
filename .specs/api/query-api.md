# Query API Contract

## Endpoint

`GET /api/v1/audit-events`

## Filters

- `actor`: exact actor match.
- `from`: include events where `recordedAt >= from`.
- `to`: include events where `recordedAt < to`.
- `action`: exact action enum match.
- `outcome`: exact outcome enum match.
- `page`: zero-based page index, default `0`.
- `size`: page size, default `50`, maximum `200`.
- `snapshotId`: optional upper bound on the event identifier.

## Response

```json
{
  "content": [],
  "page": 0,
  "size": 50,
  "totalElements": 0
}
```

Content items include `id`, `recordedAt`, `actor`, `action`, `resourceType`, `resourceId`, `outcome`, `reason` and `metadata`.

## Snapshot Boundary

`snapshotId` is an optional input parameter. When present, the query restricts both content and `totalElements` to `id <= snapshotId` under the same filters. Repeat the same S, filters and size across pages to keep the traversal bounded while the journal grows.

Without `snapshotId`, the query is unbounded and reflects the journal as of each request. The response does not report a snapshot boundary; the client supplies one itself.

## Sort Order

Results are sorted by `recordedAt` descending, then `id` descending. The unique id gives equal timestamps a defined relative order.

## Additive Filter Rule

Filters are additive and read-only; adding one must preserve results when its parameter is absent. A blank actor is dropped. Supplied action and outcome combine with AND.

Invalid enum values return `400 Bad Request` with a message naming the invalid parameter. Existing page/size defaults, size cap and validation remain unchanged.
