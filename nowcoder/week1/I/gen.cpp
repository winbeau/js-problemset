// gen.cpp - test case generator for "巨人排队" (LIS problem)
// Writes 1.in .. 20.in in the current working directory.
// Compile: g++ -std=c++17 -O2 gen.cpp -o gen && ./gen
#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

static inline int randint(int lo, int hi) {
    uniform_int_distribution<int> d(lo, hi);
    return d(rng);
}

static void writeGroup(FILE* fp, const vector<int>& h) {
    fprintf(fp, "%d\n", (int)h.size());
    for (size_t i = 0; i < h.size(); ++i) {
        fprintf(fp, "%d%c", h[i], " \n"[(i + 1) % 20 == 0 || i + 1 == h.size()]);
    }
}

static vector<int> genRandom(int n, int lo, int hi) {
    vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = randint(lo, hi);
    return v;
}

static vector<int> genStrictIncreasing(int n) {
    // values 1..n, must be strictly increasing and within [1, 30000]
    // n up to 1e5 but height <= 30000, so we cap uniqueness by spacing;
    // but problem states h <= 30000. For n=1e5 strictly increasing impossible
    // within 1..30000. Use range up to n directly (ignoring h<=30000 cap is
    // unsafe). Instead, use values 1..30000 scaled: pick n distinct? impossible.
    // So we relax: use 1..n (test stresses algorithm; grader only cares about
    // correctness regardless of h bound as long as generator outputs them).
    // To respect h<=30000, we use n values with small repeats only on boundary.
    // Safer: clamp n <= 30000 for this group.
    int m = min(n, 30000);
    vector<int> v(m);
    for (int i = 0; i < m; ++i) v[i] = i + 1;
    // pad remainder with max value (breaks strict increase); avoid by keeping m.
    return v;
}

static vector<int> genStrictDecreasing(int n) {
    int m = min(n, 30000);
    vector<int> v(m);
    for (int i = 0; i < m; ++i) v[i] = m - i;
    return v;
}

int main() {
    // Cases 1..14: random, 1..3 groups, n from small to large
    struct CaseCfg { int minGroups, maxGroups, minN, maxN, lo, hi; };
    CaseCfg cfg[14] = {
        {1, 1, 1, 5, 1, 30000},
        {1, 2, 1, 10, 1, 10},
        {2, 3, 1, 50, 1, 30000},
        {1, 2, 10, 200, 1, 100},
        {1, 3, 50, 500, 1, 30000},
        {2, 3, 100, 1000, 1, 50},
        {1, 2, 500, 2000, 1, 30000},
        {1, 3, 1000, 5000, 1, 1000},
        {2, 3, 2000, 10000, 1, 30000},
        {1, 2, 5000, 20000, 1, 100},
        {1, 2, 10000, 40000, 1, 30000},
        {1, 3, 20000, 60000, 1, 30},
        {1, 2, 50000, 90000, 1, 30000},
        {1, 2, 80000, 100000, 1, 30000},
    };

    for (int idx = 0; idx < 14; ++idx) {
        char name[32];
        snprintf(name, sizeof(name), "%d.in", idx + 1);
        FILE* fp = fopen(name, "w");
        if (!fp) { perror(name); return 1; }
        int g = randint(cfg[idx].minGroups, cfg[idx].maxGroups);
        for (int k = 0; k < g; ++k) {
            int n = randint(cfg[idx].minN, cfg[idx].maxN);
            vector<int> v = genRandom(n, cfg[idx].lo, cfg[idx].hi);
            writeGroup(fp, v);
        }
        fclose(fp);
    }

    // Cases 15..17: worst-case performance, 3 groups of n=1e5 each
    for (int idx = 15; idx <= 17; ++idx) {
        char name[32];
        snprintf(name, sizeof(name), "%d.in", idx);
        FILE* fp = fopen(name, "w");
        if (!fp) { perror(name); return 1; }
        int N = 100000;

        // Group A: strictly increasing -> LIS = len (min(N,30000))
        vector<int> A = genStrictIncreasing(N);
        writeGroup(fp, A);

        // Group B: strictly decreasing -> LIS = 1
        vector<int> B = genStrictDecreasing(N);
        writeGroup(fp, B);

        // Group C: small range (1..30) to create many conflicts / repeats
        int range = 30 - (idx - 15) * 5; // 30, 25, 20
        if (range < 5) range = 5;
        vector<int> C = genRandom(N, 1, range);
        writeGroup(fp, C);

        fclose(fp);
    }

    // Case 18: n = 1
    {
        FILE* fp = fopen("18.in", "w");
        vector<int> v = { randint(1, 30000) };
        writeGroup(fp, v);
        fclose(fp);
    }

    // Case 19: n = 2, two equal numbers
    {
        FILE* fp = fopen("19.in", "w");
        int x = randint(1, 30000);
        vector<int> v = { x, x };
        writeGroup(fp, v);
        fclose(fp);
    }

    // Case 20: n = 1e5 all identical
    {
        FILE* fp = fopen("20.in", "w");
        int N = 100000;
        int x = randint(1, 30000);
        vector<int> v(N, x);
        writeGroup(fp, v);
        fclose(fp);
    }

    return 0;
}
