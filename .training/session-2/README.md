# Session 2 — investigation

Start from `session-2/start` or its alias `session-2/pagination-investigation`, which mirrors the Java branch name. Tag `session-2-baseline` identifies the same baseline; the application is unchanged from the session-1 starting point.

```sh
git clone --single-branch --branch session-2/pagination-investigation https://github.com/ByAxe/audit-log-service-cpp.git
cd audit-log-service-cpp
git rev-parse HEAD
git status --short
```

Read the root [AGENTS.md](../../AGENTS.md), the current [Query API contract](../../.specs/api/query-api.md) and the evidence released during the session. Keep observations, hypotheses and product decisions separate.

The initial client report contains these two observations:

```text
GET /api/v1/audit-events?page=0&size=3   content IDs: 12, 11, 10
GET /api/v1/audit-events?page=1&size=3   content IDs: 10,  9,  8
```

Use only the evidence released so far. Do not create new events in your local copy before the trainer releases the corresponding evidence stage.

For each hypothesis, name a cheap probe and the observation that would distinguish it from another explanation. Record the request, response and database state you actually inspected. A plausible mechanism is not a confirmed cause until the evidence separates them.

Do not change service behavior while gathering evidence. Preserve the append-only invariant and use the supplied training dataset and sequence as directed. If the evidence exposes a missing product decision, record the question for the owner instead of silently choosing a new contract.

If you cannot run the service locally, use the trainer's supplied evidence and label it as supplied. There is no execution receipt in this entry page.

Use the [blank investigation template](investigation-template.md) to prepare `.specs/investigations/pagination-window-shift.md` in your working branch. Open [homework 2](homework.md) after the session's reveal; keep characterization and the agreed behavioral change in separate commits.
