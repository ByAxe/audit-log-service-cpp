# Homework 4 — hand over a task through files

Prepare one small task description and check whether a fresh agent can recover accepted decisions and propose the next step using only repository files. Continue completed work; do not repeat it merely for the exercise. About 45–60 minutes is a useful bound.

Use a familiar authorized work task, a next step from homework 3 or a recent change that needs a handover. If workplace access is unavailable, use the session-4 audit-log example and say why. Unfinished homework 3 does not prevent this work; mark unfinished actions honestly, and submit homework 3 separately.

## Package

Use `.specs/changes/<change-id>/` or one concise Markdown file with equivalent sections:

- `proposal.md`: purpose, scope, starting revision, implementation revision if any, decision sources and the named human who confirms them.
- `spec.md`: changed and preserved behavior, a GIVEN/WHEN/THEN scenario, explicit completed versus planned state and remaining questions.
- `tasks.md`: completed work, one next step, allowed files, check command and stop condition. If the task is complete, state that.

For the course example preserve `.specs/changes/pagination-snapshot.md`; the new directory is a separate portable package. Its accepted S rules are already set by that card. The next test step is the combined actor-filter, equal-timestamp, repeated-boundary traversal in the session README. The stale current API document is a known exercise gap, not a reason to rediscover the accepted contract from scratch.

## Check and reconstruct

Review the package against sources before handing it over. Record document findings as PASS/FAIL/WEAK. Implementation claims use PASS/FAIL/UNKNOWN with actual commands/results and the exact checked revision; supplied output remains supplied evidence.

Respond to review findings: fix them, reject them with a source-based reason, or stop for a missing decision. Independently check one finding, including a PASS, against its source. A report with no invented gaps is valid.

Save the package in a separate commit for the handover and record its SHA. Start a fresh chat without previous conversation context. Give only the file paths and that revision, ask for its understanding of the contract and a bounded next-step plan, then compare that reconstruction with the actual sources. The reader should report the actual SHA and stop if it differs or package files are dirty, without switching branches. Note any automatically supplied memory that limits the fresh-context experiment.

Correct real gaps in the package, save the new revision and repeat the handover when needed. Record the reconstruction and accepted next-step plan, or the evidence that the task is complete. Do not force new code if the task is already complete; implementation is optional when unnecessary.

Read the [OpenSpec README](https://github.com/Fission-AI/OpenSpec) for its separation of a change description, tasks and current specifications. Installing it, migrating this project or adding validation infrastructure is not required.

## Deliverable

Submit the checked package plus `session-4-handoff.md` containing:

1. Starting C++ revision, the package commit read by the fresh chat and the final package commit. Mark an unfinished working copy explicitly if you submit a partial result.
2. A five-line handover: task, accepted behavior, preserved boundaries, current evidence, next step or completion.
3. What the fresh chat reconstructed correctly, what was missing or ambiguous, and the source-based correction; an evidenced no-gap result is valid.
4. Actual checks, remaining unknowns and work still required.
5. The current API specification's owner and status, including whether independent acceptance and synchronization have occurred.

Do not copy private workplace sources into this public repository. An honest blocker or unresolved owner decision is a valid result; a prepared course implementation is not evidence that you completed your own work-task cycle.
