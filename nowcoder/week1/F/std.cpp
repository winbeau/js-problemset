#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<pair<long long, long long>> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i].first >> p[i].second;
        }
        // 邻项交换分析：按 (a - b) 降序排序
        sort(p.begin(), p.end(), [](const pair<long long, long long>& x,
                                    const pair<long long, long long>& y) {
            return (x.first - x.second) > (y.first - y.second);
        });
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            long long a = p[i].first;
            long long b = p[i].second;
            ans += (long long)i * a + (long long)(n - 1 - i) * b;
        }
        cout << ans << '\n';
    }
    return 0;
}
