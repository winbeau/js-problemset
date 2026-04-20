#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    if (!getline(cin, line)) {
        return 0;
    }

    vector<int> v;
    v.reserve(1 << 16);

    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        if (token.empty()) continue;
        v.push_back(stoi(token));
    }

    sort(v.begin(), v.end());

    int n = (int)v.size();
    for (int i = 0; i < n; ++i) {
        cout << v[i];
        if ((i + 1) % 4 == 0 || i == n - 1) {
            cout << '\n';
        } else {
            cout << ' ';
        }
    }

    return 0;
}
