# Session 3 — from investigation to a verifiable change

Start from `session-3/start`, the prepared result of homework 2. It accepts an optional input `snapshotId`, bounds content and count by `id <= S` when given, and orders by `recordedAt DESC, id DESC`. The response does not yet report S; a request without it remains unbounded.

```sh
git clone --single-branch --branch session-3/start https://github.com/ByAxe/audit-log-service-cpp.git
cd audit-log-service-cpp
git rev-parse HEAD
git status --short
```

Keep a link to your working branch. The Java revision in [source provenance](../../docs/source-provenance.md) identifies the source snapshot, not this C++ checkout. Read [AGENTS.md](../../AGENTS.md), the [current query contract](../../.specs/api/query-api.md) and the [investigation handover](../../.specs/investigations/pagination-window-shift.md). Run the root build and check commands; supplied historical evidence is not your own run.

## The next contract

Before editing, fill in the six fields in the prepared [change card](../../.specs/changes/pagination-snapshot.md). Use these requirements when agreeing the contract:

1. A request without `snapshotId` fixes an upper boundary S once and already applies it to its own content and count.
2. The response returns S as `snapshotId`; later requests repeat it and receive the same S back.
3. A valid event appended after the first GET completes receives an id above S and stays outside that traversal.
4. Repeating the filters, size and S advances through the original set without duplicates or gaps; `totalElements` remains constant.
5. Existing filters, validation, page/size behavior and `recordedAt DESC, id DESC` order remain unchanged.
6. Use the exercise's selected behavior: S is the global maximum id before user filters; an empty journal gives S=0. A supplied S, including zero, is used and returned without recalculation. A filter with no matches returns empty content and totalElements=0 while retaining the global S.

For the illustrative first page `[12, 11, 10]`, the upper boundary is 12. The last row's id 10 is not that boundary. A first request can start at any page; S is not derived from a row's page position.

## Work sequence

Agree the contract and the smallest expected file list with the task owner. Add a meaningful failing HTTP/PostgreSQL check, inspect why it fails, make the minimal change, and run the relevant checks. Keep a requirement-to-evidence table with PASS, FAIL or UNKNOWN. Do not weaken a check to obtain a green result.

Use one combined HTTP/PostgreSQL test in an isolated scenario without parallel writes. Create two events with a new actor A, then an event with a new actor B; save all three ids. Request `actor=A,page=0,size=1` without S: the response's S must equal B's id and totalElements must equal 2. After that GET completes, append another A, then request page 1 with actor A and the returned S. Both responses must retain S and totalElements=2; together the pages contain exactly the two original A events, without duplicates or the new A.

Run the integration command from the root README and record the named scenario's result; it exercises the actual binary and PostgreSQL. Unit checks can prove service boundary resolution, but a stub cannot prove the SQL restriction. There is no new opaque token, cursor, migration or write-path task here.

Show a peer the result, the decisions that led to it and your actual checks. Explain what remains unverified, answer the peer's questions, and record their remark and your response. Swap roles; allow six minutes for each person and finish an incomplete discussion in the homework. A separate peer test run or additional probe is not required for this discussion. [Homework 3](homework.md) specifies both required reports.
