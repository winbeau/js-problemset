# CLAUDE

## Nowcoder Packaging

- `python3 scripts/package_nowcoder_week.py week1`
- 该命令会把 `nowcoder/week1/` 打成 15 个单题 zip 和 1 个整周 zip。
- 默认输出目录为 `nowcoder/packages/week1/`。
- 产物命名为 `A.zip` 到 `O.zip`，以及 `week1.zip`。

## Commit Convention

- 提交信息使用 `<type>: <summary>`。
- `<summary>` 使用简短中文或英文短语，聚焦本次变更的核心结果。
- 常用类型：`feat`、`fix`、`refactor`、`docs`、`chore`。
