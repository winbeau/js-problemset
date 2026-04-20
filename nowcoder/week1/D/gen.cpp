#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

static int randint(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

static string fmtTime(int h, int m) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
    return string(buf);
}

// Generate a random (arrive, leave) pair in minutes, with arrive < leave,
// both within [0, 23*60+59] = [0, 1439].
// mode controls how the interval is chosen:
//   0: fully random, arrive < leave
//   1: entirely outside lunch (12:00-13:00) and dinner (17:00-18:00) windows
//   2: crosses both lunch and dinner windows
//   3: coincides exactly with lunch (12:00-13:00)
//   4: coincides exactly with dinner (17:00-18:00)
//   5: full day 00:00 - 23:59
//   6: entire 07:00 - 22:00
static pair<int,int> genInterval(int mode) {
    if (mode == 3) return {12*60, 13*60};
    if (mode == 4) return {17*60, 18*60};
    if (mode == 5) return {0, 23*60+59};
    if (mode == 6) return {7*60, 22*60};
    if (mode == 2) {
        // arrive before 12:00, leave after 18:00
        int a = randint(0, 12*60 - 1);
        int l = randint(18*60 + 1, 23*60 + 59);
        return {a, l};
    }
    if (mode == 1) {
        // pick a slot that lies entirely outside both meal windows
        // available slots: [0, 12*60], [13*60, 17*60], [18*60, 23*60+59]
        int slot = randint(0, 2);
        int lo, hi;
        if (slot == 0) { lo = 0; hi = 12*60; }
        else if (slot == 1) { lo = 13*60; hi = 17*60; }
        else { lo = 18*60; hi = 23*60+59; }
        if (hi - lo < 2) { lo = 18*60; hi = 23*60+59; }
        int a = randint(lo, hi - 1);
        int l = randint(a + 1, hi);
        return {a, l};
    }
    // mode 0: fully random
    int a = randint(0, 23*60 + 58);
    int l = randint(a + 1, 23*60 + 59);
    return {a, l};
}

static void writeInterval(ostream& os, pair<int,int> p) {
    os << fmtTime(p.first / 60, p.first % 60) << ' '
       << fmtTime(p.second / 60, p.second % 60) << '\n';
}

static void writeRandomSemester(ostream& os) {
    int days = randint(7, 400);
    int firstDow = randint(1, 7);
    os << days << ' ' << firstDow << '\n';
    // Ensure we cover a mix of modes across the 7 days:
    // - at least one day crossing both meal windows
    // - at least one day entirely outside meal windows
    vector<int> modes(7);
    for (int i = 0; i < 7; ++i) modes[i] = randint(0, 2); // 0 random, 1 outside, 2 crossing
    // force indices
    modes[randint(0, 6)] = 2;
    int forceOutside = randint(0, 6);
    if (modes[forceOutside] == 2) forceOutside = (forceOutside + 1) % 7;
    modes[forceOutside] = 1;
    for (int i = 0; i < 7; ++i) {
        auto p = genInterval(modes[i]);
        writeInterval(os, p);
    }
}

static void writeHeavySemester(ostream& os, int firstDow) {
    os << 400 << ' ' << firstDow << '\n';
    for (int i = 0; i < 7; ++i) {
        writeInterval(os, {7*60, 22*60});
    }
}

static void writeExtremeSemester(ostream& os, int mode) {
    // T=1, 1 day, first dow 1
    os << 1 << ' ' << 1 << '\n';
    for (int i = 0; i < 7; ++i) {
        if (mode == 0) writeInterval(os, {0, 23*60+59});
        else if (mode == 1) writeInterval(os, {12*60, 13*60});
        else writeInterval(os, {17*60, 18*60});
    }
}

static void genCaseRandom(const string& path) {
    ofstream os(path);
    int T = randint(1, 5);
    os << T << '\n';
    for (int s = 0; s < T; ++s) writeRandomSemester(os);
}

static void genCaseHeavy(const string& path, int baseDow) {
    ofstream os(path);
    os << 5 << '\n';
    for (int s = 0; s < 5; ++s) {
        int dow = ((baseDow - 1 + s) % 7) + 1;
        writeHeavySemester(os, dow);
    }
}

static void genCaseExtreme(const string& path, int mode) {
    ofstream os(path);
    os << 1 << '\n';
    writeExtremeSemester(os, mode);
}

int main() {
    // Cases 1..14: random mixes
    for (int i = 1; i <= 14; ++i) {
        string path = to_string(i) + ".in";
        genCaseRandom(path);
    }
    // Cases 15..17: heavy / constant-limit stress
    // Different starting day-of-week per semester-switch boundary across samples
    genCaseHeavy("15.in", 1);
    genCaseHeavy("16.in", 3);
    genCaseHeavy("17.in", 6);
    // Cases 18..20: extreme edge cases
    genCaseExtreme("18.in", 0); // 00:00 23:59
    genCaseExtreme("19.in", 1); // 12:00 13:00 (fully overlaps lunch)
    genCaseExtreme("20.in", 2); // 17:00 18:00 (fully overlaps dinner)
    return 0;
}
