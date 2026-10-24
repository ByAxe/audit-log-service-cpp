# Homework 3 — verifiable change and independent acceptance

Submit two Markdown reports: `session-3-peer-review.md` for the course change and `session-3-verifiable-change.md` for one bounded work task. Both parts are required; keep private workplace evidence local and sanitize shared reports.

## Part 1: close the course-change loop

Continue your session branch. If it is unavailable, start your own branch from `session-3/start` after preserving any existing work. Record the exact C++ SHA you hand over and any later replacement SHA.

Your handover contains the six-statement `.specs/changes/pagination-snapshot.md` contract, the accepted file plan, implementation and meaningful HTTP/PostgreSQL test, actual changed-file list, requirement/evidence table and actual check output. The contract must state whether S is global or filter-specific; a prepared example is not a substitute for that owner decision.

Use the root C++ build/check commands, `git diff --check` and `git diff --stat`. Record the relevant named integration scenario, even when the runner executes it as part of the full integration run. PASS means a check ran and supported the requirement; FAIL means it ran and contradicted it; UNKNOWN means the check could not run or its evidence was insufficient.

Exchange a branch or patch, exact SHA, contract, evidence table and output with an independent peer. Do not give them the agent chat.

The reviewer must:

1. Read the contract before the implementation, then inspect the actual diff.
2. Map at least two requirements to their code and executable evidence.
3. Run the relevant integration scenario independently and perform a distinguishing filtered probe.
4. Challenge at least one claimed PASS: explain what the check does and does not prove.
5. Give a human verdict: **ACCEPT**, **NEEDS CHANGE** or **NEEDS OWNER DECISION**, with reasons.

In `session-3-peer-review.md`, record the reviewed SHA, requirements examined, commands and results, the challenged PASS, the verdict and the author's response. If changes follow, identify their new SHA and the checks rerun. An agent can gather evidence; the human reviewer owns the acceptance verdict.

## Part 2: transfer the method to a work task

Choose a small current task within your authorized workplace scope, or one next step from your homework-1 map. Examples include one validation rule, a reproducible edge case, a small response/command/configuration correction or a local defect with a clear expected result. Use a fresh working branch and chat. Record desired and preserved behavior, boundaries, explicit decisions and a minimal file plan before allowing edits.

Run one meaningful executable check before implementation, make the focused change without weakening that check, and run the project's applicable targeted and full checks. In a fresh read-only review, map requirements to actual results, inspect the diff yourself and give the change to an engineer who can accept it.

Write this in `session-3-verifiable-change.md`. **DONE** means the change, checks and independent verdict exist. **BLOCKED** or **NEEDS DECISION** is an honest outcome when access, execution or an owner decision is missing: state the command/error or question, responsible person and next action. Do not invent a successful run. Preserve existing tests and safety boundaries.

Separate agents for tests and implementation are an optional advanced method, not a required extra role or infrastructure setup. The requirements are a clear contract, evidence and independent human judgment.
