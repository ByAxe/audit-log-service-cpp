# Change: report the snapshot boundary from the query API

This is the accepted implementation contract for the prepared C++ `session-4/start` example. It is inherited from the public Java session-4 source; [source provenance](../../docs/source-provenance.md) identifies the Java revisions. Verify the current C++ checkout and record its own SHA/results; this card contains no execution receipt or human-acceptance claim.

## Changed behavior

The prior contract accepted input `snapshotId` but did not report a boundary. A first request without S was unbounded and the client had to source one itself.

Now every query resolves exactly one S and reports it:

- A request without `snapshotId` determines S once and already applies it to its own content and count.
- The response carries `snapshotId=S`, so later pages can repeat it.
- A request with S uses and returns that value unchanged, including 0.

## Preserved behavior

- Existing actor/from/to/action/outcome filters, combinations and validation.
- Blank actor is still dropped; no trimming or case conversion of nonblank values.
- Default page 0 and size 50, size capped at 200.
- Invalid action/outcome return 400 naming the parameter.
- `recordedAt DESC,id DESC` order.
- Existing content/page/size/totalElements names and meanings.
- Append-only journal and read-only query path.

## Boundary rule

| Request | S |
| --- | --- |
| S absent, journal nonempty | Maximum id of the whole journal before user filters |
| S absent, journal empty | 0 |
| S present, including 0 | Exactly the supplied value |

These fallback choices come from the prepared Java session-4 example and are retained for this C++ starter. They are not decisions attributed to a prior homework participant or cohort. A separately confirmed conflicting owner decision must be resolved explicitly rather than silently overwritten.

Both content and `totalElements` use the same filters AND `id <= S`. Repeat the same filters, size and S on later pages: content advances while S and the bounded total remain constant. A filter matching nothing still returns global S with empty content and total 0; an empty later page echoes the supplied S. Never derive S from the last page row or its position in the sort order.

```json
{
  "content": [],
  "page": 0,
  "size": 50,
  "totalElements": 0,
  "snapshotId": 0
}
```

## Scope and limitations

The change adds no cursor, opaque token, endpoint, schema or migration change; it does not alter the write path or access policy. The read service consults the journal maximum once for a request without S; a request carrying S does not recompute it.

S is an identifier ceiling, not an MVCC snapshot or a guarantee for arbitrary overlapping commits. The verified course scenario creates an event after the first GET completes. A transaction that reserves id 100 but commits after another transaction's id 101 can become visible after a reader fixes S=101; that late row can then affect later pages. Closing this gap needs a different mechanism and is outside this example.

## Checks and exercise gap

`tests/unit.cpp` covers service resolution, empty journal, explicit S and explicit 0. Its repository is a stub and proves which boundary the search receives.

`tests/integration.py` includes `SnapshotBoundaryContractTests`: reported first boundary, later append excluded, filtered traversal, global boundary, empty result/page, and explicit 0. `PaginationWindowShiftTests` retains explicit input S and standalone equal-time ordering. These checks use the real HTTP binary and PostgreSQL; run the root build/check commands and inspect actual results.

The combined actor filter + three equal timestamps + reused S + post-GET append regression is intentionally absent in this starter. The session-4 README defines that next test; do not describe the entire filtered-boundary traversal as untested.

## Document authority and stop conditions

`.specs/api/query-api.md` deliberately retains the preceding response contract for the session-4 specification exercise. This card is authoritative until the current API document is reconciled after independent acceptance. Preserve this historical card and the participant's portable package.

For this read-side exercise, stop and seek the task owner's decision if delivering the contract seems to require schema, trigger, write-path, dependency or access-policy changes; weakening a test; renaming fields; changing filter semantics; or resolving a real conflict in accepted behavior. Existing implementation and checks do not substitute for a human acceptance decision on the participant's exact revision.
