#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile


ROOT = Path(__file__).resolve().parent.parent
NOWCODER_DIR = ROOT / "nowcoder"
PACKAGES_DIR = NOWCODER_DIR / "packages"


def iter_files(root: Path) -> list[Path]:
    return sorted(path for path in root.rglob("*") if path.is_file())


def write_zip(zip_path: Path, files: list[Path], arcname_root: Path) -> None:
    zip_path.parent.mkdir(parents=True, exist_ok=True)
    if zip_path.exists():
        zip_path.unlink()
    with ZipFile(zip_path, "w", compression=ZIP_DEFLATED) as archive:
        for path in files:
            archive.write(path, path.relative_to(arcname_root))


def package_problem(problem_dir: Path, output_dir: Path) -> Path:
    zip_path = output_dir / f"{problem_dir.name}.zip"
    write_zip(zip_path, iter_files(problem_dir), problem_dir.parent)
    return zip_path


def package_week(week_dir: Path, output_dir: Path) -> Path:
    zip_path = output_dir / f"{week_dir.name}.zip"
    write_zip(zip_path, iter_files(week_dir), week_dir.parent)
    return zip_path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Package nowcoder week assets into per-problem zips and a week zip."
    )
    parser.add_argument(
        "week",
        help="Week directory name under nowcoder, for example: week1",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        help="Optional output directory for zip files. Defaults to nowcoder/packages/<week>.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    week_dir = NOWCODER_DIR / args.week
    if not week_dir.is_dir():
        raise SystemExit(f"week directory not found: {week_dir}")

    output_dir = args.output_dir if args.output_dir else PACKAGES_DIR / args.week
    output_dir = output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    problem_dirs = sorted(
        path
        for path in week_dir.iterdir()
        if path.is_dir() and len(path.name) == 1 and path.name.isalpha()
    )
    if not problem_dirs:
        raise SystemExit(f"no problem directories found under: {week_dir}")

    produced: list[Path] = []
    for problem_dir in problem_dirs:
        produced.append(package_problem(problem_dir, output_dir))
    produced.append(package_week(week_dir, output_dir))

    print(f"packaged {len(problem_dirs)} problem zips and 1 week zip into {output_dir}")
    for path in produced:
        print(path.relative_to(ROOT))


if __name__ == "__main__":
    main()
