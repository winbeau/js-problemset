#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<int> makeSorted(int n) {
    vector<int> a(n);
    a[0] = randInt(0, 5);
    for (int i = 1; i < n; ++i) a[i] = a[i - 1] + randInt(0, 6);
    return a;
}

void writeCase(int id, const vector<vector<int>>& tests) {
    ofstream out(to_string(id) + ".in");
    out << tests.size() << '\n';
    for (const auto& a : tests) {
        out << a.size() << '\n';
        for (int i = 0; i < (int)a.size(); ++i) {
            if (i) out << ' ';
            out << a[i];
        }
        out << '\n';
    }
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int T = randInt(1, 5);
        vector<vector<int>> tests;
        for (int t = 0; t < T; ++t) tests.push_back(makeSorted(randInt(2, 20)));
        writeCase(id, tests);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<vector<int>> tests;
        for (int t = 0; t < 5; ++t) tests.push_back(makeSorted(randInt(80, 99)));
        writeCase(id, tests);
    }

    writeCase(18, {{1, 2}});
    writeCase(19, {{1, 1, 2, 2}, {1, 1, 2, 2, 3}});
    writeCase(20, {{0, 0, 0}, {2, 4, 6, 8}});
    return 0;
}
