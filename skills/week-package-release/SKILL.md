---
name: week-package-release
description: Package one completed nowcoder week into per-problem zips and a week zip. Use when nowcoder/weekx already contains validated judge assets and the next step is producing A-O zip files plus weekx.zip with the repository packaging script.
---

# Week Package Release

Use this helper only after build and verification are complete.

## Packaging Command

Run:

```bash
python3 scripts/package_nowcoder_week.py weekx
```

Replace `weekx` with the lowercase target directory name, for example `week1`.

## Expected Output

The default output directory is:

- `nowcoder/packages/weekx/`

Expected files:

- `A.zip` through `O.zip`
- `weekx.zip`

## Validation

After packaging, verify:

- there are exactly 16 zip files in `nowcoder/packages/weekx/`
- a single-problem archive contains paths rooted at `A/`, `B/`, etc.
- the week archive contains paths rooted at `weekx/`

Recommended checks:

```bash
find nowcoder/packages/weekx -maxdepth 1 -type f | sort
unzip -l nowcoder/packages/weekx/A.zip | sed -n '1,40p'
unzip -l nowcoder/packages/weekx/weekx.zip | sed -n '1,40p'
```

## Scope Boundary

- Do not regenerate code or data here.
- Do not commit here unless the user explicitly asks.
- If packaging fails because the week directory is incomplete, return to `week-build-verify` first.
