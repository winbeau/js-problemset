#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<long long> buildFull(int n) {
    vector<long long> a(n);
    a[0] = randInt(1, 10);
    for (int i = 1; i < n; ++i) a[i] = a[i - 1] + randInt(1, 5);
    return a;
}

void writeCase(int id, const vector<tuple<vector<long long>, long long>>& tests) {
    ofstream out(to_string(id) + ".in");
    for (const auto& [arr, x] : tests) {
        out << arr.size() << '\n';
        for (int i = 0; i < (int)arr.size(); ++i) {
            if (i) out << ' ';
            out << arr[i];
        }
        out << '\n' << x << '\n';
    }
}

tuple<vector<long long>, long long> makeMaskedCase(int n) {
    vector<long long> full = buildFull(n);
    int p = randInt(0, n - 1);
    long long x = full[p];
    vector<long long> masked = full;
    for (int i = 0; i < n; ++i) {
        if (rng() % 100 < 60) masked[i] = -1;
    }
    masked[p] = (rng() & 1) ? x : -1;
    return {masked, x};
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int cases = randInt(1, 3);
        vector<tuple<vector<long long>, long long>> tests;
        for (int t = 0; t < cases; ++t) tests.push_back(makeMaskedCase(randInt(5, 50)));
        writeCase(id, tests);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<tuple<vector<long long>, long long>> tests;
        tests.push_back(makeMaskedCase(10000));
        writeCase(id, tests);
    }

    writeCase(18, {{{1, 2, 3, 4, 5}, 3}});
    writeCase(19, {{{1, -1, -1, -1, 5}, 3}});
    writeCase(20, {{{-1, -1, -1, -1, -1}, 3}});
    return 0;
}
