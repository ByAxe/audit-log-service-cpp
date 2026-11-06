# Session 2 — investigation

Start from `session-2/start` or its alias `session-2/pagination-investigation`, which mirrors the Java branch name. Tag `session-2-baseline` preserves the original baseline; the application is unchanged from the session-1 starting point.

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

## Reproduce after evidence B is released

The bug is already present in the starting code. Two GET requests without an intervening append are the control scenario and should not overlap. The incident requires a new event between page 0 and page 1.

After the trainer releases evidence B, run the complete scenario in a fresh temporary PostgreSQL database:

Use the latest starting branch (`session-2/start` or `session-2/pagination-investigation`). Preserve your own work before updating; if you have already committed your investigation, use a separate clone for this demonstration.

```sh
git fetch origin
git pull --ff-only
cmake -S . -B build
cmake --build build -j2
python3 .training/session-2/reproduce.py build/audit-log-service
```

The script starts and stops its own service and database, rejects an external database URL, and never sends requests to an existing server. It needs the same PostgreSQL tools as `tests/run-postgres.sh`.

The final line must contain `SESSION_2_BUG_REPRODUCED`: page 0 is `[12, 11, 10]`, then POST adds an event and page 1 becomes `[10, 9, 8]`. A scan limited to the original four pages misses ID 1, which remains readable on page 4. This is a reproduction of the training bug, not its fix. The complete integration suite can be green because its existing pagination checks do not append between those page requests.
