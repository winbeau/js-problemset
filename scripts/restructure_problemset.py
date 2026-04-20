#!/usr/bin/env python3
from __future__ import annotations

import re
import shutil
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
RAW_DIR = ROOT / "raw"
SCHOOLS_DIR = ROOT / "schools"
DOCS_DIR = ROOT / "docs" / "roadmap"

FILE_RE = re.compile(r"^(?P<school>.*)计算机(?P<kind>保研机试真题|考研复试机试真题)\.md$")
STD_HEADING_RE = re.compile(r"^##\s+(?P<title>.+?)\s*-\s*(?P<school>.+?)\s*$")
META_HEADING_RE = re.compile(r"^##\s*(题目描述|样例|输入格式|输出格式|边界说明)\s*$")
TOP_LINK_RE = re.compile(r"^\[\*\*完整题目及在线评测：https://pgcode\.cn/\*\*\]\(https://pgcode\.cn/\)\s*$")

SOURCE_LABELS = {
    "保研机试真题": "保研",
    "考研复试机试真题": "考研",
}
SOURCE_ORDER = {"保研": 0, "考研": 1}


@dataclass
class SourceFile:
    path: Path
    filename: str
    school: str
    source_label: str


@dataclass
class Problem:
    school: str
    source_label: str
    source_file: str
    source_path: Path
    source_order: int
    title: str
    filename: str
    content: str
    status: str
    note: str


@dataclass
class DuplicateResolution:
    source_file: str
    title: str
    resolution: str


def sanitize_filename(name: str) -> str:
    table = str.maketrans({
        "/": "／",
        "\\": "＼",
        ":": "：",
        "*": "＊",
        "\"": "＂",
        "<": "＜",
        ">": "＞",
        "|": "｜",
    })
    return name.translate(table).strip()


def detect_problem_status(content: str, title: str) -> str:
    return "待补充" if content.strip() == f"# {title}\n\n待添加" else "已整理"


def normalize_title(title: str) -> str:
    return re.sub(r"\s+", " ", title.strip())


def discover_source_files() -> list[SourceFile]:
    candidates: list[SourceFile] = []
    search_roots = [ROOT, RAW_DIR] if RAW_DIR.exists() else [ROOT]
    for base in search_roots:
        local: list[SourceFile] = []
        for path in sorted(base.glob("*.md")):
            if base == ROOT and path.name == "README.md":
                continue
            match = FILE_RE.match(path.name)
            if not match:
                continue
            local.append(
                SourceFile(
                    path=path,
                    filename=path.name,
                    school=match.group("school"),
                    source_label=SOURCE_LABELS[match.group("kind")],
                )
            )
        if local:
            return local
    return candidates


def strip_top_link(lines: list[str]) -> list[str]:
    idx = 0
    while idx < len(lines) and lines[idx].strip() == "":
        idx += 1
    if idx < len(lines) and TOP_LINK_RE.match(lines[idx].strip()):
        idx += 1
        while idx < len(lines) and lines[idx].strip() == "":
            idx += 1
        return lines[idx:]
    return lines


def extract_standard_headings(lines: list[str], school: str) -> tuple[list[tuple[int, str]], list[dict]]:
    standard: list[tuple[int, str]] = []
    issues: list[dict] = []
    for i, line in enumerate(lines):
        if not line.startswith("## "):
            continue
        match = STD_HEADING_RE.match(line.rstrip())
        if match:
            heading_school = match.group("school").strip()
            title = match.group("title").strip()
            if heading_school != school:
                issues.append(
                    {
                        "line_no": i + 1,
                        "raw_heading": line.rstrip(),
                        "issue_type": "school_suffix_mismatch",
                        "severity": "high",
                        "suggested_action": f"将标题学校后缀改为 {school}",
                    }
                )
                continue
            standard.append((i, title))
            continue
        issue_type = "meta_heading_at_h2" if META_HEADING_RE.match(line.rstrip()) else "missing_school_suffix"
        severity = "medium" if issue_type == "meta_heading_at_h2" else "high"
        suggested_action = "将该二级标题降级为正文小节" if issue_type == "meta_heading_at_h2" else "人工确认该标题是否为题目边界"
        issues.append(
            {
                "line_no": i + 1,
                "raw_heading": line.rstrip(),
                "issue_type": issue_type,
                "severity": severity,
                "suggested_action": suggested_action,
            }
        )
    return standard, issues


def build_problem_content(title: str, chunk_lines: list[str]) -> str:
    body = "".join(chunk_lines[1:]).lstrip("\n")
    return f"# {title}\n\n{body}".rstrip() + "\n"


def parse_file(source: SourceFile) -> tuple[list[tuple[str, str]], list[dict], dict]:
    raw_lines = source.path.read_text(encoding="utf-8").splitlines(keepends=True)
    lines = strip_top_link(raw_lines)
    standard, issues = extract_standard_headings(lines, source.school)
    summary = {
        "standard_h2_count": len(standard),
        "invalid_h2_count": len(issues),
        "single_problem_file": len(standard) == 1,
    }

    normalized_titles = [title.strip() for _, title in standard]
    counts = Counter(normalized_titles)
    for title, count in counts.items():
        if count > 1:
            for idx, (line_no, current) in enumerate(standard, start=1):
                if current == title:
                    issues.append(
                        {
                            "line_no": line_no + 1,
                            "raw_heading": lines[line_no].rstrip(),
                            "issue_type": "duplicate_title_in_file",
                            "severity": "medium",
                            "suggested_action": f"同文件内重复 {count} 次，依赖局部序号区分并人工复核",
                            "problem_index_in_file": idx,
                        }
                    )

    if not standard:
        return [], issues, summary

    chunks: list[tuple[str, str]] = []
    boundaries = standard + [(len(lines), "")]
    for idx in range(len(standard)):
        start_line, title = boundaries[idx]
        end_line, _ = boundaries[idx + 1]
        content = build_problem_content(title, lines[start_line:end_line])
        chunks.append((title, content))

    if len(standard) == 1:
        issues.append(
            {
                "line_no": standard[0][0] + 1,
                "raw_heading": lines[standard[0][0]].rstrip(),
                "issue_type": "single_problem_file",
                "severity": "low",
                "suggested_action": "按单题文件处理，不再继续二次切分",
                "problem_index_in_file": 1,
            }
        )

    return chunks, issues, summary


def ensure_clean_dir(path: Path) -> None:
    if path.exists():
        shutil.rmtree(path)
    path.mkdir(parents=True, exist_ok=True)


def dedupe_school_problems(problems: list[Problem]) -> tuple[list[Problem], dict[tuple[str, str], str]]:
    grouped: dict[tuple[str, str], list[Problem]] = defaultdict(list)
    for problem in problems:
        grouped[(problem.source_file, normalize_title(problem.title))].append(problem)

    kept: list[Problem] = []
    resolutions: dict[tuple[str, str], str] = {}
    for key, items in grouped.items():
        items.sort(key=lambda problem: problem.source_order)
        if len(items) == 1:
            kept.extend(items)
            continue

        ready = [problem for problem in items if problem.status == "已整理"]
        pending = [problem for problem in items if problem.status == "待补充"]
        if ready and pending:
            kept.extend(ready)
            resolutions[key] = "placeholder_removed"
            continue
        if pending and not ready:
            kept.append(pending[0])
            resolutions[key] = "pending_collapsed"
            continue

        kept.extend(items)
        resolutions[key] = "formal_duplicate_kept"

    kept.sort(key=lambda problem: problem.source_order)
    active_duplicates = Counter((problem.source_file, normalize_title(problem.title)) for problem in kept)
    for problem in kept:
        notes = [note for note in problem.note.split("，") if note]
        notes = [note for note in notes if note != "同文件重复题名"]
        if active_duplicates[(problem.source_file, normalize_title(problem.title))] > 1:
            notes.append("同文件重复题名")
        problem.note = "，".join(dict.fromkeys(notes))
    return kept, resolutions


def freeze_manifest(groups: dict[str, list[SourceFile]]) -> dict[str, int]:
    weighted = sorted(groups.items(), key=lambda item: (-len(item[1]), item[0]))
    buckets = {1: [], 2: [], 3: []}
    weights = {1: 0, 2: 0, 3: 0}
    manifest: dict[str, int] = {}
    for school, files in weighted:
        worker_id = min(weights, key=lambda wid: (weights[wid], wid))
        manifest[school] = worker_id
        buckets[worker_id].append(school)
        weights[worker_id] += len(files)
    return manifest


def render_problem_table(problems: list[Problem], start_index: int) -> list[str]:
    lines = [
        "| 序号 | 题目 | 来源 | 难度 | 范围 | 状态 | 备注 |\n",
        "| --- | --- | --- | --- | --- | --- | --- |\n",
    ]
    for offset, problem in enumerate(problems, start=start_index):
        rel = f"./{problem.filename}"
        note = problem.note or ""
        lines.append(
            f"| {offset:03d} | [{problem.title}]({rel}) | {problem.source_label} | 待定 |  | {problem.status} | {note} |\n"
        )
    return lines


def write_school_readme(school_dir: Path, school: str, problems: list[Problem]) -> None:
    ready = [problem for problem in problems if problem.status == "已整理"]
    pending = [problem for problem in problems if problem.status == "待补充"]
    has_review = any("待复核" in problem.note for problem in problems)
    lines = [
        f"# {school}\n\n",
        f"- 题目总数：{len(problems)}\n",
        f"- 已整理：{len(ready)}\n",
        f"- 待补充：{len(pending)}\n",
        f"- 待复核：{'是' if has_review else '否'}\n",
        "- 目录内混放保研/考研题目，来源以 `README` 表格标记区分。\n",
        "- 难度枚举：`简单 / 中等 / 困难`，首轮统一记为 `待定`。\n",
        "- 范围字段首轮留空，后续补齐标签。\n",
        "- 编号规则：前段编号全部为已整理题目，待补充题统一后置。\n\n",
        "## 已整理题目\n\n",
    ]
    if ready:
        lines.extend(render_problem_table(ready, start_index=1))
    else:
        lines.append("当前暂无已整理题目。\n")
    lines.append("\n## 待补充题目\n\n")
    if pending:
        lines.extend(render_problem_table(pending, start_index=len(ready) + 1))
    else:
        lines.append("当前暂无待补充题目。\n")
    school_dir.joinpath("README.md").write_text("".join(lines), encoding="utf-8")


def write_root_readme(total_files: int, total_schools: int, total_problems: int) -> None:
    content = f"""# 各大高校计算机考研/保研复试机试真题

仓库现已重构为“原始题库 + 整理题库 + 进度账本”三层结构，便于持续切分、补全和长期维护。

## 目录结构

- `raw/`：保留原始学校级整份题单，共 {total_files} 个原始 `.md` 文件。
- `schools/`：整理后的学校题库，共 {total_schools} 个学校目录、{total_problems} 道独立题目文件。
- `docs/roadmap/`：重构实施进度、异常清单、并行分工和后续待办。

## 如何使用

### 查看原始题库

直接进入 `raw/`，可查看各学校原始整份题单。

### 刷整理后的题目

进入 `schools/<学校>/`：

- 每道题拆为一个独立 `.md`
- 每个学校有一个 `README.md` 汇总题目、来源、难度、范围与状态
- `README` 中 `来源` 字段用于区分 `保研` / `考研`
- 题目编号规则为“已整理在前，待补充后置”
- 同文件同名且仅为 `待添加` 的重复题会自动清理

### 查看重构进度

进入 `docs/roadmap/`：

- `progress-index.md`：所有原始题单的 `pending/done` 状态账本
- `exceptions.md`：结构异常、重复题名、单题文件等问题记录
- `worker-manifest.md`：3 个并行 worker 的学校分片清单
- `todo.md`：详细待办与后续补全事项

## 说明

- 首轮重构优先完成结构切分和索引生成。
- 难度默认记为 `待定`，范围标签后续补齐。
- 原文中 `待添加` 的题目已保留为独立文件，并在学校 `README` 中标记为 `待补充`。
- 学校 `README` 采用“两段分组”：先列已整理题目，再列待补充题目。
- 同文件内的占位型重复题会自动去重；正式题面重复项继续保留并标记复核。
"""
    ROOT.joinpath("README.md").write_text(content, encoding="utf-8")


def write_docs(
    source_files: list[SourceFile],
    file_issues: dict[str, list[dict]],
    summaries: dict[str, dict],
    manifest: dict[str, int],
    school_problems: dict[str, list[Problem]],
    duplicate_resolutions: dict[tuple[str, str], str],
) -> None:
    DOCS_DIR.mkdir(parents=True, exist_ok=True)

    README = """# Roadmap Rules

- `progress-index.md` 是唯一主进度总账，一行对应一个原始学校级 `.md`。
- 主状态仅使用 `pending` / `done`。
- 结构异常、标题异常、重复题名等问题不写回主状态，统一记录到 `exceptions.md`。
- 并行执行以“学校”为最小写入归属，同一学校的保研/考研文件必须落在同一个 worker。
- 根 `README.md` 和 `docs/roadmap/*` 仅由主 agent 维护。
- 学校内编号规则固定为：`已整理` 在前、`待补充` 在后，同状态内保持原始顺序。
- 同文件同名重复题默认只自动清理占位项，不自动合并正式题面重复。
"""
    DOCS_DIR.joinpath("README.md").write_text(README, encoding="utf-8")

    pending_lines = ["## Pending\n\n", "| 类型 | 学校 | 原始文件 | 备注 |\n", "| --- | --- | --- | --- |\n"]
    done_lines = ["## Done\n\n", "| 类型 | 学校 | 原始文件 | 备注 |\n", "| --- | --- | --- | --- |\n"]
    for source in sorted(source_files, key=lambda item: (SOURCE_ORDER[item.source_label], item.school, item.filename)):
        issues = file_issues.get(source.filename, [])
        remark = "含异常待复核" if issues else "首轮完成"
        done_lines.append(f"| {source.source_label} | {source.school} | `{source.filename}` | {remark} |\n")
    progress = [
        "# Progress Index\n\n",
        f"- 原始文件总数：{len(source_files)}\n",
        f"- Pending：0\n",
        f"- Done：{len(source_files)}\n\n",
        *pending_lines,
        "\n",
        *done_lines,
    ]
    DOCS_DIR.joinpath("progress-index.md").write_text("".join(progress), encoding="utf-8")

    exceptions_lines = [
        "# Exceptions\n\n",
        "| 原始文件 | 行号 | 异常类型 | 严重级别 | 原始标题 | 建议处理 | 状态 |\n",
        "| --- | --- | --- | --- | --- | --- | --- |\n",
    ]
    for filename in sorted(file_issues):
        for issue in sorted(file_issues[filename], key=lambda item: (item.get("line_no", 0), item["issue_type"])):
            raw_heading = issue.get("raw_heading", "").replace("|", "\\|")
            action = issue.get("suggested_action", "")
            status = "pending"
            if issue["issue_type"] == "duplicate_title_in_file":
                match = STD_HEADING_RE.match(issue.get("raw_heading", ""))
                if match:
                    key = (filename, normalize_title(match.group("title")))
                    resolution = duplicate_resolutions.get(key)
                    if resolution == "placeholder_removed":
                        action = "已自动删除同名占位重复项，保留正式题面"
                        status = "resolved"
                    elif resolution == "pending_collapsed":
                        action = "已自动合并重复占位题，仅保留首个占位项"
                        status = "resolved"
            action = action.replace("|", "\\|")
            exceptions_lines.append(
                f"| `{filename}` | {issue.get('line_no', '')} | {issue['issue_type']} | {issue['severity']} | {raw_heading} | {action} | {status} |\n"
            )
    DOCS_DIR.joinpath("exceptions.md").write_text("".join(exceptions_lines), encoding="utf-8")

    worker_groups: dict[int, list[str]] = defaultdict(list)
    for school, worker_id in manifest.items():
        worker_groups[worker_id].append(school)
    manifest_lines = [
        "# Worker Manifest\n\n",
        "- 分片单位是学校，同校保研/考研文件始终归同一 worker。\n",
        "- 生成规则：按学校文件数降序、学校名升序排序，依次放入当前最轻的 worker。\n\n",
    ]
    for worker_id in sorted(worker_groups):
        schools = sorted(worker_groups[worker_id])
        file_count = sum(
            1 for source in source_files if manifest[source.school] == worker_id
        )
        problem_count = sum(len(school_problems[school]) for school in schools)
        manifest_lines.append(f"## Worker {worker_id}\n\n")
        manifest_lines.append(f"- 学校数：{len(schools)}\n")
        manifest_lines.append(f"- 原始文件数：{file_count}\n")
        manifest_lines.append(f"- 题目数：{problem_count}\n\n")
        manifest_lines.append("| 学校 | 原始文件数 | 题目数 |\n")
        manifest_lines.append("| --- | --- | --- |\n")
        for school in schools:
            original_count = sum(1 for source in source_files if source.school == school)
            manifest_lines.append(f"| {school} | {original_count} | {len(school_problems[school])} |\n")
        manifest_lines.append("\n")
    DOCS_DIR.joinpath("worker-manifest.md").write_text("".join(manifest_lines), encoding="utf-8")

    todo_lines = [
        "# Todo\n\n",
        "## 已完成\n\n",
        "- [x] 盘点 76 个原始学校级题库文件。\n",
        "- [x] 建立 `raw/`、`schools/`、`docs/roadmap/` 目标结构。\n",
        "- [x] 归档原始题库到 `raw/`。\n",
        "- [x] 按学校拆分为独立题目文件并生成学校 `README.md`。\n",
        "- [x] 生成 `progress-index.md`、`exceptions.md`、`worker-manifest.md`。\n",
        "- [x] 将所有原始 `.md` 标记为 `done`。\n",
        "- [x] 将学校内 `待补充` 题统一后置编号，并将 `README` 改为两段分组展示。\n",
        "- [x] 自动删除同文件内的占位型重复题，保留正式题面重复项待复核。\n\n",
        "## 待办\n\n",
        "- [ ] 补齐各学校 `README.md` 中的难度字段。\n",
        "- [ ] 为题目补充范围标签。\n",
        "- [ ] 复核 `exceptions.md` 中标记的异常标题与单题文件。\n",
        "- [ ] 复核仍保留的正式题面重复项是否需要进一步合并或补备注。\n",
        "- [ ] 补全所有 `待补充` 题面的正式内容。\n\n",
        "## 学校级实施清单\n\n",
        "| 学校 | Worker | 状态 | 题目数 | 备注 |\n",
        "| --- | --- | --- | --- | --- |\n",
    ]
    for school in sorted(school_problems):
        worker_id = manifest[school]
        remarks = "含异常需复核" if any(source.school == school and file_issues.get(source.filename) for source in source_files) else ""
        todo_lines.append(f"| {school} | {worker_id} | done | {len(school_problems[school])} | {remarks} |\n")
    DOCS_DIR.joinpath("todo.md").write_text("".join(todo_lines), encoding="utf-8")


def move_sources_to_raw(source_files: list[SourceFile]) -> list[SourceFile]:
    RAW_DIR.mkdir(parents=True, exist_ok=True)
    moved: list[SourceFile] = []
    for source in source_files:
        target = RAW_DIR / source.filename
        if source.path.resolve() != target.resolve():
            shutil.move(str(source.path), target)
        moved.append(
            SourceFile(
                path=target,
                filename=source.filename,
                school=source.school,
                source_label=source.source_label,
            )
        )
    return moved


def main() -> None:
    source_files = discover_source_files()
    if not source_files:
        raise SystemExit("No source markdown files found in repository root.")

    for path in [SCHOOLS_DIR, DOCS_DIR]:
        ensure_clean_dir(path)
    RAW_DIR.mkdir(parents=True, exist_ok=True)

    source_files = move_sources_to_raw(source_files)
    grouped: dict[str, list[SourceFile]] = defaultdict(list)
    for source in source_files:
        grouped[source.school].append(source)

    file_issues: dict[str, list[dict]] = {}
    summaries: dict[str, dict] = {}
    school_problems: dict[str, list[Problem]] = defaultdict(list)
    duplicate_resolutions: dict[tuple[str, str], str] = {}

    for school, files in grouped.items():
        files.sort(key=lambda item: (SOURCE_ORDER[item.source_label], item.filename))
        school_dir = SCHOOLS_DIR / school
        school_dir.mkdir(parents=True, exist_ok=True)
        source_order = 1
        for source in files:
            chunks, issues, summary = parse_file(source)
            summaries[source.filename] = summary
            if issues:
                file_issues[source.filename] = issues
            for title, content in chunks:
                status = detect_problem_status(content, title)
                note_parts = []
                duplicates = [
                    issue for issue in issues
                    if issue["issue_type"] == "duplicate_title_in_file" and issue.get("raw_heading") == f"## {title}-{school}"
                ]
                if duplicates:
                    note_parts.append("同文件重复题名")
                if any(issue["issue_type"] in {"single_problem_file", "missing_school_suffix", "school_suffix_mismatch", "meta_heading_at_h2"} for issue in issues):
                    note_parts.append("待复核")
                school_problems[school].append(
                    Problem(
                        school=school,
                        source_label=source.source_label,
                        source_file=source.filename,
                        source_path=source.path,
                        source_order=source_order,
                        title=title,
                        filename="",
                        content=content,
                        status=status,
                        note="，".join(dict.fromkeys(note_parts)),
                    )
                )
                source_order += 1
        school_problems[school], resolutions = dedupe_school_problems(school_problems[school])
        duplicate_resolutions.update(resolutions)
        school_problems[school].sort(key=lambda problem: (problem.status == "待补充", problem.source_order))
        for final_index, problem in enumerate(school_problems[school], start=1):
            filename = f"{final_index:03d}-{sanitize_filename(problem.title)}.md"
            school_dir.joinpath(filename).write_text(problem.content, encoding="utf-8")
            problem.filename = filename
        write_school_readme(school_dir, school, school_problems[school])

    manifest = freeze_manifest(grouped)
    write_docs(source_files, file_issues, summaries, manifest, school_problems, duplicate_resolutions)
    total_problems = sum(len(items) for items in school_problems.values())
    write_root_readme(len(source_files), len(grouped), total_problems)


if __name__ == "__main__":
    main()
