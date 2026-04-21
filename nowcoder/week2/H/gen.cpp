#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<pair<int, int>> makeIntervals(int n) {
    vector<pair<int, int>> segs;
    int cur = 1;
    for (int i = 0; i < n; ++i) {
        cur += randInt(0, 3);
        int l = cur;
        int r = l + randInt(1, 6);
        segs.push_back({l, r});
        cur = r - randInt(0, 1);
    }
    shuffle(segs.begin(), segs.end(), rng);
    return segs;
}

void writeCase(int id, const vector<pair<int, int>>& segs, int k) {
    ofstream out(to_string(id) + ".in");
    out << segs.size() << ' ' << k << '\n';
    for (auto [l, r] : segs) out << l << ' ' << r << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(4, 20);
        writeCase(id, makeIntervals(n), randInt(1, n));
    }

    for (int id = 15; id <= 17; ++id) {
        int n = 1000;
        writeCase(id, makeIntervals(n), randInt(1, 60));
    }

    writeCase(18, {{1, 2}}, 1);
    writeCase(19, {{1, 2}, {2, 4}, {5, 6}, {7, 8}, {7, 9}}, 2);
    writeCase(20, {{1, 3}, {10, 12}, {20, 24}}, 3);
    return 0;
}
