# Homework 2 — characterize, then make the agreed change

Open this assignment after the session's investigation and evidence reveal. Continue your own working branch from `session-2/pagination-investigation`; record its starting revision. The starter contains no completed investigation or homework implementation.

The original engineering assignment has two separate commits. The first establishes the behavior without changing production code. The second implements the bounded change under the contract below.

## Commit 1: executable characterization

Add integration checks through the public HTTP API against real PostgreSQL:

- Read page 0, append one valid event through POST, then read page 1 with the same size. Show that the total grows by one and that the two responses intersect as observed.
- As a control, read adjacent pages with no append between them. Show an unchanged total and disjoint page contents.

Derive expectations from identifiers and counts obtained during the scenario: intersections and count deltas carry the claim. Hardcoded seed IDs alone are insufficient. Also establish that the previously read events remain retrievable; a repeated response item does not by itself prove a stored event was lost.

Keep `src/`, `include/` and migrations unchanged for this commit. Extend the existing integration checks rather than adding a new test framework. Complete `.specs/investigations/pagination-window-shift.md` with the symptom boundary, competing hypotheses and probes, actual evidence, confirmed mechanism, separate adjacent risks and the owner question. Record test commands and their actual result, then commit this characterization separately.

## Commit 2: the bounded API change

The agreed change preserves page/size pagination and all existing filters:

- Add optional query input `snapshotId` as a signed 64-bit integer.
- When supplied, apply `id <= snapshotId` to both page content and `totalElements`.
- When absent, leave the request unbounded as before.
- Sort by `recordedAt DESC, id DESC`, using the unique identifier to order equal timestamps.
- A traversal repeating one S must exclude events appended beyond S and retain the same bounded total.
- The response still has only `content`, `page`, `size` and `totalElements`; reporting an effective boundary is a later task.

Add meaningful integration checks for the bounded traversal and equal-timestamp order, update the current query contract, and make the smallest read-side change. Preserve the existing validation behavior, blank-actor semantics, append-only triggers and separation of query from append. Do not introduce keyset pagination, caching or a new endpoint.

## Check and submit

Use the root build and test commands:

```sh
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
tests/run-postgres.sh build/audit-log-service
git diff --check
```

Record the two commit SHAs, the actual test output and links to your investigation and changed contract. If a check cannot run, give its command and concrete blocker; do not claim PASS from a build or another person's output. Do not weaken existing tests or modify the write path to complete the read-side task.
