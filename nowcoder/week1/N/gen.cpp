#include <bits/stdc++.h>
using namespace std;

mt19937 rng(20240427);

static const int MAXV = 100000000;

void write_case(const string& path, int N, const vector<int>& vals) {
    FILE* fp = fopen(path.c_str(), "w");
    fprintf(fp, "%d\n", N);
    for (int i = 0; i < N; ++i) {
        fprintf(fp, "%d%c", vals[i], (i + 1 == N) ? '\n' : ' ');
    }
    fclose(fp);
}

vector<int> gen_random_distinct(int N, int lo, int hi) {
    vector<int> res;
    res.reserve(N);
    if ((long long)(hi - lo + 1) <= (long long)N * 4) {
        // dense: shuffle a range
        vector<int> pool;
        pool.reserve(hi - lo + 1);
        for (int v = lo; v <= hi; ++v) pool.push_back(v);
        shuffle(pool.begin(), pool.end(), rng);
        for (int i = 0; i < N; ++i) res.push_back(pool[i]);
    } else {
        unordered_set<int> used;
        used.reserve(N * 2);
        uniform_int_distribution<int> dist(lo, hi);
        while ((int)res.size() < N) {
            int v = dist(rng);
            if (used.insert(v).second) res.push_back(v);
        }
    }
    return res;
}

int main() {
    // Cases 1..14: random N from 1..100, values covering 1..1e8
    // Use various N and value ranges.
    struct RandCfg { int N; int lo; int hi; };
    vector<RandCfg> randCfgs = {
        {1, 1, MAXV},
        {2, 1, MAXV},
        {3, 1, 10},
        {5, 1, 100},
        {10, 1, 1000},
        {15, 1, 10000},
        {20, 1, 100000},
        {30, 1, 1000000},
        {40, 1, 10000000},
        {50, 1, MAXV},
        {60, 1, MAXV},
        {75, 1, MAXV},
        {90, 1, MAXV},
        {100, 1, MAXV},
    };

    for (int i = 0; i < 14; ++i) {
        int N = randCfgs[i].N;
        int lo = randCfgs[i].lo, hi = randCfgs[i].hi;
        vector<int> vals = gen_random_distinct(N, lo, hi);
        write_case(to_string(i + 1) + ".in", N, vals);
    }

    // Case 15: N=100 strictly ascending -> right chain
    {
        int N = 100;
        vector<int> vals;
        vals.reserve(N);
        // pick 100 distinct values sorted ascending across 1..1e8
        vector<int> pick = gen_random_distinct(N, 1, MAXV);
        sort(pick.begin(), pick.end());
        write_case("15.in", N, pick);
    }

    // Case 16: N=100 strictly descending -> left chain
    {
        int N = 100;
        vector<int> pick = gen_random_distinct(N, 1, MAXV);
        sort(pick.begin(), pick.end(), greater<int>());
        write_case("16.in", N, pick);
    }

    // Case 17: N=100 random
    {
        int N = 100;
        vector<int> vals = gen_random_distinct(N, 1, MAXV);
        write_case("17.in", N, vals);
    }

    // Case 18: N=1
    {
        int N = 1;
        uniform_int_distribution<int> dist(1, MAXV);
        vector<int> vals = {dist(rng)};
        write_case("18.in", N, vals);
    }

    // Case 19: N=2
    {
        int N = 2;
        vector<int> vals = gen_random_distinct(N, 1, MAXV);
        write_case("19.in", N, vals);
    }

    // Case 20: N=100 values 1..100 (shuffled to be distinct order; problem says distinct)
    {
        int N = 100;
        vector<int> vals;
        vals.reserve(N);
        for (int v = 1; v <= 100; ++v) vals.push_back(v);
        shuffle(vals.begin(), vals.end(), rng);
        write_case("20.in", N, vals);
    }

    return 0;
}
