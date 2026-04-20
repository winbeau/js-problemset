---
name: week-build-verify
description: Compile, run, and verify one nowcoder week after gen.cpp and std.cpp exist. Use when a week needs 1.in-20.in and 1.out-20.out generated on disk, per-problem compile status recorded, representative sample checks run, and ROADMAP updated from real results.
---

# Week Build Verify

Use this helper after all problem folders have `gen.cpp`, `std.cpp`, and `run_std.cpp`.

## Per-Problem Commands

Run these inside each `nowcoder/weekx/<letter>/` directory:

```bash
g++ -O2 -std=c++17 gen.cpp -o gen.exe
g++ -O2 -std=c++17 std.cpp -o std.exe
g++ -O2 -std=c++17 run_std.cpp -o run_std.exe
./gen.exe
PATH=.:$PATH ./run_std.exe
```

The `PATH=.:$PATH` prefix is the default Linux-compatible way to keep the copied `run_std.cpp` unchanged while allowing it to invoke `std.exe`.

## Verification Rules

For every problem:

- ensure all three executables were built
- ensure `1.in..20.in` exist
- ensure `1.out..20.out` exist
- ensure `diff <(./std.exe < 1.in) 1.out` succeeds

Representative checks:

- sample letters default to `A`, `F`, `J`, `O`
- if statement sample input is easy to extract, replay it against `std.exe`
- otherwise at minimum verify the first generated case and output pair are self-consistent

Global checks:

- `ls nowcoder/weekx/` should show `A..O` and `ROADMAP.md`
- each problem directory should have at least:
  - `1 md`
  - `3 cpp`
  - `3 exe`
  - `20 in`
  - `20 out`

## Failure Handling

- If compile fails, record `⚠` on `compile + run` and move to the next problem after capturing the failing stage.
- If generation fails, keep the source files intact and record the failure.
- If validation fails, repair only the affected problem, then rerun that problem's compile and verification path.

## Roadmap Sync

- Mark `compile + run` complete only after executables and all `1..20` input/output files exist.
- Mark global checks complete only after all corresponding spot checks have been performed.
