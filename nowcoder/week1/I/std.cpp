#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        // 根据 Dilworth 定理，最少的非严格递减路队数 = 最长严格递增子序列长度
        // 使用 patience sorting + lower_bound 求严格递增 LIS 长度
        vector<int> tails;
        tails.reserve(n);
        for (int i = 0; i < n; ++i) {
            auto it = lower_bound(tails.begin(), tails.end(), a[i]);
            if (it == tails.end()) {
                tails.push_back(a[i]);
            } else {
                *it = a[i];
            }
        }

        cout << (int)tails.size() << '\n';
    }

    return 0;
}
