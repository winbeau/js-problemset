#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

void writeCase(int id, int n, int m, int k, int hmax, const vector<int>& h) {
    ofstream out(to_string(id) + ".in");
    out << n << ' ' << m << ' ' << k << ' ' << hmax << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) out << ' ';
        out << h[i];
    }
    out << '\n';
}

vector<int> makeHeights(int n, int m, int k, int hmax) {
    vector<int> h(n);
    for (int i = 0; i < n; ++i) h[i] = randInt(1, hmax);
    int prefix = randInt(m, n);
    int start = prefix - m;
    int base = randInt(1, max(1, hmax - max(k, 1)));
    for (int i = start; i < prefix; ++i) h[i] = base + randInt(0, k);
    return h;
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(10, 60);
        int m = randInt(1, n);
        int k = randInt(0, 15);
        int hmax = 200;
        writeCase(id, n, m, k, hmax, makeHeights(n, m, k, hmax));
    }

    for (int id = 15; id <= 17; ++id) {
        int n = 5000;
        int m = randInt(50, 200);
        int k = randInt(0, 30);
        int hmax = 100000;
        writeCase(id, n, m, k, hmax, makeHeights(n, m, k, hmax));
    }

    writeCase(18, 1, 1, 0, 10, vector<int>{7});
    writeCase(19, 5, 3, 2, 10, vector<int>{3, 1, 4, 1, 5});
    writeCase(20, 8, 4, 0, 20, vector<int>{9, 2, 2, 2, 2, 10, 11, 12});
    return 0;
}
