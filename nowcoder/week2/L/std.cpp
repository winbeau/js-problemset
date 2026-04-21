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
        vector<int> b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        if (is_sorted(a.begin(), a.end())) {
            cout << "Yes\n";
            continue;
        }
        bool has0 = false, has1 = false;
        for (int x : b) {
            has0 |= (x == 0);
            has1 |= (x == 1);
        }
        cout << ((has0 && has1) ? "Yes" : "No") << '\n';
    }

    return 0;
}
