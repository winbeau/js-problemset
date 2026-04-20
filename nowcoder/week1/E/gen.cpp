#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

long long randint(long long lo, long long hi) {
    uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

struct Case {
    int N;
    long long D;
    vector<long long> price;
    vector<long long> coupon;
};

void writeCase(int idx, const Case& c) {
    string path = "/home/winbeau/Projects/js-problemset/nowcoder/week1/E/" + to_string(idx) + ".in";
    FILE* f = fopen(path.c_str(), "w");
    fprintf(f, "%d %lld\n", c.N, c.D);
    for (int i = 0; i < c.N; i++) {
        if (i) fputc(' ', f);
        fprintf(f, "%lld", c.price[i]);
    }
    fputc('\n', f);
    for (int i = 0; i < c.N; i++) {
        if (i) fputc(' ', f);
        fprintf(f, "%lld", c.coupon[i]);
    }
    fputc('\n', f);
    fclose(f);
}

// Build a case with random N, D in given ranges, and a price value range.
// Strategy: pick min_price in [lo_price, hi_price]; coupons in [1, min_price-1]; prices in [min_price, hi_price].
Case makeRandom(int N, long long D, long long lo_price, long long hi_price) {
    Case c;
    c.N = N;
    c.D = D;
    c.price.resize(N);
    c.coupon.resize(N);
    long long min_price = randint(max<long long>(2, lo_price), hi_price);
    // if hi_price < 2 we cannot have a coupon < min_price >=1; clamp
    if (min_price < 2) min_price = 2;
    long long cmax = min_price - 1;
    for (int i = 0; i < N; i++) {
        c.coupon[i] = randint(1, cmax);
    }
    for (int i = 0; i < N; i++) {
        c.price[i] = randint(min_price, hi_price);
    }
    // guarantee at least one price equals min_price so that max(coupon) < min(price) holds tightly-ish
    c.price[0] = min_price;
    return c;
}

int main() {
    // 1..14 random, varying scales of N, D, value ranges
    struct Spec { int N; long long D; long long lo_price; long long hi_price; };
    vector<Spec> specs = {
        {1,        1,        2,        10},         // 1: tiny
        {2,        5,        2,        10},         // 2
        {5,        20,       2,        50},         // 3
        {10,       100,      2,        100},        // 4
        {50,       500,      2,        1000},       // 5
        {100,      1000,     2,        10000},      // 6
        {500,      5000,     2,        100000},     // 7
        {1000,     10000,    2,        1000000},    // 8
        {2000,     50000,    2,        10000000},   // 9
        {5000,     100000,   2,        100000000},  // 10
        {10000,    200000,   2,        1000000000}, // 11
        {20000,    500000,   2,        1000000000}, // 12
        {50000,    800000,   2,        1000000000}, // 13
        {100000,   1000000,  2,        1000000000}, // 14
    };
    for (int i = 0; i < (int)specs.size(); i++) {
        auto& s = specs[i];
        Case c = makeRandom(s.N, s.D, s.lo_price, s.hi_price);
        writeCase(i + 1, c);
    }

    // 15: N=1e5 D=1e6, all prices same, all coupons same
    {
        Case c;
        c.N = 100000;
        c.D = 1000000;
        long long p = 2, cp = 1; // every net cost = 1, can buy up to D
        c.price.assign(c.N, p);
        c.coupon.assign(c.N, cp);
        writeCase(15, c);
    }

    // 16: N=1e5 D=1e6, prices increasing, coupons decreasing
    {
        Case c;
        c.N = 100000;
        c.D = 1000000;
        c.price.resize(c.N);
        c.coupon.resize(c.N);
        long long min_price = 2;
        for (int i = 0; i < c.N; i++) {
            c.price[i] = min_price + i; // 2,3,4,...
        }
        for (int i = 0; i < c.N; i++) {
            c.coupon[i] = 1 + (c.N - 1 - i) % (min_price - 1 + 1); // stays 1 since min_price=2
            if (c.coupon[i] >= min_price) c.coupon[i] = min_price - 1;
        }
        // To really differentiate, bump min_price
        min_price = 100;
        for (int i = 0; i < c.N; i++) {
            c.price[i] = min_price + i;
        }
        for (int i = 0; i < c.N; i++) {
            c.coupon[i] = (min_price - 1) - (i % (min_price - 1)); // decreasing wave
            if (c.coupon[i] < 1) c.coupon[i] = 1;
            if (c.coupon[i] >= min_price) c.coupon[i] = min_price - 1;
        }
        writeCase(16, c);
    }

    // 17: N=1e5 D=1e6, prices uniform random in large range, coupons uniform random below min_price
    {
        Case c = makeRandom(100000, 1000000, 1000, 1000000000);
        writeCase(17, c);
    }

    // 18: N=1 D=1
    {
        Case c;
        c.N = 1;
        c.D = 1;
        c.price = {2};
        c.coupon = {1};
        writeCase(18, c);
    }

    // 19: N=1 D=1e6
    {
        Case c;
        c.N = 1;
        c.D = 1000000;
        c.price = {1000000000};
        c.coupon = {999999999};
        writeCase(19, c);
    }

    // 20: N=2, boundary D where 0/1/2/3 purchases are possible
    // Pair costs: (p_i - c_j). With N=2, at most 2 purchases possible.
    // Let prices = {10, 20}, coupons = {5, 9}. min_price=10, max_coupon=9 OK.
    // Best pairing to maximize items with min sum: match largest coupon to smallest price? We want at most 2 items; smallest two net costs = 10-9=1, 20-5=15; total=16.
    // Pick D=15: can afford only 1 item (cheapest net cost = 1). D=16: exactly 2.
    {
        Case c;
        c.N = 2;
        c.D = 16;
        c.price = {10, 20};
        c.coupon = {5, 9};
        writeCase(20, c);
    }

    return 0;
}
