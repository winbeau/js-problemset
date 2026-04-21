#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

void writeCase(int id, const vector<pair<vector<int>, int>>& tests) {
    ofstream out(to_string(id) + ".in");
    for (const auto& [a, d] : tests) {
        out << a.size() << '\n';
        for (int i = 0; i < (int)a.size(); ++i) {
            if (i) out << ' ';
            out << a[i];
        }
        out << '\n' << d << '\n';
    }
}

vector<int> randomVals(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = randInt(1, 1000000);
    return a;
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int cases = randInt(1, 3);
        vector<pair<vector<int>, int>> tests;
        for (int t = 0; t < cases; ++t) {
            int n = randInt(1, 60);
            int maxDepth = 0;
            while ((1 << maxDepth) <= n) ++maxDepth;
            tests.push_back({randomVals(n), randInt(1, maxDepth + 2)});
        }
        writeCase(id, tests);
    }

    for (int id = 15; id <= 17; ++id) {
        writeCase(id, {{randomVals(1000), randInt(1, 12)}});
    }

    writeCase(18, {{{42}, 1}});
    writeCase(19, {{{1, 2, 3, 4}, 2}});
    writeCase(20, {{{10, 20, 30, 40, 50, 60, 70}, 4}});
    return 0;
}
