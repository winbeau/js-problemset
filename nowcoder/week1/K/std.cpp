#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    while (cin >> N) {
        if (N == 0) break;
        // consume rest of the line after N
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<string> tmpl(N);
        for (int i = 0; i < N; ++i) {
            string line;
            getline(cin, line);
            // remove possible trailing '\r' from Windows line endings
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if ((int)line.size() < N) line.resize(N, ' ');
            else if ((int)line.size() > N) line.resize(N);
            tmpl[i] = line;
        }

        int Q;
        cin >> Q;

        // compute size S = N^Q
        long long S = 1;
        for (int i = 0; i < Q; ++i) S *= N;

        // For each cell (r, c) in the S x S result, decompose coordinates
        // level by level. Determine if any level has a space -> output ' ',
        // otherwise output the template's non-space character at level 1.
        string out;
        out.reserve((size_t)(S * (S + 1)));

        for (long long r = 0; r < S; ++r) {
            for (long long c = 0; c < S; ++c) {
                long long rr = r, cc = c;
                long long block = S;
                bool isSpace = false;
                char ch = ' ';
                // Peel from top level down to level 1.
                // At each step: which cell (tr, tc) of the template we are in.
                for (int k = 0; k < Q; ++k) {
                    block /= N; // size of sub-block at this level
                    long long tr = rr / block;
                    long long tc = cc / block;
                    rr %= block;
                    cc %= block;
                    char t = tmpl[(int)tr][(int)tc];
                    if (t == ' ') { isSpace = true; break; }
                    ch = t;
                }
                out.push_back(isSpace ? ' ' : ch);
            }
            out.push_back('\n');
        }

        cout << out;
    }
    return 0;
}
