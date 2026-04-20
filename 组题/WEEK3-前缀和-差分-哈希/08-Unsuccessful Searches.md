> 训练周次：WEEK3 前缀和、差分与哈希（5.11-5.17）
> 原题来源：浙江大学 考研 020
> 原题路径：`schools/浙江大学/020-Unsuccessful Searches.md`
> 难度 / 范围：中等 / 哈希,模拟

# Unsuccessful Searches

#### 题目描述

Given an initially empty hash table $ HT $ of size $ 11 $.

The hash function is $ H(key) = key \% 7 $, with linear probing used to resolve the collisions.

Now hash the keys $ 87 $, $ 40 $, $ 30 $, $ 6 $, $ 11 $, $ 22 $, $ 98 $ and $ 20 $ one by one into $ HT $.

What is the average search time for unsuccessful searches?

The answer is $ 6 $.

Now you are supposed to write a program to solve this kind of problems.

#### 输入格式

Each input file contains one test case.

For each case, the first line gives 3 positive integers $ TSize $ ($ \leq 10^3 $, the table size), $ M $ ($ \leq TSize $, the divisor in the hash function), and $ N $ ($ \leq TSize $, the number of integers to be inserted).

Then $ N $ non-negative integers ($ \leq 10^4 $) are given in the next line, separated by spaces.

#### 输出格式

Print in a line the average search time for unsuccessful searches, after hashing the $ N $ integers into the table.

The answer must be accurate up to $ 1 $ decimal place.

#### 输入样例
```
11 7 8
87 40 30 6 11 22 98 20
```

#### 输出样例
```
6.0
```
