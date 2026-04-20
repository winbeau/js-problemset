// Generator for 中南大学保研机试·奇怪的餐厅
// Writes 1.in .. 20.in
// Seed: mt19937 rng(20240427);

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

static std::mt19937 rng(20240427);

static inline int randint(int lo, int hi) {
    std::uniform_int_distribution<int> d(lo, hi);
    return d(rng);
}

static inline long long randll(long long lo, long long hi) {
    std::uniform_int_distribution<long long> d(lo, hi);
    return d(rng);
}

// generate z as two-decimal value in [0.10, 1.00]
static inline double randZ() {
    int t = randint(10, 100);
    return t / 100.0;
}

static FILE* openCase(int idx) {
    char name[64];
    std::snprintf(name, sizeof(name), "%d.in", idx);
    FILE* fp = std::fopen(name, "w");
    if (!fp) {
        std::fprintf(stderr, "cannot open %s\n", name);
        std::exit(1);
    }
    return fp;
}

// ---------- random cases (1..14) ----------
static void genRandom(int idx) {
    FILE* fp = openCase(idx);
    int groups = randint(1, 5);
    for (int g = 0; g < groups; ++g) {
        int N = randint(1, 99);
        long long T = randll(1, 1000000LL);
        std::fprintf(fp, "%d %lld\n", N, T);
        for (int i = 0; i < N; ++i) {
            double z = randZ();
            int H = randint(1, 100000);
            std::fprintf(fp, "%.2f %d\n", z, H);
        }
    }
    std::fclose(fp);
}

// ---------- heavy cases (15..17) ----------
static void genHeavy(int idx) {
    FILE* fp = openCase(idx);
    int groups = 5;
    for (int g = 0; g < groups; ++g) {
        int N = 99;
        // T at 1e9 scale
        long long T = randll(500000000LL, 1000000000LL);
        std::fprintf(fp, "%d %lld\n", N, T);
        for (int i = 0; i < N; ++i) {
            // diverse z
            double z = randZ();
            // wide H distribution: mix small, medium, large
            int bucket = randint(0, 2);
            int H;
            if (bucket == 0) H = randint(1, 100);
            else if (bucket == 1) H = randint(101, 10000);
            else H = randint(10001, 100000);
            std::fprintf(fp, "%.2f %d\n", z, H);
        }
    }
    std::fclose(fp);
}

// ---------- extreme cases (18..20) ----------

// Case 18: N=1 with multiple groups, mixing boundary conditions.
static void genExtreme18(int idx) {
    FILE* fp = openCase(idx);
    // several groups all with N=1
    int groups = 5;
    for (int g = 0; g < groups; ++g) {
        int N = 1;
        long long T = randll(1, 1000000LL);
        double z = randZ();
        int H = randint(1, 100000);
        std::fprintf(fp, "%d %lld\n", N, T);
        std::fprintf(fp, "%.2f %d\n", z, H);
    }
    std::fclose(fp);
}

// Case 19: all z=1.0 (no discount); also T equal to sum(H) for one group.
static void genExtreme19(int idx) {
    FILE* fp = openCase(idx);
    int groups = 3;
    for (int g = 0; g < groups; ++g) {
        int N = randint(1, 99);
        std::vector<int> Hs(N);
        long long sumH = 0;
        for (int i = 0; i < N; ++i) {
            Hs[i] = randint(1, 100000);
            sumH += Hs[i];
        }
        long long T;
        if (g == 0) {
            // T exactly equals sum(H)
            T = sumH;
        } else if (g == 1) {
            // T larger than sum(H)
            T = sumH + randll(1, 100000LL);
        } else {
            // T smaller than sum(H)
            T = std::max<long long>(1, sumH - randll(1, std::max<long long>(1, sumH - 1)));
        }
        std::fprintf(fp, "%d %lld\n", N, T);
        for (int i = 0; i < N; ++i) {
            std::fprintf(fp, "%.2f %d\n", 1.00, Hs[i]);
        }
    }
    std::fclose(fp);
}

// Case 20: all z identical and extremely small; T far smaller than min(H).
static void genExtreme20(int idx) {
    FILE* fp = openCase(idx);
    int groups = 3;
    for (int g = 0; g < groups; ++g) {
        int N = randint(50, 99);
        // pick a common small z (two-decimal) like 0.10
        double z = 0.10;
        // ensure min H is large, T far smaller than min(H)
        int minH = 50000;
        std::vector<int> Hs(N);
        for (int i = 0; i < N; ++i) {
            Hs[i] = randint(minH, 100000);
        }
        long long actualMin = *std::min_element(Hs.begin(), Hs.end());
        long long T = randll(1, std::max<long long>(1, actualMin / 100));
        std::fprintf(fp, "%d %lld\n", N, T);
        for (int i = 0; i < N; ++i) {
            std::fprintf(fp, "%.2f %d\n", z, Hs[i]);
        }
    }
    std::fclose(fp);
}

int main() {
    for (int i = 1; i <= 14; ++i) genRandom(i);
    for (int i = 15; i <= 17; ++i) genHeavy(i);
    genExtreme18(18);
    genExtreme19(19);
    genExtreme20(20);
    return 0;
}
