# 各大高校计算机考研/保研复试机试真题

仓库现已重构为“原始题库 + 整理题库 + 进度账本”三层结构，便于持续切分、补全和长期维护。

## 目录结构

- `raw/`：保留原始学校级整份题单，共 76 个原始 `.md` 文件。
- `schools/`：整理后的学校题库，共 47 个学校目录、1365 道独立题目文件。
- `组题/`：按周组织的训练题单目录，当前内置前 4 周专题训练集。
- `nowcoder/`：按周落地的评测资源目录，当前包含 `week1/` 与 `packages/`。
- `skills/`：仓库内 skill / SOP 目录，用于把某一周从题面处理到评测资源与打包的流程固定下来。
- `docs/roadmap/`：重构实施进度、异常清单、并行分工和后续待办。

## 当前统计

- 已整理题目：464
- 待补充题目：901
- 总题目数：1365
- 已整理难度分布：简单 278 / 中等 150 / 困难 36

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

### 查看训练题单

进入 `组题/`：

- 顶层 `README.md` 汇总周次、日期、主题与实际选题统计
- 每周一个子目录，包含题单 `README.md` 与复制出的训练题面
- 组题仅使用 `已整理 + keep` 的题目，跨周不重复
- 当前按主题纯度优先组织前 4 周，部分周次会因题池不足而少于 15 题

### 查看 nowcoder 评测资源

进入 `nowcoder/`：

- `nowcoder/weekx/` 表示某一周已经落地的评测资源
- 每周目录下固定使用 `A..O` 作为题目目录
- 每题目录内通常包含：
  - 重命名后的题面 markdown
  - `gen.cpp`
  - `std.cpp`
  - `run_std.cpp`
  - `gen.exe` / `std.exe` / `run_std.exe`
  - `1.in..20.in`
  - `1.out..20.out`
- `nowcoder/weekx/ROADMAP.md` 是该周的唯一恢复账本
- `nowcoder/packages/weekx/` 保存单题 zip 与整周 zip

## WEEK 处理流水线

仓库当前约定了一条固定流水线，把 `组题/WEEKx-*` 处理为 `nowcoder/weekx/`：

1. 建立或续用 `ROADMAP.md`
2. 建立 `A..O` 目录并复制题面
3. 生成每题的 `gen.cpp` 与 `std.cpp`
4. 编译并落盘 `20` 组 `.in/.out`
5. 抽样校验
6. 打包为单题 zip 和整周 zip

当前打包命令为：

```bash
python3 scripts/package_nowcoder_week.py week1
```

该命令会把 `nowcoder/week1/` 打成 `A.zip` 到 `O.zip`，以及 `week1.zip`，默认输出到 `nowcoder/packages/week1/`。

## 仓库内 Skills / SOP

根目录 `skills/` 既可以理解为仓库内 skill，也可以理解为一套可复用 SOP 文档：

- `skills/nowcoder-week-pipeline/`：主 skill，负责整周总控
- `skills/week-roadmap-bootstrap/`：建骨架与 `ROADMAP`
- `skills/week-problem-generation/`：生成 `gen.cpp` / `std.cpp`
- `skills/week-build-verify/`：编译、生成 `.in/.out`、抽样验收
- `skills/week-package-release/`：打包 zip

### 作为“可复用 SOP 文档”使用

这是最稳妥的用法，不依赖客户端自动发现仓库内 skill。直接在 Claude / Codex 里显式要求先读 `SKILL.md`：

```text
请先阅读 skills/nowcoder-week-pipeline/SKILL.md，然后按这个 skill 处理 WEEK2。
```

如果只想执行某一阶段：

```text
请阅读 skills/week-build-verify/SKILL.md，只对 WEEK2 做编译、生成 .in/.out 和验收。
```

### 作为全局 skill 使用

如果你本地的 Codex 环境支持全局 skill，可以把仓库内 `skills/*` 复制或软链接到本机 skill 目录，例如 `~/.codex/skills/`，再按 skill 名直接调用。这里仅简单提示这种用法，不把全局安装流程作为仓库主工作流。

## 如何接续开发

接手本仓库时，推荐顺序：

1. 先读本 `README.md`
2. 再读 `AGENTS.md` 与 `CLAUDE.md`
3. 若处理某一周，先读 `skills/nowcoder-week-pipeline/SKILL.md`
4. 若目标周目录已存在，优先读 `nowcoder/weekx/ROADMAP.md`
5. 从第一个未完成项继续，不重做已完成项

默认接续原则：

- 优先续建，不重复已完成步骤
- 以磁盘真实状态为准，必要时回填 `ROADMAP.md`
- 除非用户明确要求，否则不要顺手改动不相关周次
- 打包、提交属于后置动作，通常在编译和验收完成后再做
- 多 agent 并行只在用户明确要求时启用

## 仓库入口文件职责

- `README.md`：面向人类接手者，描述整体结构和主要工作流
- `AGENTS.md`：面向通用 coding agent，描述提交约定与执行习惯
- `CLAUDE.md`：面向 Claude / Codex 风格代理，描述 week 流水线与恢复规则
- `nowcoder/weekx/ROADMAP.md`：面向具体周次的中断恢复
- `skills/`：面向可复用、可组合的仓库内 SOP

## 说明

- 首轮重构优先完成结构切分、难度推测和索引生成。
- `已整理` 题已补齐难度字段与范围标签。
- 原文中 `待添加` 的题目已保留为独立文件，并在学校 `README` 中标记为 `待补充`。
- 学校 `README` 采用“两段分组”：先列已整理题目，再列待补充题目。
- 同文件内的占位型重复题会自动去重；同校高重合的已整理题会额外复核并清理明确重复项。
