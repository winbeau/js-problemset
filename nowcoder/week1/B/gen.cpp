#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

long long randRange(long long lo, long long hi) {
    uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

void writeCase(int idx, const vector<pair<long long, long long>> &intervals) {
    char name[32];
    snprintf(name, sizeof(name), "%d.in", idx);
    FILE *fp = fopen(name, "w");
    fprintf(fp, "%d\n", (int)intervals.size());
    for (auto &p : intervals) {
        fprintf(fp, "%lld %lld\n", p.first, p.second);
    }
    fclose(fp);
}

vector<pair<long long, long long>> genRandom(int n, long long lo, long long hi) {
    vector<pair<long long, long long>> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) {
        long long a = randRange(lo, hi);
        long long b = randRange(lo, hi);
        if (a > b) swap(a, b);
        v.emplace_back(a, b);
    }
    return v;
}

int main() {
    // Cases 1..14: random with varying scales
    // Size schedule across 1..14
    int sizes[14] = {1, 2, 5, 10, 50, 200, 1000, 3000, 8000, 20000, 40000, 60000, 80000, 100000};
    long long scales[14] = {
        10LL, 100LL, 1000LL, 10000LL, 100000LL, 1000000LL,
        10000000LL, 100000000LL, 1000000000LL,
        1000000000LL, 1000000000LL, 1000000000LL,
        1000000000LL, 1000000000LL
    };

    for (int i = 0; i < 14; ++i) {
        int n = sizes[i];
        long long s = scales[i];
        // allow negatives: range [-s, s]
        auto v = genRandom(n, -s, s);
        writeCase(i + 1, v);
    }

    // Case 15: n = 1e5, all intervals identical
    {
        int n = 100000;
        long long a = randRange(-1000000000LL, 1000000000LL);
        long long b = randRange(-1000000000LL, 1000000000LL);
        if (a > b) swap(a, b);
        vector<pair<long long, long long>> v(n, {a, b});
        writeCase(15, v);
    }

    // Case 16: n = 1e5, intervals递增相连 (consecutive, back-to-back)
    {
        int n = 100000;
        vector<pair<long long, long long>> v;
        v.reserve(n);
        long long start = -1000000000LL;
        // total span must fit in ±1e9. span = 2e9, n intervals -> each interval ~20000 long
        long long totalSpan = 2000000000LL;
        long long per = totalSpan / n; // 20000
        long long cur = start;
        for (int i = 0; i < n; ++i) {
            long long l = cur;
            long long r = cur + per;
            if (r > 1000000000LL) r = 1000000000LL;
            v.emplace_back(l, r);
            cur = r;
        }
        writeCase(16, v);
    }

    // Case 17: n = 1e5, all random with extreme value range ±1e9
    {
        int n = 100000;
        auto v = genRandom(n, -1000000000LL, 1000000000LL);
        writeCase(17, v);
    }

    // Case 18: n = 1 (extreme single interval)
    {
        long long a = randRange(-1000000000LL, 1000000000LL);
        long long b = randRange(-1000000000LL, 1000000000LL);
        if (a > b) swap(a, b);
        vector<pair<long long, long long>> v = {{a, b}};
        writeCase(18, v);
    }

    // Case 19: n = 2, two完全分离 intervals
    {
        // choose a < b < c < d so [a,b] and [c,d] disjoint
        long long a = -1000000000LL;
        long long b = -500000000LL;
        long long c = 500000000LL;
        long long d = 1000000000LL;
        vector<pair<long long, long long>> v = {{a, b}, {c, d}};
        writeCase(19, v);
    }

    // Case 20: n small, all intervals degenerate to points (l==r)
    {
        int n = 50;
        vector<pair<long long, long long>> v;
        v.reserve(n);
        for (int i = 0; i < n; ++i) {
            long long x = randRange(-1000000000LL, 1000000000LL);
            v.emplace_back(x, x);
        }
        writeCase(20, v);
    }

    return 0;
}
