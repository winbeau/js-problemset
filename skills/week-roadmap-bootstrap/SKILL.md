---
name: week-roadmap-bootstrap
description: Initialize or resume the nowcoder/weekx skeleton for one training week. Use when a week needs a resumable ROADMAP, A-O problem folders, copied markdown statements with renamed H1 titles, or copied run_std.cpp files before code generation begins.
---

# Week Roadmap Bootstrap

Use this helper before generating `gen.cpp` and `std.cpp`.

## Inputs To Infer

- week source dir: `组题/WEEKx-*`
- week target dir: `nowcoder/weekx`
- source markdown list: sorted `NN-题名.md`, excluding `README.md`

Reject the task only if the source week is missing or does not uniquely map to one directory.

## Bootstrap Steps

1. Enumerate the week's source markdown files in sorted order.
2. Map them to letters `A..O` in that same order.
3. Create or update `nowcoder/weekx/ROADMAP.md`.
4. For each problem:
   - ensure `nowcoder/weekx/<letter>/` exists
   - copy the source markdown into the target directory
   - rename the copied markdown file to the new H1 text
   - replace only the first H1 line; preserve the rest of the file verbatim
   - copy `nowcoder/run_std.cpp` into the problem directory
5. Update roadmap checkboxes immediately after each completed item.

## H1 And Markdown Rules

- The destination markdown filename must exactly match the new H1 plus `.md`.
- Change only the top-level H1.
- Preserve the full body and source citation block.
- If the destination markdown already exists, verify the H1 and keep it if correct; do not rewrite blindly.

## ROADMAP Rules

Create a markdown checklist that is resumable by a later session.

Per problem, track:

- `mkdir + md`
- `run_std.cpp`
- `gen.cpp`
- `std.cpp`
- `compile + run`

Use:

- `[x]` for done
- `[ ]` for pending
- `⚠ <brief reason>` at the end of a line for failures or manual review

If the disk state is ahead of the roadmap, mark completed items to match the disk.

## Defaults

- Treat already existing folders and files as recoverable progress, not as conflicts.
- Do not generate `gen.cpp` or `std.cpp` here.
- Do not compile here.
