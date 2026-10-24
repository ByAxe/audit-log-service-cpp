# Session 4 — specification as portable context

Start from `session-4/start`. It contains a prepared homework-3 implementation: every query resolves one S, applies it to content and count, and returns it as `snapshotId`. Repeating S continues the bounded traversal. You can use this starting point even if your own homework is unfinished; the prepared implementation is not your personal homework submission.

```sh
git clone --single-branch --branch session-4/start https://github.com/ByAxe/audit-log-service-cpp.git
cd audit-log-service-cpp
git rev-parse HEAD
git status --short
```

Record the C++ SHA and read [AGENTS.md](../../AGENTS.md), the [accepted change card](../../.specs/changes/pagination-snapshot.md) and [verification boundaries](baseline-verification.md). Java revisions in [source provenance](../../docs/source-provenance.md) are provenance, not this C++ checkout's identity or execution evidence.

## The documentation gap is deliberate

`.specs/api/query-api.md` retains the session-3 contract: input-only S, no reported boundary. This document is intentionally stale and is part of the exercise. Until it has been reconciled after independent acceptance, **`.specs/changes/pagination-snapshot.md` is the authoritative contract**.

Do not implement returned S again or re-add stable sorting: they already exist. Reconstruct the accepted behavior from the card, implementation and checks, then assemble a package another person or a fresh agent can use without this chat.

The package can be `.specs/changes/pagination-snapshot/{proposal.md,spec.md,tasks.md}` or one concise document with equivalent sections. Preserve the existing `.md` change card. Include the source and implementation revisions, actual decision sources, preserved behavior, scenarios, evidence boundaries and one permitted next step. Add a design document only for a real unresolved design choice.

## The one combined regression still to add

The prepared checks cover a filtered traversal at a reported S with distinct timestamps, and equal-timestamp order separately. They do not yet combine every condition below in one traversal:

1. Insert three initial events for actor A with one shared `recordedAt`, plus an event for B that the actor filter must exclude.
2. First GET uses `actor=A,page=0,size=1` without `snapshotId`; save its reported S and total.
3. After that GET completes, append a new A event whose id is above S.
4. Read the remaining pages with the same actor, size and S.
5. Assert every original A id appears exactly once in `recordedAt DESC,id DESC` order; B and the new A are absent; count and reported S remain unchanged on every page.

Read the nearest existing scenarios in `tests/integration.py` first. The gap is the shared timestamp within the filtered, bounded, growing traversal, not an absence of all filtered-boundary coverage. This behavior already works; a new characterization of it can pass immediately. Do not break working production code to manufacture a failing test.

## Check the package and hand it over

Record document quality in `spec-eval.md` with PASS/FAIL/WEAK, and implementation evidence in `implementation-review.md` with PASS/FAIL/UNKNOWN; equivalent sections in one concise file are also sufficient. Name the source of each decision; a missing result is UNKNOWN rather than an invented PASS. Run the root C++ build/check commands for execution claims and record the actual named scenarios.

Give a fresh chat or peer only the package and fixed C++ SHA, without oral explanation or earlier chat history. Check whether they recover the accepted contract, unresolved questions and permitted next step. After independent acceptance, update the current query contract while keeping the historical package. No OpenSpec installation or repository conversion is required.

[Homework 4](homework.md) applies this to a small authorized task. A checked package and a successful reconstruction/next-step plan are sufficient when no implementation remains necessary.
