# Source provenance and session branches

This repository adapts the public [ByAxe/audit-log-service](https://github.com/ByAxe/audit-log-service) for the Wargaming Blitz C++ cohort. The source training sequence was used for the World of Tanks HEAT Java cohort. The source code and public participant materials identify the snapshots below; private correspondence, transcripts and facilitator answer keys are not copied here.

| C++ starting ref | Public Java source ref | Java revision |
| --- | --- | --- |
| `main`, `session-1/start`, `heat-session-1-baseline` | `main`, `heat-session-1-baseline` | [`f07485e50bedfa579ddc77d01215fb4c20fb4e81`](https://github.com/ByAxe/audit-log-service/commit/f07485e50bedfa579ddc77d01215fb4c20fb4e81) |
| `session-2/start`, `session-2/pagination-investigation`, tag `session-2-baseline` | `session-2/pagination-investigation`, tag `session-2-baseline` | [`f07485e50bedfa579ddc77d01215fb4c20fb4e81`](https://github.com/ByAxe/audit-log-service/commit/f07485e50bedfa579ddc77d01215fb4c20fb4e81) |
| `session-3/start` | `session-3/start` | [`e5ca211dfa0c30f676691b121dd3b802bb53b057`](https://github.com/ByAxe/audit-log-service/commit/e5ca211dfa0c30f676691b121dd3b802bb53b057) |
| `session-4/start` | `session-4/start` | [`917fd1b8943aaa77db7c57eb58235b2b89039671`](https://github.com/ByAxe/audit-log-service/commit/917fd1b8943aaa77db7c57eb58235b2b89039671) |
| `session-5/start` | `session-5/start` | [`c4b13336cdd21e9c050a822bee6b8a7301dad6f5`](https://github.com/ByAxe/audit-log-service/commit/c4b13336cdd21e9c050a822bee6b8a7301dad6f5) |

These are source revisions, not C++ commit IDs or C++ execution receipts. Each C++ branch contains the corresponding port and its participant instructions.

Sessions 1 and 2 deliberately share a starting implementation. Session 3 starts with the result prepared after session 2. Session 4 includes a prepared homework-3 result; its Java production change originates at [`028d9bbb0d07ca4194486f0a8a96ec06ebb77c14`](https://github.com/ByAxe/audit-log-service/commit/028d9bbb0d07ca4194486f0a8a96ec06ebb77c14). Session 5 keeps the session-4 application and adds a separate small team exercise.

The goal is parity with the source branch's observable contract, current limitations and exercise gaps. Later solutions are not backported. Java-specific tooling and framework behavior are adapted as described in [C++ adaptation](cpp-adaptation.md).
