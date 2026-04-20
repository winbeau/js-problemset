// gen.cpp - 数据生成器：最小价值和
// 生成 1.in .. 20.in
// 1..14  随机：多组(1..5)，n 从小到 1e5，a,b 覆盖全范围
// 15..17 卡常：单文件包含 5 组、每组 n=1e5；或单组 n=1e5 a,b 接近 1e9（long long 测试）
// 18..20 极端：n=1 / n=2 均等 / 全零

#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

static long long randint(long long lo, long long hi) {
    uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

struct Pair { long long a, b; };

static void writeCase(FILE* fp, int n, const vector<Pair>& v) {
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; ++i) {
        fprintf(fp, "%lld %lld\n", v[i].a, v[i].b);
    }
}

// 生成一组 n 规模的随机 (a, b)，值域 [vlo, vhi]
static vector<Pair> genGroup(int n, long long vlo, long long vhi) {
    vector<Pair> g(n);
    for (int i = 0; i < n; ++i) {
        g[i].a = randint(vlo, vhi);
        g[i].b = randint(vlo, vhi);
    }
    return g;
}

static void openAndWrite(int idx, const vector<pair<int, vector<Pair>>>& cases) {
    char name[64];
    snprintf(name, sizeof(name), "%d.in", idx);
    FILE* fp = fopen(name, "w");
    if (!fp) { fprintf(stderr, "fopen %s failed\n", name); exit(1); }
    for (auto& c : cases) writeCase(fp, c.first, c.second);
    fclose(fp);
}

int main() {
    // ---------------- 1..14 随机：多组 (1..5)，n 从小到 1e5 ----------------
    // 设计 n 上限梯度，前面小，后面到达 1e5
    int nCapSeq[14] = {
        3, 5, 10, 20, 50, 200, 1000, 3000,
        10000, 20000, 40000, 60000, 80000, 100000
    };

    for (int t = 1; t <= 14; ++t) {
        int groups = (int)randint(1, 5);
        int nCap = nCapSeq[t - 1];
        // 值域：前期小一些，后期放开到 1e9
        long long vhi;
        if (t <= 4) vhi = 100;
        else if (t <= 8) vhi = 100000;
        else vhi = 1000000000LL;

        vector<pair<int, vector<Pair>>> cases;
        long long totalN = 0;
        for (int g = 0; g < groups; ++g) {
            int n = (int)randint(1, max(1, nCap));
            // 防止总 n 过大（个别测试点综合规模控制）
            if (totalN + n > 500000LL) n = (int)max(1LL, 500000LL - totalN);
            if (n < 1) n = 1;
            vector<Pair> v = genGroup(n, 0, vhi);
            cases.push_back({n, v});
            totalN += n;
            if (totalN >= 500000LL) break;
        }
        openAndWrite(t, cases);
    }

    // ---------------- 15: 单文件 5 组，每组 n=1e5（累加 5e5 对） ----------------
    {
        vector<pair<int, vector<Pair>>> cases;
        for (int g = 0; g < 5; ++g) {
            int n = 100000;
            vector<Pair> v = genGroup(n, 0, 1000000000LL);
            cases.push_back({n, v});
        }
        openAndWrite(15, cases);
    }

    // ---------------- 16: 单组 n=1e5，a,b 均接近 1e9（测 long long） ----------------
    {
        int n = 100000;
        vector<Pair> v(n);
        for (int i = 0; i < n; ++i) {
            v[i].a = randint(900000000LL, 1000000000LL);
            v[i].b = randint(900000000LL, 1000000000LL);
        }
        vector<pair<int, vector<Pair>>> cases{{n, v}};
        openAndWrite(16, cases);
    }

    // ---------------- 17: 单文件 5 组，每组 n=1e5，值均接近 1e9 ----------------
    {
        vector<pair<int, vector<Pair>>> cases;
        for (int g = 0; g < 5; ++g) {
            int n = 100000;
            vector<Pair> v(n);
            for (int i = 0; i < n; ++i) {
                v[i].a = randint(900000000LL, 1000000000LL);
                v[i].b = randint(900000000LL, 1000000000LL);
            }
            cases.push_back({n, v});
        }
        openAndWrite(17, cases);
    }

    // ---------------- 18: 单组 n=1 ----------------
    {
        vector<Pair> v(1);
        v[0].a = randint(0, 1000000000LL);
        v[0].b = randint(0, 1000000000LL);
        vector<pair<int, vector<Pair>>> cases{{1, v}};
        openAndWrite(18, cases);
    }

    // ---------------- 19: 单组 n=2，a_i == b_i 全等 ----------------
    {
        vector<Pair> v(2);
        long long x = randint(0, 1000000000LL);
        v[0].a = x; v[0].b = x;
        v[1].a = x; v[1].b = x;
        vector<pair<int, vector<Pair>>> cases{{2, v}};
        openAndWrite(19, cases);
    }

    // ---------------- 20: 全零 ----------------
    {
        int n = 100;
        vector<Pair> v(n);
        for (int i = 0; i < n; ++i) { v[i].a = 0; v[i].b = 0; }
        vector<pair<int, vector<Pair>>> cases{{n, v}};
        openAndWrite(20, cases);
    }

    return 0;
}
