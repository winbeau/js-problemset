#include <bits/stdc++.h>
using namespace std;

int groupOf(char c) {
    if (c >= 'a' && c <= 'i') return 0;
    if (c >= 'j' && c <= 'r') return 1;
    // [s-z] and '_'
    return 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k[3];
    while (cin >> k[0] >> k[1] >> k[2]) {
        if (k[0] == 0 && k[1] == 0 && k[2] == 0) break;
        // consume newline after the integers
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string line;
        if (!getline(cin, line)) break;

        vector<int> pos[3];
        vector<char> chr[3];
        for (int i = 0; i < (int)line.size(); ++i) {
            int g = groupOf(line[i]);
            pos[g].push_back(i);
            chr[g].push_back(line[i]);
        }

        string result = line;
        for (int g = 0; g < 3; ++g) {
            int len = (int)chr[g].size();
            if (len == 0) continue;
            int shift = k[g] % len;
            // "向右循环移动 k 位": new[i] = old[(i - k + len) % len]
            vector<char> shifted(len);
            for (int i = 0; i < len; ++i) {
                shifted[i] = chr[g][((i - shift) % len + len) % len];
            }
            for (int i = 0; i < len; ++i) {
                result[pos[g][i]] = shifted[i];
            }
        }

        cout << result << '\n';
    }

    return 0;
}
