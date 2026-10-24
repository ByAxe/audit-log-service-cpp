# Session 3 — from investigation to a verifiable change

Start from `session-3/start`, the prepared result of homework 2. It accepts an optional input `snapshotId`, bounds content and count by `id <= S` when given, and orders by `recordedAt DESC, id DESC`. The response does not yet report S; a request without it remains unbounded.

```sh
git clone --single-branch --branch session-3/start https://github.com/ByAxe/audit-log-service-cpp.git
cd audit-log-service-cpp
git rev-parse HEAD
git status --short
```

Record the C++ starting SHA; the Java revision in [source provenance](../../docs/source-provenance.md) identifies the source snapshot, not this C++ checkout. Read [AGENTS.md](../../AGENTS.md), the [current query contract](../../.specs/api/query-api.md) and the [investigation handover](../../.specs/investigations/pagination-window-shift.md). Run the root build and check commands; supplied historical evidence is not your own run.

## The next contract

Before editing, write six concise contract statements in `.specs/changes/pagination-snapshot.md`:

1. A request without `snapshotId` fixes an upper boundary S once and already applies it to its own content and count.
2. The response returns S as `snapshotId`; later requests repeat it and receive the same S back.
3. A valid event appended after the first GET completes receives an id above S and stays outside that traversal.
4. Repeating the filters, size and S advances through the original set without duplicates or gaps; `totalElements` remains constant.
5. Existing filters, validation, page/size behavior and `recordedAt DESC, id DESC` order remain unchanged.
6. State the owner's decision for global versus filtered maximum, an empty journal and a filtered result with no matches. Do not silently infer these decisions from an example.

For the illustrative first page `[12, 11, 10]`, the upper boundary is 12. The last row's id 10 is not that boundary. A first request can start at any page; S is not derived from a row's page position.

## Work sequence

Agree the contract and the smallest expected file list with the task owner. Add a meaningful failing HTTP/PostgreSQL check, inspect why it fails, make the minimal change, and run the relevant checks. Keep a requirement-to-evidence table with PASS, FAIL or UNKNOWN. Do not weaken a check to obtain a green result.

The new checks must establish that the first request returns S, that a later append lands above it, that subsequent pages repeat S, that originals are complete and unique, and that the total remains constant. Exercise a filter as well: for example, read `actor=A,page=0,size=1`, append another A after the GET completes, then continue at the same S. If your primary scenario already uses actor, use outcome for an additional distinguishing probe.

Run the integration command from the root README and record the named scenario's result; it exercises the actual binary and PostgreSQL. Unit checks can prove service boundary resolution, but a stub cannot prove the SQL restriction. There is no new opaque token, cursor, migration or write-path task here.

Independent acceptance concerns an exact source revision. Give a peer the contract, changed files and actual output without the chat; the human reviewer decides whether the evidence supports acceptance. [Homework 3](homework.md) specifies both required reports.
