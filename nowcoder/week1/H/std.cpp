#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

struct Person {
    double z;
    int H;
};

int main() {
    int N;
    long long T;
    while (cin >> N >> T) {
        vector<Person> a(N);
        for (int i = 0; i < N; ++i) {
            cin >> a[i].z >> a[i].H;
        }
        sort(a.begin(), a.end(), [](const Person& x, const Person& y) {
            return x.z < y.z;
        });
        double rem = (double)T;
        double cost = 0.0;
        for (int i = 0; i < N; ++i) {
            double use = min(rem, (double)a[i].H);
            cost += use * a[i].z;
            rem -= use;
            if (rem <= 0) {
                rem = 0;
                break;
            }
        }
        if (rem > 0) {
            cost += rem;
        }
        printf("%lld\n", (long long)floor(cost));
    }
    return 0;
}
