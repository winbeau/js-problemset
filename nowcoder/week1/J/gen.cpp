#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

static int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

// Full valid alphabet: lowercase letters + underscore
static const string ALPHA = "abcdefghijklmnopqrstuvwxyz_";

static char randChar() {
    return ALPHA[randInt(0, (int)ALPHA.size() - 1)];
}

static string randString(int len) {
    string s;
    s.reserve(len);
    for (int i = 0; i < len; i++) s.push_back(randChar());
    return s;
}

static void writeCase(ofstream& out, int k1, int k2, int k3, const string& cipher) {
    out << k1 << ' ' << k2 << ' ' << k3 << '\n';
    out << cipher << '\n';
}

static void writeTerminator(ofstream& out) {
    out << "0 0 0\n";
}

static void genRandom(const string& path, int maxGroups) {
    ofstream out(path);
    int groups = randInt(1, maxGroups);
    for (int i = 0; i < groups; i++) {
        int k1 = randInt(1, 100);
        int k2 = randInt(1, 100);
        int k3 = randInt(1, 100);
        int len = randInt(1, 80);
        string s = randString(len);
        writeCase(out, k1, k2, k3, s);
    }
    writeTerminator(out);
}

static void genStress(const string& path) {
    ofstream out(path);
    for (int i = 0; i < 20; i++) {
        string s = randString(80);
        writeCase(out, 100, 100, 100, s);
    }
    writeTerminator(out);
}

static void genSingleChar(const string& path) {
    ofstream out(path);
    int k1 = randInt(1, 100);
    int k2 = randInt(1, 100);
    int k3 = randInt(1, 100);
    string s(1, randChar());
    writeCase(out, k1, k2, k3, s);
    writeTerminator(out);
}

static void genAllUnderscore(const string& path) {
    ofstream out(path);
    int k1 = randInt(1, 100);
    int k2 = randInt(1, 100);
    int k3 = randInt(1, 100);
    string s(80, '_');
    writeCase(out, k1, k2, k3, s);
    writeTerminator(out);
}

static void genAllFirstGroup(const string& path) {
    ofstream out(path);
    int k1 = randInt(1, 100);
    int k2 = randInt(1, 100);
    int k3 = randInt(1, 100);
    // length up to 80, only letters a..i
    int len = 80;
    string s;
    s.reserve(len);
    for (int i = 0; i < len; i++) {
        s.push_back('a' + (i % 9));
    }
    writeCase(out, k1, k2, k3, s);
    writeTerminator(out);
}

int main() {
    string base = "/home/winbeau/Projects/js-problemset/nowcoder/week1/J/";

    // 1..14 random
    for (int i = 1; i <= 14; i++) {
        string p = base + to_string(i) + ".in";
        genRandom(p, 20);
    }

    // 15..17 stress: 20 groups, len 80, k=100
    for (int i = 15; i <= 17; i++) {
        string p = base + to_string(i) + ".in";
        genStress(p);
    }

    // 18: single length-1 message
    genSingleChar(base + "18.in");
    // 19: single all-underscore length 80
    genAllUnderscore(base + "19.in");
    // 20: single all-first-group
    genAllFirstGroup(base + "20.in");

    return 0;
}
