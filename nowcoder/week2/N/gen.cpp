#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240504);
static const string ALPHA = "abcdefghijklmnopqrstuvwxyz";

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

string randomString(int len) {
    string s(len, 'a');
    for (int i = 0; i < len; ++i) s[i] = ALPHA[randInt(0, 25)];
    return s;
}

string makeReplaceCmd(int i, int len) {
    string rep = randomString(len);
    return string("1") + char('0' + i) + char('0' + len) + rep;
}

string makeReverseCmd(int i, int len) {
    return string("0") + char('0' + i) + char('0' + len);
}

void writeDataset(ofstream& out, string s, int q) {
    out << s << '\n' << q << '\n';
    for (int t = 0; t < q; ++t) {
        int i = randInt(0, (int)s.size() - 1);
        int len = randInt(1, min(9, (int)s.size() - i));
        string cmd;
        if ((rng() & 1) == 0) {
            cmd = makeReverseCmd(i, len);
            reverse(s.begin() + i, s.begin() + i + len);
        } else {
            cmd = makeReplaceCmd(i, len);
            s.replace(i, len, cmd.substr(3));
        }
        out << cmd << '\n';
    }
}

int main() {
    for (int id = 1; id <= 14; ++id) {
        ofstream out(to_string(id) + ".in");
        int cases = randInt(1, 3);
        for (int c = 0; c < cases; ++c) {
            writeDataset(out, randomString(randInt(3, 9)), randInt(1, 6));
        }
    }

    for (int id = 15; id <= 17; ++id) {
        ofstream out(to_string(id) + ".in");
        writeDataset(out, randomString(9), 40);
    }

    ofstream out18("18.in");
    out18 << "a\n1\n001\n";

    ofstream out19("19.in");
    out19 << "bac\n2\n003\n112as\n";

    ofstream out20("20.in");
    out20 << "abcdef\n3\n013\n122zz\n031\n";
    return 0;
}
