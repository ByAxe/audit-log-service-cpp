# Homework 3 — verifiable change and peer discussion

Submit two Markdown reports: `session-3-peer-review.md` for the course change and `session-3-verifiable-change.md` for one bounded work task. Add your first and last name to both filenames and upload them to the cohort's OneDrive folder before the next class on 20 November 2026; no exact time has been specified. Both parts are required; keep private workplace evidence local and sanitize shared reports. Include a link to a branch, test or other checkable result in each report. Do not submit the full agent chat.

## Part 1: close the course-change loop

Continue your session branch from the first unfinished step. If it is unavailable, start your own branch from `session-3/start` after preserving any existing work. Keep a link to the change you show your peer.

Open the prepared `.specs/changes/pagination-snapshot.md` card and fill its six fields. Keep the accepted plan, expected and actual changed files, results and unknowns in the same card. The selected contract is a global maximum id before filters, S=0 for an empty journal, and reuse and return of a supplied S including zero. A filter with no matches returns empty content and totalElements=0 while retaining the global S. Existing filters, validation, ordering and append-only behavior remain unchanged; arbitrary overlapping transactions are outside this exercise.

Ask the agent to write and run one combined HTTP/PostgreSQL test before changing service code. In an isolated scenario without parallel writes, create A, A, then B with the highest id and save all three ids. GET actor=A, page=0, size=1 without S must return B's id as snapshotId and totalElements=2. After that GET completes, POST a new A and GET actor=A, page=1, size=1 with the returned S. Both responses retain S and totalElements=2; together the pages contain exactly the two original A events without duplicates or the new A. Inspect why the test fails: missing behavior permits implementation; an environment or fixture error does not. Then allow the agreed implementation without weakening the test.

Use the root C++ build/check commands, `git diff --check`, `git diff --stat` and `git status --short`. Register a new integration test class in `main()` after the original `QueryTests`; preserve the checks that expect the initial 12 records. The runner has no individual-scenario selector: run `tests/run-postgres.sh build/audit-log-service` and find your named test in its output. PASS means a check ran and supported the requirement; FAIL means it ran and contradicted it; UNKNOWN means the check could not run or its evidence was insufficient. Record the actual command and error if execution is blocked, and continue the card, test and code review without claiming a successful run.

Discuss the work with a peer:

1. Show your card, change and check results. Explain what you achieved, how you reached it, and what remains unclear or unverified.
2. Your peer says what they understood and asks questions. Answer them and note the remark and your response.
3. Clarify your explanation or correct the work when needed, then swap roles. Discuss what you learned and will use next time.

In `session-3-peer-review.md`, record your name and peer, the behavior changed and card location, the result and how you reached it, actual commands/results, unverified items and reasons, your peer's questions and remark, your response and any changes, and what you understood or asked about their work. A separate peer runtime run, extra probe or formal acceptance verdict is not required for this discussion.

## Part 2: transfer the method to a work task

Choose a small current task within your authorized workplace scope, or one next step from your homework-1 map: for example, a test, reproduction scenario or check of a small fix. Agree the result and plan with the agent before allowing work. Changing a production service solely for this homework is not required. If no suitable work example exists, say so explicitly.

Carry out the agreed task with the agent, inspect the result and actual checks, and show it to a colleague. For an implementation task, start with a meaningful failing check and preserve its expectations. For a scenario or review task, record the evidence you actually obtained and its limits.

In `session-3-verifiable-change.md`, record the task, plan, actions, checks, colleague feedback, and what remains unfinished. A blocked check or missing owner decision is an honest outcome: state the command/error or question and the next action. Do not invent a successful run. Preserve existing tests and safety boundaries.

Separate agents for tests and implementation are optional; no additional role or infrastructure setup is required.
