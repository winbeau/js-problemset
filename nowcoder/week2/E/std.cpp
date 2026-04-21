#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cin >> m;
    vector<long long> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];

    vector<long long> c;
    c.reserve(n + m);
    int i = 0, j = 0;
    while (i < n || j < m) {
        if (j == m || (i < n && a[i] <= b[j])) c.push_back(a[i++]);
        else c.push_back(b[j++]);
    }

    for (int k = 0; k < (int)c.size(); ++k) {
        if (k) cout << ' ';
        cout << c[k];
    }
    cout << '\n';
    return 0;
}
