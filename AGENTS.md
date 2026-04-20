# AGENTS

## Repo Handoff

- 仓库整体结构、目录职责和常用工作流见 `README.md`。
- 若任务涉及某个 `WEEKx` 的 nowcoder 资源，优先阅读 `skills/nowcoder-week-pipeline/SKILL.md`。
- 若目标目录 `nowcoder/weekx/` 已存在，先读取该目录下的 `ROADMAP.md`，默认按续建处理。
- `skills/` 在本仓库内既是 skill 定义，也是一套可复用 SOP 文档；即便客户端不会自动发现，也可以显式要求 agent 先阅读对应 `SKILL.md` 再执行。

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

## Week Workflow

- 题面来源默认来自 `组题/WEEKx-*`。
- 评测资源目标目录默认是 `nowcoder/weekx/`。
- 标准顺序是：
  - 建骨架与 `ROADMAP.md`
  - 复制题面与 `run_std.cpp`
  - 生成 `gen.cpp` / `std.cpp`
  - 编译并生成 `1.in..20.in` 与 `1.out..20.out`
  - 抽样校验
  - 打包到 `nowcoder/packages/weekx/`
- 若用户没有明确授权，不要擅自使用多 agent 并行；若用户明确要求并行，可按 `skills/week-problem-generation/SKILL.md` 中的批次和职责拆分执行。

## Skill Usage

- 作为仓库内 SOP 使用时，推荐显式提示：
  - `请先阅读 skills/nowcoder-week-pipeline/SKILL.md，然后处理 WEEK2。`
- 如需全局 skill 方式，可将 `skills/*` 复制或软链接到本机 `~/.codex/skills/`；是否支持自动发现取决于客户端实现。
