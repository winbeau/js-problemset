#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<long long> xs(n), ys(n);
        for (int i = 0; i < n; ++i) {
            cin >> xs[i] >> ys[i];
        }

        sort(xs.begin(), xs.end());
        sort(ys.begin(), ys.end());

        vector<long long> zs(n);
        for (int i = 0; i < n; ++i) {
            zs[i] = xs[i] - i;
        }
        sort(zs.begin(), zs.end());

        long long medY = ys[n / 2];
        long long medZ = zs[n / 2];

        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += llabs(ys[i] - medY);
            ans += llabs(zs[i] - medZ);
        }

        cout << ans << '\n';
    }

    return 0;
}
