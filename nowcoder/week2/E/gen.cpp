#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<int> makeSorted(int n, int base) {
    vector<int> a(n);
    a[0] = base;
    for (int i = 1; i < n; ++i) a[i] = a[i - 1] + randInt(1, 5);
    return a;
}

void writeCase(int id, const vector<int>& a, const vector<int>& b) {
    ofstream out(to_string(id) + ".in");
    out << a.size() << '\n';
    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n' << b.size() << '\n';
    for (int i = 0; i < (int)b.size(); ++i) {
        if (i) out << ' ';
        out << b[i];
    }
    out << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        vector<int> a = makeSorted(randInt(1, 20), randInt(1, 10));
        vector<int> b = makeSorted(randInt(1, 20), randInt(1, 10));
        writeCase(id, a, b);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<int> a = makeSorted(100, 1);
        vector<int> b = makeSorted(100, 2);
        writeCase(id, a, b);
    }

    writeCase(18, vector<int>{1}, vector<int>{2});
    writeCase(19, vector<int>{2, 4, 6, 8}, vector<int>{3, 5, 7});
    writeCase(20, vector<int>{1, 3, 5}, vector<int>{1, 2, 4, 6});
    return 0;
}
