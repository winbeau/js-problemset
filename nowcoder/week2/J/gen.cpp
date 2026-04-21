#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

void writeCase(int id, int n, int m) {
    vector<long long> a(n), b(m);
    for (int i = 0; i < n; ++i) a[i] = 1LL + 5000LL * i;
    for (int j = 0; j < m; ++j) b[j] = 2LL + j;
    int pi = randInt(0, n - 1);
    int pj = randInt(0, m - 1);
    long long x = a[pi] + b[pj];

    ofstream out(to_string(id) + ".in");
    out << n << ' ' << m << ' ' << x << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n';
    for (int j = 0; j < m; ++j) {
        if (j) out << ' ';
        out << b[j];
    }
    out << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) writeCase(id, randInt(5, 80), randInt(5, 80));
    for (int id = 15; id <= 17; ++id) writeCase(id, 2000, 2000);
    writeCase(18, 1, 1);

    ofstream out19("19.in");
    out19 << "4 5 6\n1 2 4 7\n3 4 6 8 9\n";

    ofstream out20("20.in");
    out20 << "3 4 5011\n1 5001 10001\n2 3 10 11\n";
    return 0;
}
