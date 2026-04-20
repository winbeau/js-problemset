# CLAUDE

## Repo Handoff

- 先读 `README.md` 获取仓库结构、目录职责和当前工作流。
- 处理某个 `WEEKx` 时，优先读 `skills/nowcoder-week-pipeline/SKILL.md`。
- 如果目标周目录已经存在，先读 `nowcoder/weekx/ROADMAP.md`，默认按续建处理，不重做已完成项。
- `skills/` 是仓库内的可复用 SOP / skill 定义；即便运行环境不会自动注册它们，也应把对应 `SKILL.md` 当成执行手册使用。

## Nowcoder Packaging

- `python3 scripts/package_nowcoder_week.py week1`
- 该命令会把 `nowcoder/week1/` 打成 15 个单题 zip 和 1 个整周 zip。
- 默认输出目录为 `nowcoder/packages/week1/`。
- 产物命名为 `A.zip` 到 `O.zip`，以及 `week1.zip`。

## Week Pipeline

- 默认输入：用户只给 `WEEKx`
- 自动推断：
  - 源题面目录：`组题/WEEKx-*`
  - 目标评测目录：`nowcoder/weekx/`
  - 打包目录：`nowcoder/packages/weekx/`
- 默认顺序：
  1. 建或续用 `ROADMAP.md`
  2. 建 `A..O` 目录并复制题面
  3. 生成 `gen.cpp` / `std.cpp`
  4. 编译并落盘 `20` 组 `.in/.out`
  5. 抽样验收
  6. 打包

## Skill Usage

- 作为仓库内 SOP 文档时，直接显式引用：
  - `请先阅读 skills/nowcoder-week-pipeline/SKILL.md，然后按该 skill 处理 WEEK2。`
  - `请阅读 skills/week-build-verify/SKILL.md，只执行 WEEK2 的编译与验收。`
- 如需把这些 skill 当作全局 skill 使用，可将 `skills/*` 安装到本机 skill 目录，例如 `~/.codex/skills/`。这里只做简单兼容说明，不把全局安装流程作为仓库主工作流。

## Execution Defaults

- 默认优先续建，不重建已完成目录。
- 默认先验证磁盘状态，再回填 `ROADMAP.md`。
- 默认包含打包，不默认包含提交。
- 多 agent 并行只在用户明确要求时启用。

## Commit Convention

- 提交信息使用 `<type>: <summary>`。
- `<summary>` 使用简短中文或英文短语，聚焦本次变更的核心结果。
- 常用类型：`feat`、`fix`、`refactor`、`docs`、`chore`。
