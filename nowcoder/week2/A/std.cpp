#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    long long x;
    cin >> x;

    int left = 0, right = n - 1, cnt = 0;
    while (left <= right) {
        ++cnt;
        int mid = (left + right) / 2;
        if (a[mid] == x) {
            cout << cnt << '\n';
            return 0;
        }
        if (a[mid] < x) left = mid + 1;
        else right = mid - 1;
    }

    return 0;
}
