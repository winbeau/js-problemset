#include <bits/stdc++.h>
using namespace std;

long long lowerMedian(const vector<long long>& a, const vector<long long>& b) {
    int total = (int)a.size() + (int)b.size();
    int need = (total - 1) / 2;
    int i = 0, j = 0;
    long long cur = 0;
    for (int step = 0; step <= need; ++step) {
        if (j == (int)b.size() || (i < (int)a.size() && a[i] <= b[j])) cur = a[i++];
        else cur = b[j++];
    }
    return cur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n) {
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        if (!(cin >> m)) break;
        vector<long long> b(m);
        for (int i = 0; i < m; ++i) cin >> b[i];
        cout << lowerMedian(a, b) << '\n';
    }

    return 0;
}
