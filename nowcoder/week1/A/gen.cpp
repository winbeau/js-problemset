#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

long long randRange(long long lo, long long hi) {
    uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

// Write sequence to file id
void writeCase(int id, const vector<long long>& a) {
    ofstream ofs(to_string(id) + ".in");
    ofs << a.size() << "\n";
    for (size_t i = 0; i < a.size(); ++i) {
        if (i) ofs << ' ';
        ofs << a[i];
    }
    ofs << "\n";
}

// Construct a Jolly sequence of length n:
// Use a random permutation of 1..n-1 as absolute differences,
// with random signs, starting at a random base.
vector<long long> makeJolly(int n, long long base, long long cap = 1000000000LL) {
    vector<long long> diffs(n - 1);
    iota(diffs.begin(), diffs.end(), 1);
    shuffle(diffs.begin(), diffs.end(), rng);
    vector<long long> a(n);
    // We may need to choose signs so values stay within [-cap, cap].
    // Simple greedy: try random sign, if out of range flip.
    a[0] = base;
    for (int i = 1; i < n; ++i) {
        long long d = diffs[i - 1];
        int sign = (rng() & 1) ? 1 : -1;
        long long nxt = a[i - 1] + sign * d;
        if (nxt > cap || nxt < -cap) {
            sign = -sign;
            nxt = a[i - 1] + sign * d;
        }
        // If still out of range, clamp by flipping toward center
        if (nxt > cap || nxt < -cap) {
            // fallback: pick sign that moves toward zero
            if (a[i - 1] > 0) sign = -1; else sign = 1;
            nxt = a[i - 1] + sign * d;
        }
        a[i] = nxt;
    }
    return a;
}

vector<long long> makeRandom(int n, long long lo, long long hi) {
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) a[i] = randRange(lo, hi);
    return a;
}

int main() {
    // 1..14: random/mixed cases
    // Case 1: very small Jolly n=1
    {
        vector<long long> a = {randRange(-1000000000LL, 1000000000LL)};
        writeCase(1, a);
    }
    // Case 2: small n=2 Jolly
    {
        long long x = randRange(-100, 100);
        vector<long long> a = {x, x + 1};
        writeCase(2, a);
    }
    // Case 3: small n=5 Jolly
    {
        vector<long long> a = makeJolly(5, randRange(-50, 50));
        writeCase(3, a);
    }
    // Case 4: small n=9 Not Jolly (random tiny)
    {
        vector<long long> a = makeRandom(9, -20, 20);
        writeCase(4, a);
    }
    // Case 5: n=8 Jolly with moderate values
    {
        vector<long long> a = makeJolly(8, randRange(-10000, 10000));
        writeCase(5, a);
    }
    // Case 6: n=50 random (likely Not Jolly)
    {
        vector<long long> a = makeRandom(50, -1000, 1000);
        writeCase(6, a);
    }
    // Case 7: n=100 Jolly with larger magnitude
    {
        vector<long long> a = makeJolly(100, randRange(-500000, 500000));
        writeCase(7, a);
    }
    // Case 8: n=200 random (Not Jolly likely)
    {
        vector<long long> a = makeRandom(200, -1000000000LL, 1000000000LL);
        writeCase(8, a);
    }
    // Case 9: n=3 Jolly
    {
        vector<long long> a = makeJolly(3, randRange(-1000000000LL, 1000000000LL));
        writeCase(9, a);
    }
    // Case 10: n=7 with duplicates -> Not Jolly
    {
        vector<long long> a(7);
        for (int i = 0; i < 7; ++i) a[i] = (i % 2 == 0) ? 5 : 5;
        writeCase(10, a);
    }
    // Case 11: n=500 Jolly
    {
        vector<long long> a = makeJolly(500, randRange(-100000000LL, 100000000LL));
        writeCase(11, a);
    }
    // Case 12: n=1000 random
    {
        vector<long long> a = makeRandom(1000, -1000000000LL, 1000000000LL);
        writeCase(12, a);
    }
    // Case 13: n=6 Not Jolly constructed (diffs 1,1,2,3,4)
    {
        vector<long long> a = {0, 1, 2, 4, 7, 11};
        writeCase(13, a);
    }
    // Case 14: n=1500 Jolly
    {
        vector<long long> a = makeJolly(1500, randRange(-100000000LL, 100000000LL));
        writeCase(14, a);
    }

    // 15..17: stress at upper bound n=2999
    // Case 15: Jolly with large |a_i| via cumulative construction
    {
        vector<long long> a = makeJolly(2999, randRange(-100000000LL, 100000000LL));
        writeCase(15, a);
    }
    // Case 16: Not-Jolly random extreme
    {
        vector<long long> a = makeRandom(2999, -1000000000LL, 1000000000LL);
        writeCase(16, a);
    }
    // Case 17: Nearly-but-not-quite Jolly:
    // Build Jolly then swap two diffs so one repeats and one missing.
    {
        int n = 2999;
        vector<long long> diffs(n - 1);
        iota(diffs.begin(), diffs.end(), 1);
        shuffle(diffs.begin(), diffs.end(), rng);
        // Corrupt: duplicate one value, remove another.
        int i1 = (int)randRange(0, n - 2);
        int i2 = (int)randRange(0, n - 2);
        while (i2 == i1) i2 = (int)randRange(0, n - 2);
        diffs[i2] = diffs[i1];
        vector<long long> a(n);
        long long cap = 1000000000LL;
        a[0] = randRange(-100000000LL, 100000000LL);
        for (int i = 1; i < n; ++i) {
            long long d = diffs[i - 1];
            int sign = (rng() & 1) ? 1 : -1;
            long long nxt = a[i - 1] + sign * d;
            if (nxt > cap || nxt < -cap) {
                sign = -sign;
                nxt = a[i - 1] + sign * d;
            }
            if (nxt > cap || nxt < -cap) {
                if (a[i - 1] > 0) sign = -1; else sign = 1;
                nxt = a[i - 1] + sign * d;
            }
            a[i] = nxt;
        }
        writeCase(17, a);
    }

    // 18..20: extreme edge cases
    // Case 18: n=1 with value near 1e9 boundary
    {
        vector<long long> a = {1000000000LL};
        writeCase(18, a);
    }
    // Case 19: n=2 identical values (Not Jolly)
    {
        vector<long long> a = {-1000000000LL, -1000000000LL};
        writeCase(19, a);
    }
    // Case 20: n=2 with difference exactly 1 (Jolly), near boundary
    {
        vector<long long> a = {1000000000LL, 999999999LL};
        writeCase(20, a);
    }

    return 0;
}
