#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<int> randomDistinct(int n) {
    unordered_set<int> used;
    used.reserve(n * 2);
    vector<int> vals;
    vals.reserve(n);
    while ((int)vals.size() < n) {
        int v = randInt(1, 1000000);
        if (used.insert(v).second) vals.push_back(v);
    }
    return vals;
}

void writeCase(int id, const vector<int>& a, const vector<int>& b) {
    ofstream out(to_string(id) + ".in");
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

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(10, 2000);
        vector<int> a = randomDistinct(n);
        vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            if (rng() % 100 < 70) b[i] = a[randInt(0, n - 1)];
            else b[i] = randInt(1, 1000000);
        }
        writeCase(id, a, b);
    }

    {
        int n = 50000;
        vector<int> a = randomDistinct(n);
        writeCase(15, a, a);
    }
    {
        int n = 50000;
        vector<int> a = randomDistinct(n);
        vector<int> b = a;
        reverse(b.begin(), b.end());
        writeCase(16, a, b);
    }
    {
        int n = 50000;
        vector<int> a = randomDistinct(n);
        vector<int> b(n);
        for (int i = 0; i < n; ++i) b[i] = a[randInt(0, n - 1)];
        writeCase(17, a, b);
    }

    writeCase(18, vector<int>{7}, vector<int>{7});
    writeCase(19, vector<int>{1, 2, 3, 4, 5}, vector<int>{1, 2, 3, 4, 5});
    writeCase(20, vector<int>{1, 3, 5, 7, 9}, vector<int>{9, 1, 7, 3, 5});
    return 0;
}
