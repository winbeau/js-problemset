# AGENTS

## Commit Convention

提交信息统一使用：

```text
<type>: <summary>
```

`<summary>` 使用简短中文或英文短语，聚焦本次变更的核心结果，不写空泛描述。

支持的 `<type>`：

- `feat`: 新功能、新能力、新增可直接给用户使用的内容
- `fix`: 缺陷修复、错误修正、回归修复、异常行为修复
- `refactor`: 不改变目标行为的结构调整、重组、重命名、生成逻辑重排
- `docs`: 文档新增或更新，包括 `README`、说明、规范、roadmap
- `chore`: 杂项维护，不属于功能、修复、重构或文档的例行变更

## Examples

- `feat: add school-level problem index`
- `fix: correct duplicated problem filename`
- `refactor: reorder pending problems after completed ones`
- `docs: define repository commit types`
- `chore: refresh generated roadmap files`

## Usage Notes

- 一次提交应尽量只表达一个主意图；若同时包含多类变更，以主要变更类型为准。
- 纯文档改动使用 `docs`。
- 生成结果随脚本或结构调整一起更新时，优先使用触发该批变更的主类型，而不是机械地全部记为 `chore`。

## Nowcoder Packaging

- `python3 scripts/package_nowcoder_week.py week1`
- 该命令会把 `nowcoder/week1/` 打成 15 个单题 zip 和 1 个整周 zip。
- 默认输出目录为 `nowcoder/packages/week1/`。
- 产物命名为 `A.zip` 到 `O.zip`，以及 `week1.zip`。
