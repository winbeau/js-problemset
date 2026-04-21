#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

vector<int> makeSortedList(int n, int base, int step, bool allowDup) {
    vector<int> a(n);
    a[0] = base;
    for (int i = 1; i < n; ++i) {
        a[i] = a[i - 1] + (allowDup ? randInt(0, step) : randInt(1, step));
    }
    return a;
}

void writeLine(ofstream& out, const vector<int>& a) {
    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n';
}

void writeCase(int id, const vector<int>& a, const vector<int>& b) {
    ofstream out(to_string(id) + ".in");
    writeLine(out, a);
    writeLine(out, b);
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int n = randInt(5, 50);
        int m = randInt(5, 50);
        vector<int> a = makeSortedList(n, randInt(1, 10), 5, true);
        vector<int> b = makeSortedList(m, randInt(1, 10), 5, true);
        writeCase(id, a, b);
    }

    for (int id = 15; id <= 17; ++id) {
        int n = 2000;
        int m = 2000;
        vector<int> a = makeSortedList(n, 1, 3, true);
        vector<int> b = makeSortedList(m, 2, 3, true);
        writeCase(id, a, b);
    }

    writeCase(18, vector<int>{1, 2, 3}, vector<int>{4, 5, 6});
    writeCase(19, vector<int>{1, 1, 2, 3}, vector<int>{1, 1, 1, 2, 2, 3});
    writeCase(20, vector<int>{1, 2, 3, 4, 5}, vector<int>{3, 4, 5, 6, 7});
    return 0;
}
