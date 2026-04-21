#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int tc = 0;
    while (cin >> s) {
        bool ok = true;
        for (int l = 0, r = (int)s.size() - 1; l < r; ++l, --r) {
            if (s[l] != s[r]) {
                ok = false;
                break;
            }
        }
        cout << "case" << ++tc << ": " << (ok ? "yes" : "no") << '\n';
    }

    return 0;
}
