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

## Response

```json
{
  "content": [],
  "page": 0,
  "size": 50,
  "totalElements": 0
}
```

`content` items include:

- `id`
- `recordedAt`
- `actor`
- `action`
- `resourceType`
- `resourceId`
- `outcome`
- `reason`
- `metadata`

## Sort Order

Results are sorted by `recordedAt` descending.

## Additive Filter Rule

Filters are additive and read-only; adding a filter must never change results when its parameter is absent.

When both `action` and `outcome` are present, they combine with `AND`.

Invalid enum values return `400 Bad Request` with a message naming the invalid parameter.
