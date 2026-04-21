#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> pos(1000001, -1);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        pos[x] = i;
    }

    vector<int> lis;
    lis.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        if (x < 0 || x > 1000000 || pos[x] == -1) continue;
        int p = pos[x];
        auto it = lower_bound(lis.begin(), lis.end(), p);
        if (it == lis.end()) lis.push_back(p);
        else *it = p;
    }

    cout << lis.size() << '\n';
    return 0;
}
