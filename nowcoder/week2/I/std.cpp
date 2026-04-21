#include <bits/stdc++.h>
using namespace std;

bool checkPrefix(const vector<int>& h, int len, int m, int k) {
    vector<int> vals(h.begin(), h.begin() + len);
    sort(vals.begin(), vals.end());
    for (int l = 0, r = 0; r < len; ++r) {
        while (vals[r] - vals[l] > k) ++l;
        if (r - l + 1 >= m) return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, hmax;
    if (!(cin >> n >> m >> k >> hmax)) return 0;
    vector<int> h(n);
    for (int i = 0; i < n; ++i) cin >> h[i];

    int lo = m, hi = n, ans = n;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (checkPrefix(h, mid, m, k)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    cout << ans << '\n';
    return 0;
}
