#include <bits/stdc++.h>
using namespace std;

mt19937 rng(20240427);

static void writeCase(int idx, int n, const vector<long long>& a) {
    string name = to_string(idx) + ".in";
    FILE* fp = fopen(name.c_str(), "w");
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; ++i) {
        fprintf(fp, "%lld%c", a[i], i + 1 == n ? '\n' : ' ');
    }
    fclose(fp);
}

static long long randInt(long long lo, long long hi) {
    uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

int main() {
    const long long V = 1000000000LL;

    // Cases 1..14 随机: n 从 1 到 1e4 各量级；值域跨越 ±1e9；含重复元素使递增段被打断
    int ns[14] = {1, 2, 3, 5, 10, 20, 50, 100, 300, 800, 2000, 4000, 7000, 10000};
    for (int i = 0; i < 14; ++i) {
        int n = ns[i];
        vector<long long> a(n);
        // 为了含重复元素，缩小值域使重复概率提高：对较小 n 使用较窄值域
        long long range;
        if (n <= 10) range = max<long long>(3, (long long)n);
        else if (n <= 100) range = (long long)n / 2 + 5;
        else if (n <= 1000) range = (long long)n / 3 + 10;
        else range = max<long long>(50, (long long)n / 5);
        // 在 50% 情况下让值域贴近 ±1e9 边界
        if (i % 2 == 0) {
            for (int j = 0; j < n; ++j) {
                a[j] = randInt(-V, V);
            }
            // 强制插入一些重复元素以打断递增段
            int dupCnt = max(1, n / 20);
            for (int k = 0; k < dupCnt && n >= 2; ++k) {
                int p = (int)randInt(0, n - 2);
                a[p + 1] = a[p];
            }
        } else {
            for (int j = 0; j < n; ++j) {
                a[j] = randInt(-range, range);
            }
        }
        writeCase(i + 1, n, a);
    }

    // Case 15: n=1e4 全严格递增
    {
        int n = 10000;
        vector<long long> a(n);
        long long start = -V;
        for (int i = 0; i < n; ++i) a[i] = start + i; // 严格递增
        writeCase(15, n, a);
    }
    // Case 16: n=1e4 全严格递减
    {
        int n = 10000;
        vector<long long> a(n);
        long long start = V;
        for (int i = 0; i < n; ++i) a[i] = start - i; // 严格递减
        writeCase(16, n, a);
    }
    // Case 17: n=1e4 全相同
    {
        int n = 10000;
        vector<long long> a(n, 7);
        writeCase(17, n, a);
    }

    // Case 18: n=1
    {
        int n = 1;
        vector<long long> a = {randInt(-V, V)};
        writeCase(18, n, a);
    }
    // Case 19: n=2 增 与 n=2 相等 -> 输出两组里取一个。这里选 n=2 严格递增
    {
        int n = 2;
        vector<long long> a = {-V, V};
        writeCase(19, n, a);
    }
    // Case 20: n=10000 随机但混入几段长上升以测最大段
    {
        int n = 10000;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) a[i] = randInt(-V, V);
        // 混入若干段严格递增段
        int segs = 5;
        for (int s = 0; s < segs; ++s) {
            int len = (int)randInt(500, 2000);
            int start = (int)randInt(0, n - len);
            long long base = randInt(-V, V - len);
            for (int j = 0; j < len; ++j) a[start + j] = base + j;
            // 在段前后放置打断元素
            if (start > 0) a[start - 1] = a[start]; // 使前一位 >= 段首，打断
            if (start + len < n) a[start + len] = a[start + len - 1]; // 使后一位 <= 段尾
        }
        writeCase(20, n, a);
    }

    return 0;
}
