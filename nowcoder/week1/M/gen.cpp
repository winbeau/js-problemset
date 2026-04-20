#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

int main() {
    std::mt19937 rng(20240427);

    auto write_case = [](int idx, const std::vector<int>& a) {
        char name[64];
        std::snprintf(name, sizeof(name), "%d.in", idx);
        FILE* fp = std::fopen(name, "wb");
        if (!fp) {
            std::fprintf(stderr, "cannot open %s\n", name);
            std::exit(1);
        }
        // Build the line with a reasonably large buffer and fwrite.
        std::string buf;
        buf.reserve(a.size() * 8);
        for (size_t i = 0; i < a.size(); ++i) {
            if (i) buf.push_back(',');
            char tmp[16];
            int len = std::snprintf(tmp, sizeof(tmp), "%d", a[i]);
            buf.append(tmp, tmp + len);
        }
        buf.push_back('\n');
        std::fwrite(buf.data(), 1, buf.size(), fp);
        std::fclose(fp);
    };

    const int LO = -32768;
    const int HI = 32767;

    auto rand_int = [&](int lo, int hi) {
        std::uniform_int_distribution<int> d(lo, hi);
        return d(rng);
    };

    // Cases 1..14: random, size 1..10000, values in [-32768, 32767].
    for (int i = 1; i <= 14; ++i) {
        int n = rand_int(1, 10000);
        std::vector<int> a(n);
        for (int j = 0; j < n; ++j) a[j] = rand_int(LO, HI);
        write_case(i, a);
    }

    // Case 15: 1,000,000 random values.
    {
        int n = 1000000;
        std::vector<int> a(n);
        for (int j = 0; j < n; ++j) a[j] = rand_int(LO, HI);
        write_case(15, a);
    }

    // Case 16: 1,000,000 already ascending.
    {
        int n = 1000000;
        std::vector<int> a(n);
        for (int j = 0; j < n; ++j) a[j] = rand_int(LO, HI);
        std::sort(a.begin(), a.end());
        write_case(16, a);
    }

    // Case 17: 1,000,000 already descending.
    {
        int n = 1000000;
        std::vector<int> a(n);
        for (int j = 0; j < n; ++j) a[j] = rand_int(LO, HI);
        std::sort(a.begin(), a.end(), std::greater<int>());
        write_case(17, a);
    }

    // Case 18: exactly 1 number.
    {
        std::vector<int> a(1);
        a[0] = rand_int(LO, HI);
        write_case(18, a);
    }

    // Case 19: exactly 2 numbers.
    {
        std::vector<int> a(2);
        a[0] = rand_int(LO, HI);
        a[1] = rand_int(LO, HI);
        write_case(19, a);
    }

    // Case 20: all identical values.
    {
        int n = rand_int(100, 10000);
        int v = rand_int(LO, HI);
        std::vector<int> a(n, v);
        write_case(20, a);
    }

    return 0;
}
