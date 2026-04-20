#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


README_NAME = "README.md"
TITLE_RE = re.compile(r"^(#{1,6})\s*(.*?)\s*$")
OLD_HEADING_PATTERNS = [
    re.compile(r"^####\s*输入格式\s*$"),
    re.compile(r"^####\s*输出格式\s*$"),
    re.compile(r"^####\s*输入样例\d*\s*$"),
    re.compile(r"^####\s*输出样例\d*\s*$"),
    re.compile(r"^#\s+#(?:\s*#)*\s*$"),
]


def iter_problem_files(root: Path) -> list[Path]:
    return sorted(path for path in root.rglob("*.md") if path.name != README_NAME)


def validate_file(path: Path) -> list[str]:
    errors: list[str] = []
    lines = path.read_text(encoding="utf-8").splitlines()
    non_empty = [(index + 1, line) for index, line in enumerate(lines) if line.strip()]
    if not non_empty:
        return ["empty file"]
    first_line_no, first_line = non_empty[0]
    if not re.fullmatch(r"#\s+.+", first_line):
        errors.append(f"line {first_line_no}: file must start with a single H1 title")

    headings: list[tuple[int, int, str]] = []
    in_code_block = False
    for index, line in enumerate(lines, start=1):
        if line.strip().startswith("```"):
            in_code_block = not in_code_block
            continue
        if in_code_block:
            continue
        for pattern in OLD_HEADING_PATTERNS:
            if pattern.match(line):
                errors.append(f"line {index}: legacy heading residue `{line}`")
        match = TITLE_RE.match(line)
        if match:
            headings.append((index, len(match.group(1)), match.group(2).strip()))

    heading_names = [heading for _, _, heading in headings]
    required_h2 = ["题目描述", "输入描述", "输出描述", "样例1"]
    for heading in required_h2:
        if heading not in heading_names:
            errors.append(f"missing heading: {heading}")

    sample_indices = []
    for _, level, heading in headings:
        match = re.fullmatch(r"样例(\d+)", heading)
        if level == 2 and match:
            sample_indices.append(int(match.group(1)))
    for index in sorted(sample_indices):
        seen_input = False
        seen_output = False
        inside = False
        for _, level, heading in headings:
            if level == 2 and heading == f"样例{index}":
                inside = True
                continue
            if inside and level == 2 and heading != f"样例{index}":
                break
            if inside and level == 3 and heading == "输入":
                seen_input = True
            if inside and level == 3 and heading == "输出":
                seen_output = True
        if not seen_input:
            errors.append(f"sample {index} missing `### 输入`")
        if not seen_output:
            errors.append(f"sample {index} missing `### 输出`")

    data_range_index = None
    for idx, (line_no, level, heading) in enumerate(headings):
        if heading == "数据范围":
            data_range_index = idx
            if level != 3:
                errors.append(f"line {line_no}: `数据范围` must be H3 under 输入描述")
            parent_ok = False
            for prev in range(idx - 1, -1, -1):
                if headings[prev][1] == 2:
                    parent_ok = headings[prev][2] == "输入描述"
                    break
            if not parent_ok:
                errors.append(f"line {line_no}: `数据范围` must be placed under `输入描述`")

    sep_indexes = [idx for idx, line in enumerate(lines) if line.strip() == "---"]
    meta_start = None
    for idx, line in enumerate(lines):
        if line.startswith("> "):
            meta_start = idx
            break
    if meta_start is not None:
        if not sep_indexes:
            errors.append("metadata block must be preceded by `---`")
        else:
            last_sep = sep_indexes[-1]
            if last_sep > meta_start:
                errors.append("`---` separator appears after metadata block")
            if last_sep < meta_start - 2:
                errors.append("metadata block must appear immediately after `---` (optionally one blank line)")
        for idx in range(meta_start, len(lines)):
            line = lines[idx]
            if line.strip() == "":
                continue
            if not line.startswith("> "):
                errors.append(f"line {idx + 1}: non-metadata content found after metadata block")
                break

    return errors


def main() -> None:
    parser = argparse.ArgumentParser(description="Strictly validate normalized group problem markdown files.")
    parser.add_argument("root", nargs="?", default="组题", help="Root directory containing weekly problem sets.")
    args = parser.parse_args()
    root = Path(args.root)
    if not root.exists():
        raise SystemExit(f"Root not found: {root}")

    failures = 0
    files = iter_problem_files(root)
    for path in files:
        errors = validate_file(path)
        if not errors:
            continue
        failures += 1
        print(f"[FAIL] {path}")
        for error in errors:
            print(f"  - {error}")

    passed = len(files) - failures
    print(f"checked={len(files)} passed={passed} failed={failures}")
    if failures:
        sys.exit(1)


if __name__ == "__main__":
    main()
