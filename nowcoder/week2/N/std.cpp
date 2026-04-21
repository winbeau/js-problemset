#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int n;
    while (cin >> s >> n) {
        for (int t = 0; t < n; ++t) {
            string cmd;
            cin >> cmd;
            int type = cmd[0] - '0';
            int i = cmd[1] - '0';
            int len = cmd[2] - '0';
            if (type == 0) {
                reverse(s.begin() + i, s.begin() + i + len);
            } else {
                string rep = cmd.substr(3);
                s.replace(i, len, rep);
            }
            cout << s << '\n';
        }
    }

    return 0;
}
