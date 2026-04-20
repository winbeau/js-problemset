// Generator for 士兵排阵 (Manhattan arrangement into a horizontal row)
// Writes 1.in .. 20.in
// Each file may contain multiple test groups, read until EOF.
// Group format:
//   n
//   x_1 y_1
//   ...
//   x_n y_n
//
// Ranges: n in [1, 1e4], x,y in [-1e4, 1e4].
//
// 1..14  random mixes (n from small to 1e4; coords covering ±1e4)
// 15..17 stress / adversarial (3-5 groups, each n = 1e4)
// 18..20 extreme (n=1; n=2; all soldiers at same coord)

#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

static const int COORD_MAX = 10000;
static const int N_MAX     = 10000;

static int randInt(int lo, int hi) {
    uniform_int_distribution<int> d(lo, hi);
    return d(rng);
}

struct Writer {
    FILE* fp;
    Writer(const string& path) {
        fp = fopen(path.c_str(), "w");
        if (!fp) {
            fprintf(stderr, "cannot open %s\n", path.c_str());
            exit(1);
        }
    }
    ~Writer() { if (fp) fclose(fp); }
    void group(const vector<pair<int,int>>& pts) {
        fprintf(fp, "%d\n", (int)pts.size());
        for (auto& p : pts) fprintf(fp, "%d %d\n", p.first, p.second);
    }
};

// Random uniform point set
static vector<pair<int,int>> genRandom(int n, int lo = -COORD_MAX, int hi = COORD_MAX) {
    vector<pair<int,int>> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) {
        v.push_back({randInt(lo, hi), randInt(lo, hi)});
    }
    return v;
}

// All same y
static vector<pair<int,int>> genSameY(int n) {
    int y = randInt(-COORD_MAX, COORD_MAX);
    vector<pair<int,int>> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back({randInt(-COORD_MAX, COORD_MAX), y});
    return v;
}

// All same x (stacked vertically)
static vector<pair<int,int>> genSameX(int n) {
    int x = randInt(-COORD_MAX, COORD_MAX);
    vector<pair<int,int>> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back({x, randInt(-COORD_MAX, COORD_MAX)});
    return v;
}

// Coordinates at extremes (±COORD_MAX)
static vector<pair<int,int>> genExtreme(int n) {
    vector<pair<int,int>> v;
    v.reserve(n);
    int choices[2] = {-COORD_MAX, COORD_MAX};
    for (int i = 0; i < n; ++i) {
        v.push_back({choices[randInt(0,1)], choices[randInt(0,1)]});
    }
    return v;
}

// All same point
static vector<pair<int,int>> genAllSame(int n) {
    int x = randInt(-COORD_MAX, COORD_MAX);
    int y = randInt(-COORD_MAX, COORD_MAX);
    return vector<pair<int,int>>(n, {x, y});
}

static string path(int idx) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%d.in", idx);
    return string(buf);
}

int main() {
    // ------- 1..14 random mixed ---------
    // Distribute n sizes from very small up to 1e4.
    // Each file has several groups; total workload varies.
    struct Plan { vector<int> ns; int lo, hi; };
    vector<Plan> plans(14);
    // small
    plans[0]  = {{1,1,2,2,3,5,8}, -10, 10};
    plans[1]  = {{1,2,3,4,5,6,7,8,9,10}, -100, 100};
    plans[2]  = {{10,15,20,25,30}, -1000, 1000};
    plans[3]  = {{50, 80, 100}, -COORD_MAX, COORD_MAX};
    // medium
    plans[4]  = {{100, 200, 300}, -COORD_MAX, COORD_MAX};
    plans[5]  = {{500, 500, 500}, -COORD_MAX, COORD_MAX};
    plans[6]  = {{1000, 1000}, -COORD_MAX, COORD_MAX};
    plans[7]  = {{2000, 2000}, -COORD_MAX, COORD_MAX};
    // larger
    plans[8]  = {{3000, 3000}, -COORD_MAX, COORD_MAX};
    plans[9]  = {{5000}, -COORD_MAX, COORD_MAX};
    plans[10] = {{7000}, -COORD_MAX, COORD_MAX};
    plans[11] = {{9000}, -COORD_MAX, COORD_MAX};
    // big plus a mix
    plans[12] = {{10, 1000, 10000}, -COORD_MAX, COORD_MAX};
    plans[13] = {{10000}, -COORD_MAX, COORD_MAX};

    for (int i = 0; i < 14; ++i) {
        Writer w(path(i + 1));
        for (int n : plans[i].ns) {
            auto pts = genRandom(n, plans[i].lo, plans[i].hi);
            w.group(pts);
        }
    }

    // ------- 15..17 adversarial, 3-5 groups each n=1e4 -------
    {
        Writer w(path(15));
        // 5 groups, each n = N_MAX, one same-y, one same-x, one extreme, two random
        w.group(genSameY(N_MAX));
        w.group(genSameX(N_MAX));
        w.group(genExtreme(N_MAX));
        w.group(genRandom(N_MAX));
        w.group(genRandom(N_MAX));
    }
    {
        Writer w(path(16));
        // 4 groups
        w.group(genSameY(N_MAX));
        w.group(genSameX(N_MAX));
        w.group(genExtreme(N_MAX));
        w.group(genRandom(N_MAX));
    }
    {
        Writer w(path(17));
        // 3 groups
        w.group(genSameY(N_MAX));
        w.group(genSameX(N_MAX));
        w.group(genExtreme(N_MAX));
    }

    // ------- 18..20 extreme ---------
    {
        // single group n = 1
        Writer w(path(18));
        vector<pair<int,int>> v = {{randInt(-COORD_MAX, COORD_MAX), randInt(-COORD_MAX, COORD_MAX)}};
        w.group(v);
    }
    {
        // single group n = 2
        Writer w(path(19));
        vector<pair<int,int>> v = {
            {randInt(-COORD_MAX, COORD_MAX), randInt(-COORD_MAX, COORD_MAX)},
            {randInt(-COORD_MAX, COORD_MAX), randInt(-COORD_MAX, COORD_MAX)}
        };
        w.group(v);
    }
    {
        // single group, all soldiers at same point, moderately large n
        Writer w(path(20));
        w.group(genAllSame(N_MAX));
    }

    return 0;
}
