> 训练周次：WEEK4 栈、队列与单调结构（5.18-5.24）
> 原题来源：浙江大学 保研 012
> 原题路径：`schools/浙江大学/012-One Way In, Two Ways Out.md`
> 难度 / 范围：中等 / 数据结构,队列

# One Way In, Two Ways Out

#### 题目描述

Consider a special queue which is a linear structure that allows insertions at one end, yet deletions at both ends. Your job is to check, for a given insertion sequence, if a deletion sequence is possible. For example, if we insert $1$, $2$, $3$, $4$, and $5$ in order, then it is possible to obtain $1$, $3$, $2$, $5$, and $4$ as an output, but impossible to obtain $5$, $1$, $3$, $2$, and $4$.

#### 输入格式

Each input file contains one test case. For each case, the first line gives $2$ positive integers $N$ and $K$ ($\leq 10$), which are the number of insertions and the number of queries, respectively. Then $N$ distinct numbers are given in the next line, as the insertion sequence. Finally $K$ lines follow, each contains $N$ inserted numbers as the deletion sequence to be checked.

#### 输出格式

For each deletion sequence, print in a line `yes` if it is indeed possible to be obtained, or `no` otherwise.

#### 输入样例
```

5 4
10 2 3 4 5
10 3 2 5 4
5 10 3 2 4
2 3 10 4 5
3 5 10 4 2

```

#### 输出样例
```
yes
no
yes
yes
```
