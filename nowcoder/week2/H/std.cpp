#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<pair<ll, ll>> segs(n);
    for (int i = 0; i < n; ++i) cin >> segs[i].first >> segs[i].second;
    sort(segs.begin(), segs.end());

    vector<pair<ll, ll>> merged;
    for (auto [l, r] : segs) {
        if (merged.empty() || l > merged.back().second) {
            merged.push_back({l, r});
        } else {
            merged.back().second = max(merged.back().second, r);
        }
    }

    auto can = [&](ll len) -> bool {
        int used = 0;
        int i = 0;
        while (i < (int)merged.size()) {
            ++used;
            ll coverL = merged[i].first;
            ll coverR = coverL + len;
            ++i;
            while (i < (int)merged.size() && merged[i].second <= coverR) ++i;
            if (used > k) return false;
        }
        return true;
    };

    ll lo = 0, hi = 0;
    for (auto [l, r] : merged) hi = max(hi, r - merged.front().first);
    while (lo < hi) {
        ll mid = (lo + hi) / 2;
        if (can(mid)) hi = mid;
        else lo = mid + 1;
    }
    cout << lo << '\n';
    return 0;
}
