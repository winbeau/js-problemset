#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int searchCount(int n, int pos) {
    int cnt = 0;
    int L = 0, R = n - 1;
    while (true) {
        ++cnt;
        int mid = (L + R) / 2;
        if (mid == pos) return cnt;
        if (pos < mid) R = mid;
        else L = mid + 1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        ll X;
        cin >> X;

        vector<bool> prefixStrict(n), prefixLow(n), suffixStrict(n);
        vector<ll> prefixMinRoom(n, (ll)4e18), suffixMaxNeed(n, (ll)-4e18);

        ll last = (ll)-4e18;
        bool ok = true;
        ll best = (ll)4e18;
        for (int i = 0; i < n; ++i) {
            if (a[i] != -1) {
                if (a[i] <= last) ok = false;
                last = a[i];
                if (a[i] < i + 1) prefixLow[i] = false;
                else prefixLow[i] = true;
                best = min(best, X + i - a[i]);
            } else {
                prefixLow[i] = true;
            }
            prefixStrict[i] = ok;
            if (i) prefixLow[i] = prefixLow[i] && prefixLow[i - 1];
            prefixMinRoom[i] = best;
        }

        ll nxt = (ll)4e18;
        ok = true;
        ll need = (ll)-4e18;
        for (int i = n - 1; i >= 0; --i) {
            if (a[i] != -1) {
                if (a[i] >= nxt) ok = false;
                nxt = a[i];
                need = max(need, X + i - a[i]);
            }
            suffixStrict[i] = ok;
            suffixMaxNeed[i] = need;
        }

        int ans = 0;
        for (int p = 0; p < n; ++p) {
            if (a[p] != -1 && a[p] != X) continue;
            if ((ll)p > X - 1) continue;

            bool leftStrict = (p == 0) || prefixStrict[p - 1];
            bool leftLow = (p == 0) || prefixLow[p - 1];
            bool leftRoom = (p == 0) || ((ll)p <= prefixMinRoom[p - 1]);
            bool rightStrict = (p == n - 1) || suffixStrict[p + 1];
            bool rightNeed = (p == n - 1) || ((ll)p >= suffixMaxNeed[p + 1]);

            if (leftStrict && leftLow && leftRoom && rightStrict && rightNeed) {
                ans = max(ans, searchCount(n, p));
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
