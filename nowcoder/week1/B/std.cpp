#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    long long minL = LLONG_MAX;
    long long maxR = LLONG_MIN;

    for (int i = 0; i < n; ++i) {
        long long l, r;
        cin >> l >> r;
        if (l < minL) minL = l;
        if (r > maxR) maxR = r;
    }

    cout << (maxR - minL) << '\n';
    return 0;
}
