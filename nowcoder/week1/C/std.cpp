#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int prev = 0;
    int cur = 0;
    int best = 0;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        if (i == 0 || x > prev) {
            ++cur;
        } else {
            cur = 1;
        }
        if (cur > best) best = cur;
        prev = x;
    }

    cout << best << '\n';
    return 0;
}
