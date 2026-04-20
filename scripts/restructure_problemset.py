#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import json
import re
import shutil
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
RAW_DIR = ROOT / "raw"
SCHOOLS_DIR = ROOT / "schools"
DOCS_DIR = ROOT / "docs" / "roadmap"
ANALYSIS_PATH = ROOT / "docs" / "problem-analysis.json"
CATALOG_CSV_PATH = ROOT / "docs" / "problem-catalog.csv"

FILE_RE = re.compile(r"^(?P<school>.*)计算机(?P<kind>保研机试真题|考研复试机试真题)\.md$")
STD_HEADING_RE = re.compile(r"^##\s+(?P<title>.+?)\s*-\s*(?P<school>.+?)\s*$")
META_HEADING_RE = re.compile(r"^##\s*(题目描述|样例|输入格式|输出格式|边界说明)\s*$")
TOP_LINK_RE = re.compile(r"^\[\*\*完整题目及在线评测：https://pgcode\.cn/\*\*\]\(https://pgcode\.cn/\)\s*$")

SOURCE_LABELS = {
    "保研机试真题": "保研",
    "考研复试机试真题": "考研",
}
SOURCE_ORDER = {"保研": 0, "考研": 1}
SCOPE_LABELS = [
    "模拟", "枚举", "排序", "数学", "字符串", "双指针",
    "递归", "回溯", "DFS", "BFS", "搜索",
    "数据结构", "栈", "队列", "链表", "堆", "并查集", "树状数组",
    "树", "二叉树", "图论", "最短路", "最小生成树",
    "二分", "前缀和", "差分", "哈希",
    "动态规划", "背包", "区间DP", "RMQ", "ST表", "KMP",
]
SCOPE_SET = set(SCOPE_LABELS)


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
    stable_key: str
    difficulty: str = "待定"
    scope: str = ""
    dedupe_decision: str = "keep"
    duplicate_group: str = ""


@dataclass
class DuplicateResolution:
    source_file: str
    title: str
    resolution: str


@dataclass
class NearDuplicateIssue:
    school: str
    keys: list[str]
    titles: list[str]
    decision: str
    canonical: str | None
    reason: str


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


def make_stable_key(school: str, source_label: str, title: str, content: str, occurrence: int) -> str:
    digest = hashlib.sha1(content.encode("utf-8")).hexdigest()[:12]
    return f"{school}::{source_label}::{normalize_title(title)}::{occurrence:02d}::{digest}"


def load_analysis() -> dict:
    if not ANALYSIS_PATH.exists():
        return {"version": 1, "schools": {}}
    data = json.loads(ANALYSIS_PATH.read_text(encoding="utf-8"))
    data.setdefault("version", 1)
    data.setdefault("schools", {})
    return data


def infer_difficulty(problem: Problem) -> str:
    text = f"{problem.title}\n{problem.content}"
    hard_keywords = [
        "最短路径", "最小生成树", "连通", "欧拉", "哈夫曼", "AVL", "背包", "LCS",
        "最长公共子序列", "图", "树上", "调度", "网络", "最优", "概率", "搜索",
        "迷宫", "路径", "子矩阵", "最长子回文串", "青蛙过河",
    ]
    medium_keywords = [
        "二叉树", "树", "排序", "子序列", "回文", "组合", "查找", "矩阵", "字符串",
        "密码", "进制", "日期", "质数", "阶乘", "队列", "链表", "洗牌", "数组",
        "众数", "中位数", "第K", "转置", "前缀", "后缀", "逆序", "构造",
    ]
    score = 0
    score += sum(keyword in text for keyword in hard_keywords) * 2
    score += sum(keyword in text for keyword in medium_keywords)
    if "#### 输入格式" in problem.content and "#### 输出格式" in problem.content:
        score += 1
    if len(problem.content) > 900:
        score += 1
    if len(problem.content) > 1500:
        score += 1
    if score >= 7:
        return "困难"
    if score >= 3:
        return "中等"
    return "简单"


def normalize_scope(value: str) -> str:
    labels = []
    for part in value.split(","):
        label = part.strip()
        if label and label in SCOPE_SET and label not in labels:
            labels.append(label)
    return ",".join(labels[:3])


def infer_scope(problem: Problem) -> str:
    text = f"{problem.title}\n{problem.content}"
    labels: list[str] = []

    def add(label: str) -> None:
        if label in SCOPE_SET and label not in labels and len(labels) < 3:
            labels.append(label)

    if any(keyword in text for keyword in ["KMP", "前缀函数", "next数组", "失配"]):
        add("字符串")
        add("KMP")
    if any(keyword in text for keyword in ["树状数组", "Fenwick", "lowbit"]):
        add("数据结构")
        add("树状数组")
    if any(keyword in text for keyword in ["RMQ", "区间最值查询"]):
        add("RMQ")
    if any(keyword in text for keyword in ["ST表", "Sparse Table"]):
        add("ST表")
    if any(keyword in text for keyword in ["双指针", "滑动窗口", "尺取", "左右指针"]):
        add("双指针")
    if any(keyword in text for keyword in ["最短路径", "Dijkstra", "Floyd", "Bellman"]):
        add("图论")
        add("最短路")
    if any(keyword in text for keyword in ["最小生成树", "Kruskal", "Prim"]):
        add("图论")
        add("最小生成树")
    if any(keyword in text for keyword in ["图", "边", "顶点", "邻接", "可达", "连通"]):
        add("图论")
    if any(keyword in text for keyword in ["二叉树", "前序", "中序", "后序", "BST"]):
        add("二叉树")
    elif "树" in text:
        add("树")
    if any(keyword in text for keyword in ["DFS", "深度优先"]):
        add("DFS")
    if any(keyword in text for keyword in ["BFS", "广度优先", "队列搜索"]):
        add("BFS")
    if any(keyword in text for keyword in ["搜索", "迷宫", "走迷宫"]):
        add("搜索")
    if any(keyword in text for keyword in ["回溯", "八皇后", "全排列"]):
        add("回溯")
    if any(keyword in text for keyword in ["动态规划", "DP", "最优值", "状态转移", "最长上升子序列", "LCS"]):
        add("动态规划")
    if any(keyword in text for keyword in ["背包", "采药", "点菜问题"]):
        add("动态规划")
        add("背包")
    if any(keyword in text for keyword in ["区间DP"]):
        add("动态规划")
        add("区间DP")
    if any(keyword in text for keyword in ["前缀和"]):
        add("前缀和")
    if any(keyword in text for keyword in ["差分"]):
        add("差分")
    if any(keyword in text for keyword in ["哈希", "映射", "字典"]):
        add("哈希")
    if any(keyword in text for keyword in ["二分", "二分查找", "第K", "中位数", "有序"]):
        add("二分")
    if any(keyword in text for keyword in ["栈"]):
        add("数据结构")
        add("栈")
    if any(keyword in text for keyword in ["队列"]):
        add("数据结构")
        add("队列")
    if any(keyword in text for keyword in ["链表"]):
        add("数据结构")
        add("链表")
    if any(keyword in text for keyword in ["堆", "优先队列"]):
        add("数据结构")
        add("堆")
    if any(keyword in text for keyword in ["并查集", "Union-Find"]):
        add("数据结构")
        add("并查集")
    if any(keyword in text for keyword in ["字符串", "子串", "单词", "密码", "文本"]):
        add("字符串")
    if any(keyword in text for keyword in ["排序", "排名", "排序后"]):
        add("排序")
    if any(keyword in text for keyword in ["枚举", "遍历所有", "求所有", "所有可能"]):
        add("枚举")
    if any(keyword in text for keyword in ["数", "素数", "公约数", "质因数", "进制", "阶乘", "组合数", "几何", "矩阵面积"]):
        add("数学")
    if any(keyword in text for keyword in ["模拟", "按规则", "照题意"]):
        add("模拟")

    if not labels:
        title = problem.title
        if any(keyword in title for keyword in ["排序", "排名"]):
            add("排序")
        elif any(keyword in title for keyword in ["字符串", "密码", "单词"]):
            add("字符串")
        elif any(keyword in title for keyword in ["树", "二叉树"]):
            add("二叉树" if "二叉树" in title else "树")
        elif any(keyword in title for keyword in ["图", "路径", "网络", "道路"]):
            add("图论")
        elif any(keyword in title for keyword in ["数组", "矩阵", "队列", "栈", "链表", "堆"]):
            add("数据结构")
        elif any(keyword in title for keyword in ["最短", "最小", "最大", "第K", "最长"]):
            add("动态规划")
        else:
            add("模拟")

    return ",".join(labels[:3])


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


def apply_analysis_to_school(
    school: str,
    problems: list[Problem],
    analysis: dict,
) -> tuple[list[Problem], list[NearDuplicateIssue]]:
    school_data = analysis.get("schools", {}).get(school, {})
    difficulty_map = school_data.get("difficulties", {})
    scope_map = school_data.get("scopes", {})
    duplicate_specs = school_data.get("duplicates", [])
    problems_by_key = {problem.stable_key: problem for problem in problems}

    for problem in problems:
        if problem.status == "已整理":
            problem.difficulty = difficulty_map.get(problem.stable_key, infer_difficulty(problem))
            problem.scope = normalize_scope(scope_map.get(problem.stable_key, infer_scope(problem)))
        else:
            problem.difficulty = "待定"
            problem.scope = ""

    issues: list[NearDuplicateIssue] = []
    keys_to_remove: set[str] = set()
    for idx, spec in enumerate(duplicate_specs, start=1):
        keys = [key for key in spec.get("keys", []) if key in problems_by_key]
        if len(keys) < 2:
            continue
        decision = spec.get("decision", "review")
        canonical = spec.get("canonical")
        if canonical not in keys:
            canonical = keys[0]
        reason = spec.get("reason", "")
        group_id = f"{school}-dup-{idx:02d}"
        for key in keys:
            problem = problems_by_key[key]
            problem.duplicate_group = group_id
            problem.dedupe_decision = decision
            if decision == "review" and "近重复待复核" not in problem.note:
                problem.note = "，".join(filter(None, [problem.note, "近重复待复核"]))
        if decision == "remove":
            for key in keys:
                if key != canonical:
                    keys_to_remove.add(key)
        issues.append(
            NearDuplicateIssue(
                school=school,
                keys=keys,
                titles=[problems_by_key[key].title for key in keys],
                decision=decision,
                canonical=canonical,
                reason=reason,
            )
        )

    kept = [problem for problem in problems if problem.stable_key not in keys_to_remove]
    return kept, issues


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
            f"| {offset:03d} | [{problem.title}]({rel}) | {problem.source_label} | {problem.difficulty} | {problem.scope} | {problem.status} | {note} |\n"
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
        "- 难度枚举：`简单 / 中等 / 困难`；`待补充` 题保持 `待定`。\n",
        "- `已整理` 题已补齐范围标签；`待补充` 题暂留空。\n",
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


def write_catalog_csv(school_problems: dict[str, list[Problem]]) -> None:
    CATALOG_CSV_PATH.parent.mkdir(parents=True, exist_ok=True)
    with CATALOG_CSV_PATH.open("w", encoding="utf-8", newline="") as fh:
        writer = csv.writer(fh)
        writer.writerow([
            "school",
            "index",
            "title",
            "source",
            "status",
            "difficulty",
            "scope",
            "readme_section",
            "relative_path",
            "duplicate_group",
            "dedupe_decision",
            "notes",
        ])
        for school in sorted(school_problems):
            for index, problem in enumerate(school_problems[school], start=1):
                writer.writerow([
                    school,
                    f"{index:03d}",
                    problem.title,
                    problem.source_label,
                    problem.status,
                    problem.difficulty,
                    problem.scope,
                    "已整理题目" if problem.status == "已整理" else "待补充题目",
                    f"schools/{school}/{problem.filename}",
                    problem.duplicate_group,
                    problem.dedupe_decision,
                    problem.note,
                ])


def write_root_readme(total_files: int, total_schools: int, total_problems: int, school_problems: dict[str, list[Problem]]) -> None:
    ready = [problem for problems in school_problems.values() for problem in problems if problem.status == "已整理"]
    pending = [problem for problems in school_problems.values() for problem in problems if problem.status == "待补充"]
    difficulty_counts = Counter(problem.difficulty for problem in ready)
    content = f"""# 各大高校计算机考研/保研复试机试真题

仓库现已重构为“原始题库 + 整理题库 + 进度账本”三层结构，便于持续切分、补全和长期维护。

## 目录结构

- `raw/`：保留原始学校级整份题单，共 {total_files} 个原始 `.md` 文件。
- `schools/`：整理后的学校题库，共 {total_schools} 个学校目录、{total_problems} 道独立题目文件。
- `docs/roadmap/`：重构实施进度、异常清单、并行分工和后续待办。

## 当前统计

- 已整理题目：{len(ready)}
- 待补充题目：{len(pending)}
- 总题目数：{total_problems}
- 已整理难度分布：简单 {difficulty_counts['简单']} / 中等 {difficulty_counts['中等']} / 困难 {difficulty_counts['困难']}

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
- `problem-catalog.csv`：全仓题目汇总表，包含状态、难度、路径和去重决策

## 说明

- 首轮重构优先完成结构切分、难度推测和索引生成。
- `已整理` 题已补齐难度字段与范围标签。
- 原文中 `待添加` 的题目已保留为独立文件，并在学校 `README` 中标记为 `待补充`。
- 学校 `README` 采用“两段分组”：先列已整理题目，再列待补充题目。
- 同文件内的占位型重复题会自动去重；同校高重合的已整理题会额外复核并清理明确重复项。
"""
    ROOT.joinpath("README.md").write_text(content, encoding="utf-8")


def write_docs(
    source_files: list[SourceFile],
    file_issues: dict[str, list[dict]],
    summaries: dict[str, dict],
    manifest: dict[str, int],
    school_problems: dict[str, list[Problem]],
    duplicate_resolutions: dict[tuple[str, str], str],
    near_duplicate_issues: list[NearDuplicateIssue],
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
- 同校已整理题的高重合清理由 `docs/problem-analysis.json` 驱动，只有 `remove` 决策会自动删除。
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
    for issue in near_duplicate_issues:
        title = " / ".join(issue.titles).replace("|", "\\|")
        action = issue.reason.replace("|", "\\|")
        status = "resolved" if issue.decision == "remove" else "pending"
        exceptions_lines.append(
            f"| `{issue.school}` |  | near_duplicate_ready_problem | medium | {title} | {action} | {status} |\n"
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
        "- [x] 为全部 `已整理` 题补齐难度推测，并生成题目总表 `docs/problem-catalog.csv`。\n",
        "- [x] 复核并清理同校内明确高重合的已整理题。\n\n",
        "## 待办\n\n",
        "- [ ] 为题目补充范围标签。\n",
        "- [ ] 复核 `exceptions.md` 中标记的异常标题与单题文件。\n",
        "- [ ] 复核仍保留的正式题面重复项和 `near_duplicate_ready_problem` 待复核项是否需要进一步合并或补备注。\n",
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
    analysis = load_analysis()

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
    near_duplicate_issues: list[NearDuplicateIssue] = []
    stable_key_counters: dict[str, Counter] = defaultdict(Counter)

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
                digest = hashlib.sha1(content.encode("utf-8")).hexdigest()[:12]
                stable_tuple = (source.source_label, normalize_title(title), digest)
                stable_key_counters[school][stable_tuple] += 1
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
                        stable_key=make_stable_key(school, source.source_label, title, content, stable_key_counters[school][stable_tuple]),
                    )
                )
                source_order += 1
        school_problems[school], resolutions = dedupe_school_problems(school_problems[school])
        duplicate_resolutions.update(resolutions)
        school_problems[school], school_near_dups = apply_analysis_to_school(school, school_problems[school], analysis)
        near_duplicate_issues.extend(school_near_dups)
        school_problems[school].sort(key=lambda problem: (problem.status == "待补充", problem.source_order))
        for final_index, problem in enumerate(school_problems[school], start=1):
            filename = f"{final_index:03d}-{sanitize_filename(problem.title)}.md"
            school_dir.joinpath(filename).write_text(problem.content, encoding="utf-8")
            problem.filename = filename
        write_school_readme(school_dir, school, school_problems[school])

    manifest = freeze_manifest(grouped)
    write_docs(source_files, file_issues, summaries, manifest, school_problems, duplicate_resolutions, near_duplicate_issues)
    write_catalog_csv(school_problems)
    total_problems = sum(len(items) for items in school_problems.values())
    write_root_readme(len(source_files), len(grouped), total_problems, school_problems)


if __name__ == "__main__":
    main()
