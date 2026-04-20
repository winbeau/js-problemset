---
name: week-problem-generation
description: Generate or repair gen.cpp and std.cpp for all A-O problems in one nowcoder week. Use when a week already has the markdown skeleton and run_std.cpp copies, and the next step is producing deterministic test generators, official solutions, and a clear parallel generation plan.
---

# Week Problem Generation

Use this helper after the week skeleton exists and before compilation.

Read `references/agent-prompt-contract.md` before drafting prompts or local implementations.

## Goal

For each problem folder `A..O`, produce:

- `gen.cpp`
- `std.cpp`

Both files must match the copied markdown statement in that folder.

## Generation Rules

- `gen.cpp` writes `1.in..20.in` directly from `main()`.
- Use a fixed random seed.
- Sample tiers are fixed:
  - `1..14`: random coverage
  - `15..17`: stress
  - `18..20`: edge
- `std.cpp` reads from `stdin` and writes to `stdout` with exact formatting.
- Both files must compile with `g++ -O2 -std=c++17`.

## Parallel Plan

Use this only when the current user explicitly asked for parallel agents or delegation.

- Spawn two workers per problem:
  - generator worker owns only `<letter>/gen.cpp`
  - solution worker owns only `<letter>/std.cpp`
- Batch the week into:
  - batch 1: `A..H`
  - batch 2: `I..O`
- Each worker prompt must include:
  - source markdown path
  - target file path
  - problem summary and I/O rules
  - 20-case structure
  - C++17 and formatting constraints
- After a batch returns, integrate results into the roadmap before starting compilation.

If sub-agents are not allowed in the current session, keep the same ownership and order but implement locally.

## Repair Policy

- If a file already exists, review it first.
- Reuse valid implementations instead of rewriting from scratch.
- Rewrite only when the statement and the code disagree, compilation is likely to fail, or generated inputs would violate the statement.

## Roadmap Sync

Mark `gen.cpp` and `std.cpp` complete only after the files exist and have passed a basic source sanity check.
