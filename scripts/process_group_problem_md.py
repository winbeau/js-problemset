#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
from dataclasses import dataclass, field
from pathlib import Path


TITLE_RE = re.compile(r"^(#{1,6})\s*(.*?)\s*$")
README_NAME = "README.md"

TITLE_TRANSLATIONS = {
    "schools/北京大学/033-W's Cipher.md": "W 的密码",
    "schools/北京大学/038-Repeater.md": "重复绘图",
    "schools/复旦大学/002-INTa语言解释器.md": "INTa 语言解释器",
    "schools/浙江大学/030-Median.md": "中位数",
    "schools/北京邮电大学/026-第K小的数.md": "第 K 小的数",
    "schools/浙江大学/002-k线彩灯覆盖2.md": "K 线彩灯覆盖 2",
    "schools/浙江大学/020-Unsuccessful Searches.md": "查找失败的平均查找长度",
    "schools/深圳大学/005-DNA序列.md": "DNA 序列",
    "schools/浙江大学/012-One Way In, Two Ways Out.md": "单端入队，双端出队",
}

BODY_TRANSLATIONS = {
    "schools/北京大学/033-W's Cipher.md": {
        "description": """一家名为 Weird Wally's Wireless Widgets, Inc. 的公司生产各种体积很小、支持无线联网的设备，例如狗项圈、铅笔和浮漂等。这些设备的内存非常有限，像 $Rijndael$ 这样的标准加密算法虽然安全，但无法装入如此小的存储空间。为了给设备之间的通信提供一定的安全性，公司采用了下面这种加密方式，你需要实现其解密过程。

加密和解密都依赖三个整数密钥 $k1$、$k2$、$k3$。字符被分成三组：$[a-i]$ 为第一组，$[j-r]$ 为第二组，其余字符（$[s-z]$ 和下划线）为第三组。加密时，每一组中的字符在消息中分别向左循环移动 $k_i$ 个位置；解密时，则在各自分组内向右循环移动 $k_i$ 个位置。三组字符彼此独立移动，互不影响。

例如，字符串 $the\\_quick\\_brown\\_fox$ 使用 $k1=2$、$k2=3$、$k3=1$ 加密后，会得到 $\\_icuo\\_bfnwhoq\\_kxert$。

输入字符串只包含小写字母和下划线（$\\_$），字符串长度不超过 $80$，三个密钥均为 $1$ 到 $100$ 之间的正整数。""",
        "input": """输入包含若干组加密消息。

每组数据第一行包含三个整数 $k1$、$k2$、$k3$，第二行给出加密后的字符串。

当一行中三个密钥都为 $0$ 时，表示输入结束。""",
        "output": "对于每组加密消息，输出一行对应的解密结果。",
    },
    "schools/北京大学/038-Repeater.md": {
        "description": """生活中处处需要和谐，但 Facer 却偏偏感受不到。有一天，人们传言重复作画可以创造和谐，于是许多人开始根据一个很小的模板不断复制，形成越来越大的图案。Facer 能想象整幅图的样子，却无法亲手画出来，于是你需要编写程序把这幅图显示出来。

给定的模板仅由一种字符和空格组成。构造更高层图案的方法是：把模板中的非空字符视为“放置一个更小一级图案”的位置，把空格视为空白；然后用整个模板去替换这些非空字符的位置，递归重复这一过程。

下面是一个 $3 \\times 3$ 的模板示例：

```
# #
 #
# #
```

那么：

- $Level\\ 1$ 图案就是模板本身；
- $Level\\ 2$ 图案则是在模板中每个 `#` 所在位置放置一个完整的 $Level\\ 1$ 图案，其余位置保留空白，得到更大的分形图案。""",
        "input": """输入包含多组测试数据。

每组数据第一行是一个整数 $N$，表示模板大小为 $N \\times N$，其中 $N$ 只能取 $3$、$4$ 或 $5$。

接下来 $N$ 行给出模板内容。

随后一行包含一个整数 $Q$，表示要输出的图案层级 $Scale\\ Level$。

当 $N = 0$ 时输入结束。

保证任意一组输出图案的尺寸都不会超过 $3000 \\times 3000$。""",
        "output": "对于每组测试数据，输出给定模板构造出的第 $Q$ 层图案。",
    },
    "schools/浙江大学/030-Median.md": {
        "description": """给定一个长度为 $N$ 的递增整数序列 $S$，其中位数定义为位于中间位置的那个数。

例如，序列 $S_1 = \\{11, 12, 13, 14\\}$ 的中位数是 $12$，序列 $S_2 = \\{9, 10, 15, 16, 17\\}$ 的中位数是 $15$。

两个序列的中位数，定义为将两个序列合并成一个非递减序列后得到的中位数。比如 $S_1$ 与 $S_2$ 合并后的中位数是 $13$。

给定两个递增整数序列，请你求出它们合并后的中位数。""",
        "input": """输入可能包含多组测试数据。

每组数据占两行，每行描述一个序列。

对于每个序列，先给出一个正整数 $N$（$\\leq 1000000$），表示该序列的长度，随后给出 $N$ 个整数。

保证所有整数都在 `long int` 的表示范围内。""",
        "output": "对于每组测试数据，输出一行，表示两个序列合并后的中位数。",
    },
    "schools/浙江大学/020-Unsuccessful Searches.md": {
        "description": """给定一个初始为空的哈希表，使用线性探测法解决冲突。

例如，当哈希表大小为 $11$、哈希函数为 $H(key) = key \\% 7$ 时，依次插入 $87$、$40$、$30$、$6$、$11$、$22$、$98$ 和 $20$ 之后，查找失败时的平均查找长度为 $6$。

现在请你编写程序，求出一般情况下插入完成后“查找失败”的平均查找长度。""",
        "input": """每个输入文件包含一组测试数据。

第一行包含三个正整数 $TSize$、$M$ 和 $N$：

- $TSize$（$\\leq 10^3$）表示哈希表大小；
- $M$（$\\leq TSize$）表示哈希函数中的除数；
- $N$（$\\leq TSize$）表示待插入整数的个数。

第二行给出 $N$ 个非负整数（每个数 $\\leq 10^4$），表示按顺序插入哈希表的关键字。""",
        "output": "输出一行，表示所有整数插入完成后，查找失败的平均查找长度。结果精确到小数点后 $1$ 位。",
    },
    "schools/浙江大学/012-One Way In, Two Ways Out.md": {
        "description": """考虑一种特殊的队列：它只允许在一端插入元素，但允许从两端删除元素。

给定一个入队序列，你需要判断若干个出队序列是否有可能通过这种结构得到。

例如，若按顺序插入 $1, 2, 3, 4, 5$，则可以得到输出序列 $1, 3, 2, 5, 4$，但不可能得到 $5, 1, 3, 2, 4$。""",
        "input": """每个输入文件包含一组测试数据。

第一行给出两个正整数 $N$ 和 $K$（$\\leq 10$），分别表示入队元素个数和待判断的查询个数。

第二行给出 $N$ 个互不相同的整数，表示入队序列。

接下来 $K$ 行，每行给出一个长度为 $N$ 的序列，表示一个候选出队序列。""",
        "output": "对于每个候选出队序列，输出一行：若该序列可以得到则输出 `yes`，否则输出 `no`。",
    },
}

MAIN_HEADINGS = {
    "题目描述": "description",
    "description": "description",
    "输入格式": "input",
    "输入描述": "input",
    "input": "input",
    "output": "output",
    "输出格式": "output",
    "输出描述": "output",
    "数据范围": "data_range",
}
EXTRA_HEADINGS = {"提示", "说明", "备注", "注释", "注意"}


@dataclass
class SectionData:
    lines: list[str] = field(default_factory=list)
    extras: list[tuple[str, list[str]]] = field(default_factory=list)


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


def iter_problem_files(root: Path) -> list[Path]:
    return sorted(path for path in root.rglob("*.md") if path.name != README_NAME)


def split_metadata_and_body(text: str) -> tuple[list[str], list[str]]:
    lines = text.splitlines()
    idx = 0
    while idx < len(lines) and lines[idx].strip() == "":
        idx += 1
    metadata = []
    while idx < len(lines) and lines[idx].startswith("> "):
        metadata.append(lines[idx])
        idx += 1
    while idx < len(lines) and lines[idx].strip() == "":
        idx += 1
    return metadata, lines[idx:]


def extract_embedded_metadata(metadata: list[str], body_lines: list[str]) -> tuple[list[str], list[str]]:
    prefixes = (
        "> 训练周次：",
        "> 原题来源：",
        "> 原题路径：",
        "> 难度 / 范围：",
    )
    cleaned: list[str] = []
    idx = 0
    latest = metadata[:]
    while idx < len(body_lines):
        if body_lines[idx].strip() == "---":
            j = idx + 1
            while j < len(body_lines) and body_lines[j].strip() == "":
                j += 1
            block: list[str] = []
            while j < len(body_lines) and body_lines[j].startswith(prefixes):
                block.append(body_lines[j])
                j += 1
            if block:
                latest = block
                idx = j
                continue
        if body_lines[idx].startswith(prefixes):
            block = []
            while idx < len(body_lines) and body_lines[idx].startswith(prefixes):
                block.append(body_lines[idx])
                idx += 1
            if block:
                latest = block
            continue
        cleaned.append(body_lines[idx])
        idx += 1
    return latest, cleaned


def parse_metadata_value(metadata: list[str], prefix: str) -> str:
    for line in metadata:
        if line.startswith(prefix):
            return line.split("：", 1)[1].strip().strip("`")
    return ""


def extract_title_and_sections(body_lines: list[str]) -> tuple[str, dict[str, SectionData], dict[int, dict[str, list[str]]]]:
    title = ""
    sections = {
        "description": SectionData(),
        "input": SectionData(),
        "output": SectionData(),
        "data_range": SectionData(),
    }
    samples: dict[int, dict[str, list[str]]] = {}
    current_target: tuple[str, str | int, str | None] = ("section", "description", None)
    last_main = "description"
    current_sample = 1
    current_extra: tuple[str, list[str]] | None = None
    in_code_block = False

    def ensure_sample(index: int) -> dict[str, list[str]]:
        return samples.setdefault(index, {"输入": [], "输出": []})

    def append_line(line: str) -> None:
        nonlocal current_extra
        kind, target, label = current_target
        if kind == "section":
            sections[target].lines.append(line)
            return
        if kind == "data_range":
            sections["data_range"].lines.append(line)
            return
        if kind == "sample":
            ensure_sample(int(target))[label].append(line)
            return
        if kind == "extra":
            if current_extra is None or current_extra[0] != label:
                current_extra = (label, [])
                sections[target].extras.append(current_extra)
            current_extra[1].append(line)

    for raw_line in body_lines:
        if raw_line.strip().startswith("```"):
            append_line(raw_line)
            in_code_block = not in_code_block
            continue
        if in_code_block:
            append_line(raw_line)
            continue
        match = TITLE_RE.match(raw_line)
        if match:
            level = len(match.group(1))
            heading = match.group(2).strip()
            heading = heading.rstrip("：:")
            lowered = heading.lower()
            if level == 1 and not title:
                title = heading
                current_target = ("section", "description", None)
                current_extra = None
                continue
            if re.fullmatch(r"#\s*(#.*)?", heading) or heading.startswith("#"):
                continue
            sample_match = re.fullmatch(r"样例\s*(\d+)", heading)
            if sample_match:
                current_sample = int(sample_match.group(1))
                ensure_sample(current_sample)
                current_target = ("sample", current_sample, "输入")
                current_extra = None
                continue
            if level >= 3 and heading in {"输入", "输出"} and current_sample in samples:
                current_target = ("sample", current_sample, heading)
                current_extra = None
                continue
            sample_input = re.fullmatch(r"(?:输入样例|sample input)\s*(\d+)?", lowered)
            if sample_input:
                current_sample = int(sample_input.group(1) or "1")
                ensure_sample(current_sample)
                current_target = ("sample", current_sample, "输入")
                current_extra = None
                continue
            sample_output = re.fullmatch(r"(?:输出样例|sample output)\s*(\d+)?", lowered)
            if sample_output:
                current_sample = int(sample_output.group(1) or "1")
                ensure_sample(current_sample)
                current_target = ("sample", current_sample, "输出")
                current_extra = None
                continue
            normalized = MAIN_HEADINGS.get(heading) or MAIN_HEADINGS.get(lowered)
            if normalized == "data_range":
                current_target = ("data_range", "data_range", None)
                last_main = "input"
                current_extra = None
                continue
            if normalized in {"description", "input", "output"}:
                current_target = ("section", normalized, None)
                last_main = normalized
                current_extra = None
                continue
            label = heading or "附加说明"
            current_target = ("extra", last_main, label)
            current_extra = None
            continue
        append_line(raw_line)

    return title, sections, samples


def strip_outer_blank_lines(lines: list[str]) -> list[str]:
    start = 0
    end = len(lines)
    while start < end and lines[start].strip() == "":
        start += 1
    while end > start and lines[end - 1].strip() == "":
        end -= 1
    return lines[start:end]


def render_block(lines: list[str]) -> list[str]:
    cleaned = strip_outer_blank_lines(lines)
    if not cleaned:
        return []
    return [line.rstrip() for line in cleaned] + [""]


def render_extras(extras: list[tuple[str, list[str]]]) -> list[str]:
    rendered: list[str] = []
    for label, lines in extras:
        rendered.append(f"### {label}")
        rendered.append("")
        rendered.extend(render_block(lines))
    return rendered


def build_normalized_content(title: str, metadata: list[str], sections: dict[str, SectionData], samples: dict[int, dict[str, list[str]]]) -> str:
    lines = [f"# {title}", ""]
    lines.append("## 题目描述")
    lines.append("")
    lines.extend(render_block(sections["description"].lines))
    lines.extend(render_extras(sections["description"].extras))
    lines.append("## 输入描述")
    lines.append("")
    lines.extend(render_block(sections["input"].lines))
    if strip_outer_blank_lines(sections["data_range"].lines):
        lines.append("### 数据范围")
        lines.append("")
        lines.extend(render_block(sections["data_range"].lines))
    lines.extend(render_extras(sections["input"].extras))
    lines.append("## 输出描述")
    lines.append("")
    lines.extend(render_block(sections["output"].lines))
    lines.extend(render_extras(sections["output"].extras))

    for index in sorted(samples):
        lines.append(f"## 样例{index}")
        lines.append("")
        lines.append("### 输入")
        lines.append("")
        lines.extend(render_block(samples[index]["输入"]))
        lines.append("### 输出")
        lines.append("")
        lines.extend(render_block(samples[index]["输出"]))

    if metadata:
        lines.append("---")
        lines.append("")
        for meta_line in metadata:
            lines.append(meta_line.rstrip())
        lines.append("")

    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines) + "\n"


def apply_translations(source_path: str, title: str, sections: dict[str, SectionData]) -> str:
    translated_title = TITLE_TRANSLATIONS.get(source_path, title)
    if source_path in BODY_TRANSLATIONS:
        body = BODY_TRANSLATIONS[source_path]
        sections["description"].lines = body["description"].splitlines()
        sections["description"].extras = []
        sections["input"].lines = body["input"].splitlines()
        sections["input"].extras = []
        sections["output"].lines = body["output"].splitlines()
        sections["output"].extras = []
        sections["data_range"].lines = []
        sections["data_range"].extras = []
    return translated_title


def update_week_readme(readme_path: Path, replacements: dict[str, tuple[str, str]]) -> None:
    text = readme_path.read_text(encoding="utf-8")

    def repl(match: re.Match[str]) -> str:
        title = match.group(1)
        href = match.group(2)
        href_key = href[2:] if href.startswith("./") else href
        if href_key not in replacements:
            return match.group(0)
        new_href, new_title = replacements[href_key]
        return f"[{new_title}](./{new_href})"

    text = re.sub(r"\[([^\]]+)\]\(((?:\./)?[^)]+)\)", repl, text)
    readme_path.write_text(text, encoding="utf-8")


def process_week_dir(week_dir: Path) -> None:
    problem_files = sorted(path for path in week_dir.glob("*.md") if path.name != README_NAME)
    generated: dict[str, str] = {}
    replacements: dict[str, tuple[str, str]] = {}

    for path in problem_files:
        text = path.read_text(encoding="utf-8")
        metadata, body_lines = split_metadata_and_body(text)
        metadata, body_lines = extract_embedded_metadata(metadata, body_lines)
        source_path = parse_metadata_value(metadata, "> 原题路径：")
        title, sections, samples = extract_title_and_sections(body_lines)
        title = apply_translations(source_path, title, sections)
        normalized = build_normalized_content(title, metadata, sections, samples)
        prefix = path.name.split("-", 1)[0]
        new_name = f"{prefix}-{sanitize_filename(title)}.md"
        generated[new_name] = normalized
        replacements[path.name] = (new_name, title)

    for path in problem_files:
        path.unlink()
    for filename, content in sorted(generated.items()):
        week_dir.joinpath(filename).write_text(content, encoding="utf-8")
    update_week_readme(week_dir / README_NAME, replacements)


def main() -> None:
    parser = argparse.ArgumentParser(description="Translate and normalize problem markdown files under 组题/.")
    parser.add_argument("root", nargs="?", default="组题", help="Root directory containing weekly problem sets.")
    args = parser.parse_args()
    root = Path(args.root)
    if not root.exists():
        raise SystemExit(f"Root not found: {root}")
    for week_dir in sorted(path for path in root.iterdir() if path.is_dir()):
        process_week_dir(week_dir)


if __name__ == "__main__":
    main()
