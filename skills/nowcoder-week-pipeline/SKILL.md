---
name: nowcoder-week-pipeline
description: Orchestrate one training week into nowcoder judge assets end-to-end. Use when the user wants to process a repository week such as WEEK1, WEEK2, WEEK3, or WEEK4 into nowcoder/weekx with a resumable ROADMAP, A-O problem folders, copied markdown statements, gen.cpp/std.cpp/run_std.cpp, generated 1.in-20.in and 1.out-20.out, verification, packaging, or recovery from a half-finished week. This skill coordinates week-roadmap-bootstrap, week-problem-generation, week-build-verify, and week-package-release.
---

# Nowcoder Week Pipeline

Use this as the top-level workflow for any `WEEKx` request.

Read `references/repo-contract.md` first. Then load helper skills in this order:

1. `week-roadmap-bootstrap`
2. `week-problem-generation`
3. `week-build-verify`
4. `week-package-release`

## Default Entry

Assume the user gives only a week key such as `WEEK2`.

Infer these paths automatically:

- group source dir: the unique match for `组题/WEEKx-*`
- target dir: `nowcoder/weekx` where `x` is lowercase
- package dir: `nowcoder/packages/weekx`

If there is no unique `组题/WEEKx-*` match, stop and ask the user which source directory to use.

## Workflow

1. Inspect `AGENTS.md`, `CLAUDE.md`, `scripts/package_nowcoder_week.py`, the matched `组题/WEEKx-*` directory, and any existing `nowcoder/weekx/`.
2. Build or resume `nowcoder/weekx/ROADMAP.md` with `week-roadmap-bootstrap`.
3. Generate or repair per-problem `gen.cpp` and `std.cpp` with `week-problem-generation`.
4. Compile, generate `1.in..20.in`, generate `1.out..20.out`, and verify with `week-build-verify`.
5. Package the completed week with `week-package-release`.

## Recovery Rules

- Always prefer resuming over rebuilding.
- Read `nowcoder/weekx/ROADMAP.md` before making decisions.
- Start from the first unchecked or warned item.
- Do not redo items already marked complete unless validation proves they are broken.
- If the disk state is ahead of `ROADMAP.md`, update the roadmap to match reality.

## Multi-Agent Policy

When the user explicitly asks for parallel agents, delegation, or multi-agent execution, use the batching plan from `week-problem-generation`.

When the user does not explicitly authorize sub-agents, keep the same task decomposition but execute locally without `spawn_agent`.

## Output Contract

At the end of a successful run, the week should have:

- `nowcoder/weekx/A..O`
- one markdown statement per problem with the renamed H1
- `gen.cpp`, `std.cpp`, `run_std.cpp`
- `gen.exe`, `std.exe`, `run_std.exe`
- `1.in..20.in`
- `1.out..20.out`
- a fully updated `ROADMAP.md`
- packaged zips under `nowcoder/packages/weekx/`

## Defaults

- Problem folders are `A` through `O`.
- The problem order comes from the sorted source markdown files under `组题/WEEKx-*`.
- `run_std.cpp` is copied from `nowcoder/run_std.cpp` without modifying the source template.
- Packaging is included by default.
- Git add or commit is not included unless the user asks.
