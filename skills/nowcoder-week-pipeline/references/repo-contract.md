# Repo Contract

This repository already fixes the main nowcoder week layout:

- source statements live under `组题/WEEKx-*`
- evaluation assets live under `nowcoder/weekx`
- packaging uses `python3 scripts/package_nowcoder_week.py weekx`
- package output lives under `nowcoder/packages/weekx`

Per-problem folder contract:

- directory name: single uppercase letter `A..O`
- exactly one renamed statement markdown
- `gen.cpp`
- `std.cpp`
- `run_std.cpp`
- generated `gen.exe`, `std.exe`, `run_std.exe`
- generated `1.in..20.in`
- generated `1.out..20.out`

Roadmap contract:

- `nowcoder/weekx/ROADMAP.md` is the single recovery checkpoint
- every problem tracks:
  - `mkdir + md`
  - `run_std.cpp`
  - `gen.cpp`
  - `std.cpp`
  - `compile + run`
- global checks include:
  - `A..O` plus `ROADMAP.md`
  - expected file counts
  - representative sample verification
  - packaging status when requested

Packaging contract:

- one problem zip per letter
- one week zip
- default names `A.zip` through `O.zip` and `weekx.zip`
- single-problem archives should contain paths rooted at `A/`, `B/`, etc.
- week archive should contain paths rooted at `weekx/`
