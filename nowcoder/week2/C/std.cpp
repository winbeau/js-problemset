#include <bits/stdc++.h>
using namespace std;

vector<long long> parseLine(const string& line) {
    stringstream ss(line);
    vector<long long> vals;
    long long x;
    while (ss >> x) vals.push_back(x);
    return vals;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line1, line2;
    bool firstCase = true;
    while (getline(cin, line1)) {
        if (line1.empty()) continue;
        if (!getline(cin, line2)) break;
        vector<long long> a = parseLine(line1);
        vector<long long> b = parseLine(line2);
        vector<long long> ans;
        int i = 0, j = 0;
        while (i < (int)a.size() && j < (int)b.size()) {
            if (a[i] == b[j]) {
                ans.push_back(a[i]);
                ++i;
                ++j;
            } else if (a[i] < b[j]) {
                ++i;
            } else {
                ++j;
            }
        }
        if (!firstCase) cout << '\n';
        firstCase = false;
        for (int k = 0; k < (int)ans.size(); ++k) {
            if (k) cout << ' ';
            cout << ans[k];
        }
        cout << '\n';
    }

    return 0;
}
