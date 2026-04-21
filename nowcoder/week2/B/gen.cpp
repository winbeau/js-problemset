#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);
static const string ALPHA = "abcdefghijklmnopqrstuvwxyz0123456789";

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

char randChar() {
    return ALPHA[randInt(0, (int)ALPHA.size() - 1)];
}

string randomString(int len) {
    string s(len, 'a');
    for (int i = 0; i < len; ++i) s[i] = randChar();
    return s;
}

string makePalindrome(int len) {
    string s(len, 'a');
    for (int i = 0; i <= (len - 1) / 2; ++i) {
        char c = randChar();
        s[i] = s[len - 1 - i] = c;
    }
    return s;
}

void writeCase(int id, const vector<string>& lines) {
    ofstream out(to_string(id) + ".in");
    for (const string& line : lines) out << line << '\n';
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        int cnt = randInt(4, 12);
        vector<string> lines;
        for (int i = 0; i < cnt; ++i) {
            int len = randInt(1, 30);
            if (rng() & 1) lines.push_back(makePalindrome(len));
            else lines.push_back(randomString(len));
        }
        writeCase(id, lines);
    }

    for (int id = 15; id <= 17; ++id) {
        vector<string> lines;
        for (int i = 0; i < 40; ++i) {
            int len = randInt(80, 100);
            if (i % 3 == 0) lines.push_back(makePalindrome(len));
            else lines.push_back(randomString(len));
        }
        writeCase(id, lines);
    }

    writeCase(18, vector<string>{"a", "aa", "ab"});
    writeCase(19, vector<string>{makePalindrome(100)});
    writeCase(20, vector<string>{"level", "abcde", "noon", "haha"});
    return 0;
}
