#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        if (n & 1) {
            cout << a[n / 2] << '\n';
        } else {
            long long sum = a[n / 2 - 1] + a[n / 2];
            if (sum % 2 == 0) cout << sum / 2 << '\n';
            else cout << sum / 2 << ".5\n";
        }
    }

    return 0;
}
