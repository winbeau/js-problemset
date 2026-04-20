#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n == 1) {
        cout << "Jolly" << "\n";
        return 0;
    }

    vector<long long> d(n - 1);
    for (int i = 0; i + 1 < n; ++i) {
        d[i] = llabs(a[i + 1] - a[i]);
    }
    sort(d.begin(), d.end());

    bool jolly = true;
    for (int i = 0; i < n - 1; ++i) {
        if (d[i] != (long long)(i + 1)) {
            jolly = false;
            break;
        }
    }

    cout << (jolly ? "Jolly" : "Not jolly") << "\n";
    return 0;
}
