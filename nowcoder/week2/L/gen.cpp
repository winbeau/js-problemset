#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

void writeCase(int id, const vector<pair<vector<int>, vector<int>>>& tests) {
    ofstream out(to_string(id) + ".in");
    out << tests.size() << '\n';
    for (const auto& [a, b] : tests) {
        out << a.size() << '\n';
        for (int i = 0; i < (int)a.size(); ++i) {
            if (i) out << ' ';
            out << a[i];
        }
        out << '\n';
        for (int i = 0; i < (int)b.size(); ++i) {
            if (i) out << ' ';
            out << b[i];
        }
        out << '\n';
    }
}

pair<vector<int>, vector<int>> makeRandomCase(int n) {
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) a[i] = randInt(1, 1000);
    for (int i = 0; i < n; ++i) b[i] = randInt(0, 1);
    return {a, b};
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int T = randInt(1, 4);
        vector<pair<vector<int>, vector<int>>> tests;
        for (int t = 0; t < T; ++t) tests.push_back(makeRandomCase(randInt(3, 20)));
        writeCase(id, tests);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<pair<vector<int>, vector<int>>> tests;
        tests.push_back(makeRandomCase(100000));
        writeCase(id, tests);
    }

    writeCase(18, {{{1, 2, 3}, {0, 0, 0}}});
    writeCase(19, {{{3, 2, 1}, {0, 0, 0}}});
    writeCase(20, {{{1, 8, 9, 4, 7}, {0, 1, 1, 0, 0}}});
    return 0;
}
