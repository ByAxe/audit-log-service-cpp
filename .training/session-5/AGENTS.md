# Session 5 exercise boundaries

Read this file and README before working. These instructions describe scope;
they are not a technical restriction on tool permissions. Root security
requirements still apply.

This standalone exercise lives entirely in `.training/session-5/`. Use its local
context and C++20 check. Do not start the application, PostgreSQL, Docker, network
services, package installation, or the main project's build. Data is fixed in
`tasks.hpp`. The application's Query API does not gain owner/status fields.

Keep changes focused on adding `status` to `owner`, the corresponding checks,
and the task card. Preserve the fixed teaching data and agreed contract.
Do not refactor the Audit Log application or repair unrelated issues.
An unrelated discrepancy does not block this exercise.

Use one working copy and one result per team. The team agrees its plan and
ordinary permissions within this scope; additional facilitator signals,
separate agent roles, multiple chats, formal handoff, and commits are unnecessary.

Take check commands from README. Preserve the starter tests; do not skip,
disable, or weaken them. PASS requires an actual run. Recompile and rerun after
code changes. Identify uncommitted working copies honestly, and label supplied
output as supplied rather than your own execution.

Ask the task owner if a behavior decision beyond the exercise contract is needed.
Record a concrete blocker inside the task in `task.md`. Do not invent product
decisions or successful check output.
