#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<long long> makeIncreasing(int n, long long base, int step) {
    vector<long long> a(n);
    a[0] = base;
    for (int i = 1; i < n; ++i) a[i] = a[i - 1] + randInt(1, step);
    return a;
}

void writeSeq(ofstream& out, const vector<long long>& a) {
    out << a.size();
    for (long long v : a) out << ' ' << v;
    out << '\n';
}

void writeCase(int id, const vector<pair<vector<long long>, vector<long long>>>& tests) {
    ofstream out(to_string(id) + ".in");
    for (const auto& [a, b] : tests) {
        writeSeq(out, a);
        writeSeq(out, b);
    }
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int cases = randInt(1, 3);
        vector<pair<vector<long long>, vector<long long>>> tests;
        for (int t = 0; t < cases; ++t) {
            int n = randInt(1, 30);
            int m = randInt(1, 30);
            tests.push_back({makeIncreasing(n, randInt(1, 20), 5), makeIncreasing(m, randInt(1, 20), 5)});
        }
        writeCase(id, tests);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<pair<vector<long long>, vector<long long>>> tests;
        tests.push_back({makeIncreasing(20000, 1, 3), makeIncreasing(20000, 2, 3)});
        writeCase(id, tests);
    }

    writeCase(18, {{{5}, {7}}});
    writeCase(19, {{{11, 12, 13, 14}, {9, 10, 15, 16, 17}}});
    writeCase(20, {{{1, 100, 200}, {2, 3, 4, 5}}});
    return 0;
}
