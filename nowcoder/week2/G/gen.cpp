#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

void writeCase(int id, const vector<int>& a, int k) {
    ofstream out(to_string(id) + ".in");
    out << a.size() << '\n';
    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n' << k << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(5, 40);
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = randInt(1, 40);
        vector<int> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        writeCase(id, a, randInt(1, (int)vals.size()));
    }

    for (int id = 15; id <= 17; ++id) {
        int n = 1000;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = randInt(1, 300);
        vector<int> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        writeCase(id, a, randInt(1, (int)vals.size()));
    }

    writeCase(18, vector<int>{7}, 1);
    writeCase(19, vector<int>{2, 1, 3, 5, 2, 2}, 3);
    writeCase(20, vector<int>{5, 5, 5, 5, 6, 7}, 2);
    return 0;
}
