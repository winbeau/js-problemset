#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<int> makeSortedArray(int n, int maxStep, bool allowDup) {
    vector<int> a(n);
    a[0] = randInt(1, 10);
    for (int i = 1; i < n; ++i) {
        int step = allowDup ? randInt(0, maxStep) : randInt(1, maxStep);
        a[i] = a[i - 1] + step;
    }
    return a;
}

void writeCase(int id, const vector<int>& a, int target) {
    ofstream out(to_string(id) + ".in");
    out << a.size() << '\n';
    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n' << target << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(1, 200);
        vector<int> a = makeSortedArray(n, 10, true);
        int target = a[randInt(0, n - 1)];
        writeCase(id, a, target);
    }

    for (int id = 15; id <= 17; ++id) {
        int n = 10000;
        vector<int> a = makeSortedArray(n, 3, true);
        int target = a[randInt(0, n - 1)];
        writeCase(id, a, target);
    }

    writeCase(18, vector<int>{42}, 42);
    writeCase(19, vector<int>{1, 1, 1, 1, 1}, 1);
    writeCase(20, vector<int>{2, 4, 8, 16, 32, 64, 128}, 128);
    return 0;
}
