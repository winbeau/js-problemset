#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    long long x;
    if (!(cin >> n >> m >> x)) return 0;
    vector<long long> a(n), b(m);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) cin >> b[i];

    int i = 0, j = m - 1;
    while (i < n && j >= 0) {
        long long sum = a[i] + b[j];
        if (sum == x) {
            cout << i << ' ' << j << '\n';
            return 0;
        }
        if (sum < x) ++i;
        else --j;
    }

    return 0;
}
