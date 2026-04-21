#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        int d;
        cin >> d;
        long long start = (d >= 63) ? (long long)4e18 : ((1LL << (d - 1)) - 1);
        if (d <= 0 || start >= n) {
            cout << "EMPTY\n";
            continue;
        }
        long long end = min<long long>(n - 1, (1LL << d) - 2);
        for (long long i = start; i <= end; ++i) {
            if (i > start) cout << ' ';
            cout << a[i];
        }
        cout << '\n';
    }

    return 0;
}
