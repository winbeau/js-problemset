#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long D;
    while (cin >> N >> D) {
        vector<long long> p(N), c(N);
        for (int i = 0; i < N; ++i) cin >> p[i];
        for (int j = 0; j < N; ++j) cin >> c[j];

        sort(p.begin(), p.end());                      // ascending
        sort(c.begin(), c.end(), greater<long long>()); // descending

        using T = tuple<long long, int, int>; // (cost, i, j)
        priority_queue<T, vector<T>, greater<T>> pq;
        set<long long> visited;

        auto encode = [&](int i, int j) -> long long {
            return (long long)i * (long long)N + (long long)j;
        };

        long long firstCost = p[0] - c[0];
        // Only push if cost is strictly positive
        if (firstCost > 0) {
            pq.push({firstCost, 0, 0});
            visited.insert(encode(0, 0));
        }

        long long total = 0;
        long long k = 0;

        while (!pq.empty()) {
            auto [cost, i, j] = pq.top();
            pq.pop();

            if (cost <= 0) {
                // invalid item (cost must be > 0); try to extend neighbors but skip counting
                // According to problem, cost must be > 0, so we skip such entries.
            } else {
                if (total + cost > D) break;
                total += cost;
                k++;
                if (k >= D) {
                    // Upper bound safety
                }
            }

            if (i + 1 < N) {
                long long key = encode(i + 1, j);
                if (!visited.count(key)) {
                    visited.insert(key);
                    long long nc = p[i + 1] - c[j];
                    if (nc > 0) pq.push({nc, i + 1, j});
                }
            }
            if (j + 1 < N) {
                long long key = encode(i, j + 1);
                if (!visited.count(key)) {
                    visited.insert(key);
                    long long nc = p[i] - c[j + 1];
                    if (nc > 0) pq.push({nc, i, j + 1});
                }
            }
        }

        cout << k << " " << (D - total) << "\n";
    }

    return 0;
}
